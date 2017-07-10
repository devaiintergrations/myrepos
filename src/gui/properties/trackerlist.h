/*
 * Bittorrent Client using Qt and libtorrent.
 * Copyright (C) 2006  Christophe Dumez <chris@qbittorrent.org>
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

#ifndef TRACKERLIST_H
#define TRACKERLIST_H

#include <QClipboard>
#include <QList>
#include <QShortcut>
#include <QTreeWidget>

#include "propertieswidget.h"

#define NB_STICKY_ITEM 3

namespace BitTorrent
{
    class TorrentHandle;
}

class TrackerList: public QTreeWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(TrackerList)

public:
    enum TrackerListColumn
    {
        COL_TIER,
        COL_URL,
        COL_STATUS,
        COL_RECEIVED,
        COL_SEEDS,
        COL_PEERS,
        COL_DOWNLOADED,
        COL_MSG,

        COL_COUNT
    };

    TrackerList(PropertiesWidget *properties);
    ~TrackerList();

    int visibleColumnsCount() const;

public slots:
    void setRowColor(int row, QColor color);

    void moveSelectionUp();
    void moveSelectionDown();

    void clear();
    void loadStickyItems(BitTorrent::TorrentHandle *const torrent);
    void loadTrackers();
    void askForTrackers();
    void copyTrackerUrl();
    void reannounceSelected();
    void deleteSelectedTrackers();
    void editSelectedTracker();
    void showTrackerListMenu(QPoint);
    void displayToggleColumnsMenu(const QPoint &);
    void loadSettings();
    void saveSettings() const;

protected:
    QList<QTreeWidgetItem *> getSelectedTrackerItems() const;

private:
    PropertiesWidget *m_properties;
    QHash<QString, QTreeWidgetItem *> m_trackerItems;
    QTreeWidgetItem *m_DHTItem;
    QTreeWidgetItem *m_PEXItem;
    QTreeWidgetItem *m_LSDItem;
    QShortcut *m_editHotkey;
    QShortcut *m_deleteHotkey;
    QShortcut *m_copyHotkey;

    static QStringList headerLabels();
};

#endif // TRACKERLIST_H
