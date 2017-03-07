/*
 * Bittorrent Client using Qt and libtorrent.
 * Copyright (C) 2010  Christophe Dumez <chris@qbittorrent.org>
 * Copyright (C) 2010  Arnaud Demaiziere <arnaud@qbittorrent.org>
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
 * Contact: chris@qbittorrent.org, arnaud@qbittorrent.org
 */

#ifndef RSSFILE_H
#define RSSFILE_H

#include <QList>
#include <QStringList>
#include <QSharedPointer>

namespace Rss
{
    class Folder;
    class File;
    class Article;

    typedef QSharedPointer<File> FilePtr;
    typedef QSharedPointer<Article> ArticlePtr;
    typedef QList<ArticlePtr> ArticleList;
    typedef QList<FilePtr> FileList;

    /**
     * Parent interface for Rss::Folder and Rss::Feed.
     */
    class File : public QObject
    {
        Q_OBJECT

    protected:
        // Passkey idiom. Only allow Folder class access to becomeParent below
        class FolderKey
        {
            friend class Folder;
            FolderKey() {}
            FolderKey(const FolderKey&) = delete;
            FolderKey& operator =(const FolderKey&) = delete;
        };

    public:
        virtual ~File();

        virtual QString id() const = 0;
        virtual QString displayName() const = 0;
        virtual uint count() const = 0;
        virtual uint unreadCount() const = 0;
        virtual QString iconPath() const = 0;
        virtual ArticleList articleListByDateDesc() const = 0;
        virtual ArticleList unreadArticleListByDateDesc() const = 0;

        Folder *parentFolder() const;
        QStringList pathHierarchy() const;
        void rename(const QString &newName);

    signals:
        void nameChanged(const QString&);
        void unreadCountChanged(int = -1);
        void countChanged(int = -1);

    public slots:
        virtual void markAsRead() = 0;
        virtual bool refresh() = 0;
        virtual void removeAllSettings() = 0;
        virtual void saveItemsToDisk() = 0;
        virtual void recheckRssItemsForDownload() = 0;


    protected:
        virtual bool doRename(const QString &newName) = 0;
        void becomeParent(File *child, FolderKey);

    private:
        Folder *m_parent = nullptr;
    };
}

#endif // RSSFILE_H
