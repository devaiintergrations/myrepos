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
 * Contact: chris@qbittorrent.org, arnaud@qbittorrent.org
 */

#include "base/rss/rssmanager.h"
#include "base/rss/rssfolder.h"
#include "base/rss/rssfeed.h"
#include "base/utils/string.h"
#include "gui/sortablewidgetitems.h"
#include "guiiconprovider.h"
#include "feedlistwidget.h"

class FeedListWidget::FeedListWidgetItem: public SortableTreeWidgetItem
{
public:
    FeedListWidgetItem(FeedListWidget *parent);
    ~FeedListWidgetItem();

    bool operator<(const QTreeWidgetItem &other) const;

private:
    FeedListWidget *m_parent;
};

FeedListWidget::FeedListWidgetItem::FeedListWidgetItem(FeedListWidget *parent)
    : SortableTreeWidgetItem(static_cast<FeedListWidget *>(0))
    , m_parent(parent)
{
}

FeedListWidget::FeedListWidgetItem::~FeedListWidgetItem()
{
}

bool FeedListWidget::FeedListWidgetItem::operator<(const QTreeWidgetItem &other) const
{
    // If either is the sticky unread it sorts first
    if (this == m_parent->m_unreadStickyItem)
        return true;
    else if (&other == m_parent->m_unreadStickyItem)
        return false;

    bool isFolder = m_parent->isFolder(this);
    bool otherIsFolder = m_parent->isFolder(&other);

    // Folders sort first
    if (isFolder != otherIsFolder)
        return isFolder;

    return SortableTreeWidgetItem::operator<(other);
}

FeedListWidget::FeedListWidget(QWidget *parent, const Rss::ManagerPtr &rssmanager)
    : QTreeWidget(parent)
    , m_rssManager(rssmanager)
    , m_currentFeed(nullptr)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setDragDropMode(QAbstractItemView::InternalMove);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setColumnCount(1);
    setSortingEnabled(true);
    sortByColumn(0, Qt::AscendingOrder);
    headerItem()->setText(0, tr("RSS feeds"));
    m_unreadStickyItem = new FeedListWidgetItem(this);
    m_unreadStickyItem->setText(0, tr("Unread") + QString::fromUtf8("  (") + QString::number(rssmanager->rootFolder()->unreadCount()) + QString(")"));
    m_unreadStickyItem->setData(0, Qt::DecorationRole, GuiIconProvider::instance()->getIcon("mail-folder-inbox"));
    addTopLevelItem(m_unreadStickyItem);
    itemAdded(m_unreadStickyItem, rssmanager->rootFolder());
    connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem *,QTreeWidgetItem *)), SLOT(updateCurrentFeed(QTreeWidgetItem *)));
    setCurrentItem(m_unreadStickyItem);
}

FeedListWidget::~FeedListWidget()
{
    delete m_unreadStickyItem;
}

QTreeWidgetItem *FeedListWidget::createListItem(const Rss::FilePtr &rssFile)
{
    Q_ASSERT(rssFile);
    QTreeWidgetItem *item = new FeedListWidget::FeedListWidgetItem(this);
    item->setData(0, Qt::DisplayRole, QVariant(rssFile->displayName() + QString::fromUtf8("  (") + QString::number(rssFile->unreadCount()) + QString(")")));
    item->setData(0, Qt::DecorationRole, QIcon(rssFile->iconPath()));
    return item;
}

void FeedListWidget::itemAdded(QTreeWidgetItem *item, const Rss::FilePtr &file)
{
    m_rssMapping[item] = file;
    if (Rss::FeedPtr feed = qSharedPointerDynamicCast<Rss::Feed>(file))
        m_feedsItems[feed->id()] = item;
}

void FeedListWidget::itemAboutToBeRemoved(QTreeWidgetItem *item)
{
    Rss::FilePtr file = m_rssMapping.take(item);
    if (Rss::FeedPtr feed = qSharedPointerDynamicCast<Rss::Feed>(file)) {
        m_feedsItems.remove(feed->id());
    }
    else if (Rss::FolderPtr folder = qSharedPointerDynamicCast<Rss::Folder>(file)) {
        Rss::FeedList feeds = folder->getAllFeeds();
        foreach (const Rss::FeedPtr &feed, feeds)
            m_feedsItems.remove(feed->id());
    }
}

bool FeedListWidget::hasFeed(const QString &url) const
{
    return m_feedsItems.contains(QUrl(url).toString());
}

QList<QTreeWidgetItem *> FeedListWidget::getAllFeedItems() const
{
    return m_feedsItems.values();
}

QTreeWidgetItem *FeedListWidget::stickyUnreadItem() const
{
    return m_unreadStickyItem;
}

QStringList FeedListWidget::getItemPath(const QTreeWidgetItem *item) const
{
    QStringList path;
    if (item) {
        if (item->parent())
            path << getItemPath(item->parent());
        path.append(getRSSItem(item)->id());
    }
    return path;
}

QList<QTreeWidgetItem *> FeedListWidget::getAllOpenFolders(QTreeWidgetItem *parent) const
{
    QList<QTreeWidgetItem *> open_folders;
    int nbChildren;
    if (parent)
        nbChildren = parent->childCount();
    else
        nbChildren = topLevelItemCount();
    for (int i = 0; i<nbChildren; ++i) {
        QTreeWidgetItem *item;
        if (parent)
            item = parent->child(i);
        else
            item = topLevelItem(i);
        if (isFolder(item) && item->isExpanded()) {
            QList<QTreeWidgetItem *> open_subfolders = getAllOpenFolders(item);
            if (!open_subfolders.empty())
                open_folders << open_subfolders;
            else
                open_folders << item;
        }
    }
    return open_folders;
}

QList<QTreeWidgetItem *> FeedListWidget::getAllFeedItems(QTreeWidgetItem *folder)
{
    QList<QTreeWidgetItem *> feeds;
    const int nbChildren = folder->childCount();
    for (int i = 0; i<nbChildren; ++i) {
        QTreeWidgetItem *item = folder->child(i);
        if (isFeed(item))
            feeds << item;
        else
            feeds << getAllFeedItems(item);
    }
    return feeds;
}

Rss::FilePtr FeedListWidget::getRSSItem(const QTreeWidgetItem *item) const
{
    return m_rssMapping.value(item, Rss::FilePtr());
}

bool FeedListWidget::isFeed(const QTreeWidgetItem *item) const
{
    return (qSharedPointerDynamicCast<Rss::Feed>(m_rssMapping.value(item)) != NULL);
}

bool FeedListWidget::isFolder(const QTreeWidgetItem *item) const
{
    return (qSharedPointerDynamicCast<Rss::Folder>(m_rssMapping.value(item)) != NULL);
}

QString FeedListWidget::getItemID(const QTreeWidgetItem *item) const
{
    return m_rssMapping.value(item)->id();
}

QTreeWidgetItem *FeedListWidget::getTreeItemFromUrl(const QString &url) const
{
    return m_feedsItems.value(url, 0);
}

Rss::FeedPtr FeedListWidget::getRSSItemFromUrl(const QString &url) const
{
    return qSharedPointerDynamicCast<Rss::Feed>(getRSSItem(getTreeItemFromUrl(url)));
}

QTreeWidgetItem *FeedListWidget::currentItem() const
{
    return m_currentFeed;
}

QTreeWidgetItem *FeedListWidget::currentFeed() const
{
    return m_currentFeed;
}

void FeedListWidget::updateCurrentFeed(QTreeWidgetItem *new_item)
{
    if (!new_item) return;
    if (!m_rssMapping.contains(new_item)) return;
    if (isFeed(new_item) || ( new_item == m_unreadStickyItem) )
        m_currentFeed = new_item;
}

void FeedListWidget::dragMoveEvent(QDragMoveEvent *event)
{
    QTreeWidget::dragMoveEvent(event);

    QTreeWidgetItem *item = itemAt(event->pos());
    // Prohibit dropping onto global unread counter
    if (item == m_unreadStickyItem) {
        event->ignore();
        return;
    }
    // Prohibit dragging of global unread counter
    if (selectedItems().contains(m_unreadStickyItem)) {
        event->ignore();
        return;
    }
    // Prohibit dropping onto feeds
    if (item && isFeed(item)) {
        event->ignore();
        return;
    }
}

void FeedListWidget::dropEvent(QDropEvent *event)
{
    qDebug("dropEvent");
    QList<QTreeWidgetItem *> folders_altered;
    QTreeWidgetItem *dest_folder_item = itemAt(event->pos());
    Rss::FolderPtr dest_folder;
    if (dest_folder_item) {
        dest_folder = qSharedPointerCast<Rss::Folder>(getRSSItem(dest_folder_item));
        folders_altered << dest_folder_item;
    }
    else {
        dest_folder = m_rssManager->rootFolder();
    }
    QList<QTreeWidgetItem *> src_items = selectedItems();
    // Check if there is not going to overwrite another file
    foreach (QTreeWidgetItem *src_item, src_items) {
        Rss::FilePtr file = getRSSItem(src_item);
        if (dest_folder->hasChild(file->id())) {
            QTreeWidget::dropEvent(event);
            return;
        }
    }
    // Proceed with the move
    foreach (QTreeWidgetItem *src_item, src_items) {
        QTreeWidgetItem *parent_folder = src_item->parent();
        if (parent_folder && !folders_altered.contains(parent_folder))
            folders_altered << parent_folder;
        // Actually move the file
        Rss::FilePtr file = getRSSItem(src_item);
        m_rssManager->moveFile(file, dest_folder);
    }
    QTreeWidget::dropEvent(event);
    if (dest_folder_item)
        dest_folder_item->setExpanded(true);
    // Emit signal for update
    if (!folders_altered.empty())
        emit foldersAltered(folders_altered);
}
