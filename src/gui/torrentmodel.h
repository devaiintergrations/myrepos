/*
 * Bittorrent Client using Qt4 and libtorrent.
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

#ifndef TORRENTMODEL_H
#define TORRENTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QDateTime>
#include <QIcon>
#include <QTimer>

#include "core/bittorrent/torrenthandle.h"

struct TorrentStatusReport {
    TorrentStatusReport();
    uint nb_downloading;
    uint nb_seeding;
    uint nb_completed;
    uint nb_active;
    uint nb_inactive;
    uint nb_paused;
    uint nb_resumed;
};

class TorrentModelItem : public QObject {
    Q_OBJECT

public:
    enum Column {TR_NAME, TR_PRIORITY, TR_SIZE, TR_TOTAL_SIZE, TR_PROGRESS, TR_STATUS, TR_SEEDS, TR_PEERS, TR_DLSPEED, TR_UPSPEED, TR_ETA, TR_RATIO, TR_LABEL, TR_ADD_DATE, TR_SEED_DATE, TR_TRACKER, TR_DLLIMIT, TR_UPLIMIT, TR_AMOUNT_DOWNLOADED, TR_AMOUNT_UPLOADED, TR_AMOUNT_LEFT, TR_TIME_ELAPSED, TR_SAVE_PATH, TR_COMPLETED, TR_RATIO_LIMIT, TR_SEEN_COMPLETE_DATE, TR_LAST_ACTIVITY, TR_AMOUNT_DOWNLOADED_SESSION, TR_AMOUNT_UPLOADED_SESSION, NB_COLUMNS};

public:
    TorrentModelItem(BitTorrent::TorrentHandle *torrent);
    inline int columnCount() const { return NB_COLUMNS; }
    QVariant data(int column, int role = Qt::DisplayRole) const;
    bool setData(int column, const QVariant &value, int role = Qt::DisplayRole);
    inline BitTorrent::InfoHash hash() const { return m_torrent->hash(); }
    BitTorrent::TorrentState state() const;
    BitTorrent::TorrentHandle *torrentHandle() const;

signals:
    void labelChanged(QString previous, QString current);

private:
    static QIcon getIconByState(BitTorrent::TorrentState state);
    static QColor getColorByState(BitTorrent::TorrentState state);

private:
    BitTorrent::TorrentHandle *m_torrent;
};

class TorrentModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY(TorrentModel)

public:
    explicit TorrentModel(QObject *parent = 0);
    ~TorrentModel();
    inline int rowCount(const QModelIndex& index = QModelIndex()) const { Q_UNUSED(index); return m_items.size(); }
    int columnCount(const QModelIndex &parent=QModelIndex()) const { Q_UNUSED(parent); return TorrentModelItem::NB_COLUMNS; }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int torrentRow(const BitTorrent::InfoHash &hash) const;
    QString torrentHash(int row) const;
    TorrentStatusReport getTorrentStatusReport() const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void populate();
    bool inhibitSystem();

signals:
    void torrentAdded(TorrentModelItem *torrentItem);
    void torrentAboutToBeRemoved(TorrentModelItem *torrentItem);
    void torrentChangedLabel(TorrentModelItem *torrentItem, QString previous, QString current);
    void modelRefreshed();

private slots:
    void addTorrent(BitTorrent::TorrentHandle *const torrent);
    void notifyTorrentChanged(int row);
    void handleTorrentLabelChange(QString previous, QString current);
    void handleTorrentAboutToBeRemoved(BitTorrent::TorrentHandle *const torrent);
    void handleTorrentStatusUpdated(BitTorrent::TorrentHandle *const torrent);
    void handleTorrentsUpdated();

private:
    void beginInsertTorrent(int row);
    void endInsertTorrent();
    void beginRemoveTorrent(int row);
    void endRemoveTorrent();

private:
    QList<TorrentModelItem*> m_items;
};

#endif // TORRENTMODEL_H
