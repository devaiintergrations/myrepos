/*
 * Bittorrent Client using Qt and libtorrent.
 * Copyright (C) 2016  Mike Tzou
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

#include <type_traits>
#include <QtGlobal>

const char C_TORRENT_FILE_EXTENSION[] = ".torrent";


#if QT_VERSION < QT_VERSION_CHECK(5, 7, 0)
template <typename T>
constexpr typename std::add_const<T>::type &qAsConst(T &t) noexcept { return t; }

// prevent rvalue arguments:
template <typename T>
void qAsConst(const T &&) = delete;
#endif

// returns a const object copy
template <typename T>
constexpr typename std::add_const<T>::type copyAsConst(T &&t) noexcept { return std::move(t); }

#ifndef QL1S
#define QL1S(x) QLatin1String(x)
#endif

#ifndef QL1C
#define QL1C(x) QLatin1Char(x)
#endif

#ifndef QSL
#define QSL(x) QStringLiteral(x)
#endif

#ifndef QBAL
#define QBAL(x) QByteArrayLiteral(x)
#endif
