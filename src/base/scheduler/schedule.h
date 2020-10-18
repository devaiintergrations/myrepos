#pragma once

#include <QJsonObject>
#include <QObject>
#include <QPointer>
#include <QVector>

#include "base/global.h"
#include "scheduleday.h"

class QThread;

class Application;
class AsyncFileStorage;

namespace Scheduler
{
    class Schedule final : public QObject
    {
        Q_OBJECT
        Q_DISABLE_COPY(Schedule)

        friend class ::Application;

        Schedule();
        ~Schedule() override;

    public:
        static Schedule *instance();
        QVector<ScheduleDay*> scheduleDays() const;

    public slots:
        void updateSchedule(int day);

    signals:
        void updated(int day);

    private:
        static QPointer<Schedule> m_instance;

        QVector<ScheduleDay*> m_scheduleDays;
        QThread *m_ioThread;
        AsyncFileStorage *m_fileStorage;

        bool loadSchedule();
        void saveSchedule();
    };
}
