/*
 * Bittorrent Client using Qt and libtorrent.
 * Copyright (C) 2020  Vladimir Golovnev <glassez@yandex.ru>
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

#include "filesearcher.h"

#include <QDir>

#include "base/bittorrent/common.h"
#include "base/bittorrent/infohash.h"

void FileSearcher::search(const BitTorrent::TorrentID &id, const QStringList &originalFileNames
                          , const QString &completeSavePath, const QString &incompleteSavePath)
{
    // Returns true if any of the filenames stored at relative paths were found in dirPath,
    // indicating that it is the true save directory. Also return true if all the filenames are
    // absolute, indicating that the save path doesn't matter.
    const auto findInDir = [](const QString &dirPath, QStringList &fileNames) -> bool
    {
        const QDir dir {dirPath};
        bool found = false;
        bool allAbsolute = true;
        for (QString &fileName : fileNames)
        {
            if (QDir::isAbsolutePath(fileName))
            {
                if (QFile::exists(fileName + QB_EXT))
                    fileName += QB_EXT;
            }
            else
            {
                allAbsolute = false;
                if (dir.exists(fileName))
                {
                    found = true;
                }
                else if (dir.exists(fileName + QB_EXT))
                {
                    found = true;
                    fileName += QB_EXT;
                }
            }
        }

        return allAbsolute || found;
    };

    QString savePath = completeSavePath;
    QStringList adjustedFileNames = originalFileNames;
    const bool found = findInDir(savePath, adjustedFileNames);
    if (!found && !incompleteSavePath.isEmpty())
    {
        savePath = incompleteSavePath;
        findInDir(savePath, adjustedFileNames);
    }

    emit searchFinished(id, savePath, adjustedFileNames);
}
