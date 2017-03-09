/*
 * Bittorrent Client using Qt and libtorrent.
 * Copyright (C) 2010  Christophe Dumez
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
 *
 * Contact : chris@qbittorrent.org
 */

#include "rssdownloadrulelist.h"

#include <QFile>
#include <QDataStream>
#include <QDebug>

#include "base/preferences.h"
#include "base/qinisettings.h"
#include "rssdownloadrule_p.h"

using namespace Rss;

DownloadRuleList::DownloadRuleList()
{
    loadRulesFromStorage();
}

DownloadRule DownloadRuleList::findMatchingRule(const QString &feedUrl, const QString &articleTitle) const
{
    Q_ASSERT(Preferences::instance()->isRssDownloadingEnabled());
    qDebug() << "Matching article:" << articleTitle;
    QStringList ruleNames = m_feedRules.value(feedUrl);
    foreach (const QString &rule_name, ruleNames) {
        DownloadRule rule = m_rules[rule_name];
        if (rule.isEnabled() && rule.matches(articleTitle)) return rule;
    }
    return DownloadRule();
}

void DownloadRuleList::replace(const DownloadRuleList &other)
{
    m_rules.clear();
    m_feedRules.clear();
    merge(other);
}

void DownloadRuleList::merge(const DownloadRuleList &other)
{
    foreach (const QString &name, other.ruleNames())
        saveRule(other.getRule(name));
}

void DownloadRuleList::saveRulesToStorage()
{
    QIniSettings qBTRSS("qBittorrent", "qBittorrent-rss");
    qBTRSS.setValue("download_rules", toVariantHash());
}

void DownloadRuleList::loadRulesFromStorage()
{
    QIniSettings qBTRSS("qBittorrent", "qBittorrent-rss");
    loadRulesFromVariantHash(qBTRSS.value("download_rules").toHash());
}

QVariantHash DownloadRuleList::toVariantHash() const
{
    QVariantHash ret;
    foreach (const DownloadRule &rule, m_rules) {
        ret.insert(rule.name(), rule.toVariantHash());
    }
    return ret;
}

void DownloadRuleList::loadRulesFromVariantHash(const QVariantHash &h)
{
    foreach (const QVariant &r, h) {
        DownloadRule rule = DownloadRule::fromVariantHash(r.toHash());
        if (!rule.name().isEmpty())
            saveRule(rule);
    }
}

void DownloadRuleList::saveRule(const DownloadRule &rule)
{
    qDebug() << Q_FUNC_INFO << rule.name();
    if (m_rules.contains(rule.name())) {
        if (m_rules[rule.name()].isSame(rule)) {
            qDebug("Rule already exists, and is unchanged. Skipping");
            return;
        }

        qDebug("This is an update, removing old rule first");
        removeRule(rule.name());
    }
    m_rules.insert(rule.name(), rule);
    // Update feedRules hashtable
    foreach (const QString &feedUrl, rule.rssFeeds()) {
        m_feedRules[feedUrl].append(rule.name());
    }
    qDebug() << Q_FUNC_INFO << "EXIT";
}

void DownloadRuleList::removeRule(const QString &name)
{
    qDebug() << Q_FUNC_INFO << name;
    if (!m_rules.contains(name)) return;
    DownloadRule rule = m_rules.take(name);
    // Update feedRules hashtable
    foreach (const QString &feedUrl, rule.rssFeeds()) {
        m_feedRules[feedUrl].removeOne(rule.name());
    }
}

void DownloadRuleList::renameRule(const QString &oldName, const QString &newName)
{
    if (!m_rules.contains(oldName)) return;

    DownloadRule rule = m_rules.take(oldName);
    rule.setName(newName);
    m_rules.insert(newName, rule);
    // Update feedRules hashtable
    foreach (const QString &feedUrl, rule.rssFeeds()) {
        m_feedRules[feedUrl].replace(m_feedRules[feedUrl].indexOf(oldName), newName);
    }
}

void DownloadRuleList::updateFeedURL(const QString &oldUrl, const QString &newUrl)
{
    if (m_feedRules.contains(oldUrl)) {
        QStringList feedRules = m_feedRules.take(oldUrl);
        m_feedRules.insert(newUrl, feedRules);
        foreach (const QString &ruleName, feedRules) {
            QStringList ruleFeeds = m_rules[ruleName].rssFeeds();
            ruleFeeds.replace(ruleFeeds.indexOf(oldUrl), newUrl);
            m_rules[ruleName].setRssFeeds(ruleFeeds);
        }
    }
}

DownloadRule DownloadRuleList::getRule(const QString &name) const
{
    return m_rules.value(name);
}

QStringList DownloadRuleList::ruleNames() const
{
    return m_rules.keys();
}

bool DownloadRuleList::isEmpty() const
{
    return m_rules.isEmpty();
}

bool DownloadRuleList::serialize(const QString &path)
{
    QFile f(path);
    if (f.open(QIODevice::WriteOnly)) {
        QDataStream out(&f);
        out.setVersion(QDataStream::Qt_4_5);
        out << toVariantHash();
        f.close();
        return true;
    }

    return false;
}

bool DownloadRuleList::unserialize(const QString &path)
{
    QFile f(path);
    if (f.open(QIODevice::ReadOnly)) {
        QDataStream in(&f);
        in.setVersion(QDataStream::Qt_4_5);
        QVariantHash tmp;
        in >> tmp;
        f.close();
        if (tmp.isEmpty())
            return false;
        qDebug("Processing was successful!");
        loadRulesFromVariantHash(tmp);
        return true;
    } else {
        qDebug("Error: could not open file at %s", qPrintable(path));
        return false;
    }
}
