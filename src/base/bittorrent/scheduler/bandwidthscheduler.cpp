/*
 * Bittorrent Client using Qt and libtorrent.
 * Copyright (C) 2017  Vladimir Golovnev <glassez@yandex.ru>
 * Copyright (C) 2010  Christophe Dumez <chris@qbittorrent.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * In addition, as a special exception, the copyright holders give permission to
 * link this program with the OpenSSL project's "OpenSSL" library (or with
 * modified versions of it that use the same license as the "OpenSSL" library),
 * and distribute the linked executables. You must obey the GNU General Public
 * License in all respects for all of the code used other than "OpenSSL".  If you
 * modify file(s), you may extend this exception to your version of the file(s),
 * but you are not obligated to do so. If you do not wish to do so, delete this
 * exception statement from your version.
 */

#include "bandwidthscheduler.h"

#include <chrono>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPointer>

#include "base/logger.h"
#include "base/preferences.h"
#include "base/profile.h"
#include "base/utils/fs.h"

using namespace std::chrono_literals;

QPointer<BandwidthScheduler> BandwidthScheduler::m_instance = nullptr;

BandwidthScheduler::BandwidthScheduler(QObject *parent)
    : QObject {parent}
    , m_ioThread {new QThread(this)}
{
    Q_ASSERT(!m_instance); // only one instance is allowed
    m_instance = this;

    m_fileStorage = new AsyncFileStorage(specialFolderLocation(SpecialFolder::Config));

    if (!m_fileStorage)
        throw RuntimeError(tr("Directory for scheduler data is unavailable."));

    m_fileStorage->moveToThread(m_ioThread);
    connect(m_ioThread, &QThread::finished, m_fileStorage, &AsyncFileStorage::deleteLater);
    connect(m_fileStorage, &AsyncFileStorage::failed, [](const Path &fileName, const QString &errorString)
    {
        LogMsg(tr("Couldn't save scheduler data in %1. Error: %2")
                .arg(fileName.toString(), errorString), Log::CRITICAL);
    });

    m_ioThread->start();

    if (!loadScheduleFromDisk())
    {
        for (int day = 0; day < 7; ++day)
            m_schedule[day] = new ScheduleDay(day);

        commitSchedule(true);
    }

    connect(this, &BandwidthScheduler::scheduleUpdated, this, [this](int day)
    {
        if (day == QDate::currentDate().dayOfWeek() - 1)
            emit limitChangeRequested();
    });

    connect(&m_timer, &QTimer::timeout, this, &BandwidthScheduler::limitChangeRequested);
}

BandwidthScheduler::~BandwidthScheduler()
{
    m_timer.stop();

    m_ioThread->quit();
    m_ioThread->wait();
}

BandwidthScheduler* BandwidthScheduler::instance()
{
    return m_instance;
}

void BandwidthScheduler::start()
{
    emit limitChangeRequested();
    m_timer.start(10s);
}

void BandwidthScheduler::stop()
{
    m_timer.stop();
}

void BandwidthScheduler::backupSchedule(const QString &errorMessage, bool preserveOriginal = false) const
{
    LogMsg(errorMessage, Log::CRITICAL);

    Path filePath = m_fileStorage->storageDir() / Path(SCHEDULE_FILE_NAME);
    Path errorFilePath = filePath + u".error"_s;

    int counter = 0;
    while (errorFilePath.exists())
    {
        ++counter;
        errorFilePath = filePath + u".error"_s + QString::number(counter);
    }

    LogMsg(tr("Backing up errored schedule file in %1").arg(errorFilePath.toString()), Log::WARNING);

    if (preserveOriginal)
        QFile::copy(filePath.toString(), errorFilePath.toString());
    else
        QFile::rename(filePath.toString(), errorFilePath.toString());
}

bool BandwidthScheduler::loadScheduleFromDisk()
{
    QFile file {(m_fileStorage->storageDir() / Path(SCHEDULE_FILE_NAME)).data()};

    if (!file.exists())
        return importLegacyScheduler();

    if (!file.open(QFile::ReadOnly) || file.size() == 0)
        return false;

    QJsonParseError jsonError;
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll(), &jsonError);

    if (jsonError.error != QJsonParseError::NoError)
    {
        file.close();
        backupSchedule(tr("Scheduler JSON parsing error: %1").arg(jsonError.errorString()));
        return false;
    }

    if (!jsonDoc.isObject())
    {
        file.close();
        backupSchedule(tr("Invalid scheduler JSON format"));
        return false;
    }

    const QJsonObject jsonObj = jsonDoc.object();
    bool errored = false;

    for (int day = 0; day < 7; ++day)
    {
        if (!jsonObj[DAY_KEYS[day]].isArray())
        {
            LogMsg(tr("Ignoring invalid value for schedule day: %1 (expected an array)")
                    .arg(DAY_KEYS[day]), Log::WARNING);

            errored = true;
            m_schedule[day] = new ScheduleDay(day);
            continue;
        }

        QJsonArray arr = jsonObj[DAY_KEYS[day]].toArray();
        m_schedule[day] = ScheduleDay::fromJsonArray(arr, day, &errored);
    }

    commitSchedule(false);

    if (errored)
    {
        backupSchedule(tr("There were invalid data in the scheduler JSON file that have been ignored."), true);
        saveScheduleToDisk();
    }

    return true;
}

void BandwidthScheduler::saveScheduleToDisk() const
{
    m_fileStorage->store(Path(SCHEDULE_FILE_NAME), getJson(true));
}

QByteArray BandwidthScheduler::getJson(bool onDisk) const
{
    QJsonObject jsonObj;

    for (int day = 0; day < 7; ++day)
        jsonObj.insert(DAY_KEYS[day], scheduleDay(day, onDisk)->toJsonArray());

    return QJsonDocument(jsonObj).toJson();
}

bool BandwidthScheduler::importLegacyScheduler()
{
    Preferences *pref = Preferences::instance();
    const QTime start = pref->getLegacySchedulerStartTime();
    const QTime end = pref->getLegacySchedulerEndTime();
    const Scheduler::Days schedulerDays = pref->getLegacySchedulerDays();

    if (!start.isValid() || !end.isValid() || static_cast<int>(schedulerDays) == -1)
        return false;

    LogMsg(tr("Bandwidth scheduler format has been changed. Attempting to transfer into the new JSON format."), Log::INFO);

    const int altDownloadLimit = pref->getGlobalAltDownloadLimit();
    const int altUploadLimit = pref->getGlobalAltUploadLimit();

    for (int day = 0; day < 7; ++day)
    {
        bool shouldAdd = (schedulerDays == Scheduler::Days::EveryDay)
            || (schedulerDays == Scheduler::Days::Weekday && (day != 5 && day != 6))
            || (schedulerDays == Scheduler::Days::Weekend && (day == 5 || day == 6))
            || (day == static_cast<int>(schedulerDays) - 3);

        auto *scheduleDay = new ScheduleDay(day);

        if (shouldAdd)
        {
            if (start > end)
            {
                scheduleDay->addEntry({
                    QTime(0, 0), end,
                    altDownloadLimit, altUploadLimit, false
                });

                scheduleDay->addEntry({
                    start, QTime(23, 59, 59, 999),
                    altDownloadLimit, altUploadLimit, false
                });
            }
            else
            {
                scheduleDay->addEntry({
                    start, end,
                    altDownloadLimit, altUploadLimit, false
                });
            }
        }

        m_schedule[day] = scheduleDay;
    }

    commitSchedule(true);
    pref->removeLegacySchedulerTimes();

    LogMsg(tr("Successfully transferred the old scheduler into the new JSON format."), Log::INFO);
    return true;
}

ScheduleDay* BandwidthScheduler::scheduleDay(const int day, bool onDisk) const
{
    return onDisk ? m_scheduleOnDisk[day] : m_schedule[day];
}

ScheduleDay* BandwidthScheduler::today(bool onDisk) const
{
    return scheduleDay(QDate::currentDate().dayOfWeek() - 1, onDisk);
}

void BandwidthScheduler::commitSchedule(bool saveToDisk = true)
{
    for (int day = 0; day < 7; ++day)
        m_scheduleOnDisk[day] = new ScheduleDay(day, m_schedule.at(day)->entries());

    if (saveToDisk)
        saveScheduleToDisk();
}

void BandwidthScheduler::revertSchedule()
{
    for (int day = 0; day < 7; ++day)
        m_schedule[day] = new ScheduleDay(day, m_scheduleOnDisk.at(day)->entries());
}
