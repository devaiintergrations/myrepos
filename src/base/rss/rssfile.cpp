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

#include "rssfolder.h"
#include "rssfile.h"

using namespace Rss;

File::~File() {}

Folder *File::parentFolder() const
{
    return m_parent;
}

QStringList File::pathHierarchy() const
{
    QStringList path;
    if (m_parent)
        path << m_parent->pathHierarchy();
    path << id();
    return path;
}

void File::rename(const QString &newName)
{
    if (doRename(newName))
        emit nameChanged(newName);
}

void File::becomeParent(File *child, FolderKey)
{
    Q_ASSERT(child);

    if (child->m_parent) {
        child->disconnect(child->m_parent);
        child->m_parent->takeChild(child->id());
    }

    child->m_parent = static_cast<Folder*>(this);

    // Propogate child change signals up
    connect(child, SIGNAL(countChanged()), SIGNAL(countChanged()));
    connect(child, SIGNAL(unreadCountChanged()), SIGNAL(unreadCountChanged()));
}
