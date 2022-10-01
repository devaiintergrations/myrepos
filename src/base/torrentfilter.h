/*
 * Bittorrent Client using Qt and libtorrent.
 * Copyright (C) 2014  Vladimir Golovnev <glassez@yandex.ru>
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

#pragma once

#include <optional>

#include <QSet>
#include <QString>

#include "base/bittorrent/infohash.h"

namespace BitTorrent
{
    class Torrent;
}

using TorrentIDSet = QSet<BitTorrent::TorrentID>;

class TorrentFilter
{
public:
    enum Type
    {
        All,
        Downloading,
        Seeding,
        Completed,
        Resumed,
        Paused,
        Active,
        Inactive,
        Stalled,
        StalledUploading,
        StalledDownloading,
        Checking,
        Errored
    };

    // These mean any permutation, including no category / tag.
    static const std::optional<QString> AnyCategory;
    static const std::optional<TorrentIDSet> AnyID;
    static const std::optional<QString> AnyTag;
    static const std::optional<QString> AnyInfoHash;
    static const std::optional<QString> AnySavePath;
    static const std::optional<QString> AnyDownloadPath;
    static const std::optional<QString> AnyComment;
    static const std::optional<QString> AnyCreator;
    static const std::optional<QString> AnyFilename;

    static const TorrentFilter DownloadingTorrent;
    static const TorrentFilter SeedingTorrent;
    static const TorrentFilter CompletedTorrent;
    static const TorrentFilter PausedTorrent;
    static const TorrentFilter ResumedTorrent;
    static const TorrentFilter ActiveTorrent;
    static const TorrentFilter InactiveTorrent;
    static const TorrentFilter StalledTorrent;
    static const TorrentFilter StalledUploadingTorrent;
    static const TorrentFilter StalledDownloadingTorrent;
    static const TorrentFilter CheckingTorrent;
    static const TorrentFilter ErroredTorrent;

    TorrentFilter() = default;
    // category & tags: pass empty string for uncategorized / untagged torrents.
    TorrentFilter(Type type, const std::optional<TorrentIDSet> &idSet = AnyID
            , const std::optional<QString> &category = AnyCategory, const std::optional<QString> &tag = AnyTag);
    TorrentFilter(const QString &filter, const std::optional<TorrentIDSet> &idSet = AnyID
            , const std::optional<QString> &category = AnyCategory, const std::optional<QString> &tags = AnyTag);

    bool setType(Type type);
    bool setTypeByName(const QString &filter);
    bool setTorrentIDSet(const std::optional<TorrentIDSet> &idSet);
    bool setCategory(const std::optional<QString> &category);
    bool setTag(const std::optional<QString> &tag);
    bool setInfoHash(const std::optional<QString> &infohash);
    bool setSavePath(const std::optional<QString> &save_path);
    bool setDownloadPath(const std::optional<QString> &download_path);
    bool setComment(const std::optional<QString> &comment);
    bool setCreator(const std::optional<QString> &creator);
    bool setFilename(const std::optional<QString> &filename);

    bool match(const BitTorrent::Torrent *torrent) const;

private:
    bool matchState(const BitTorrent::Torrent *torrent) const;
    bool matchInfoHash(const BitTorrent::Torrent *torrent) const;
    bool matchSavePath(const BitTorrent::Torrent *torrent) const;
    bool matchDownloadPath(const BitTorrent::Torrent *torrent) const;
    bool matchComment(const BitTorrent::Torrent *torrent) const;
    bool matchCreator(const BitTorrent::Torrent *torrent) const;
    bool matchFilename(const BitTorrent::Torrent *torrent) const;
    bool matchHash(const BitTorrent::Torrent *torrent) const;
    bool matchCategory(const BitTorrent::Torrent *torrent) const;
    bool matchTag(const BitTorrent::Torrent *torrent) const;

    Type m_type {All};
    std::optional<QString> m_category;
    std::optional<QString> m_tag;
    std::optional<TorrentIDSet> m_idSet;
    std::optional<QString> m_infoHash;
    std::optional<QString> m_savePath;
    std::optional<QString> m_downloadPath;
    std::optional<QString> m_comment;
    std::optional<QString> m_creator;
    std::optional<QString> m_filename;
};
