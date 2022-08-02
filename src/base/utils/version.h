/*
 * Bittorrent Client using Qt and libtorrent.
 * Copyright (C) 2022  Mike Tzou (Chocobo1)
 * Copyright (C) 2016  Eugene Shalygin
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

#include <array>
#include <optional>
#include <type_traits>

#include <QByteArray>
#include <QDebug>
#include <QString>

#include "base/exceptions.h"
#include "base/global.h"
#include "base/interfaces/istringable.h"

namespace Utils
{
    template <typename T, std::size_t N, std::size_t Mandatory = N>
    class Version final : public IStringable
    {
        static_assert((N > 0), "The number of version components may not be smaller than 1");
        static_assert((N >= Mandatory),
                      "The number of mandatory components may not be larger than the total number of components");

        using ComponentsArray = std::array<T, N>;

    public:
        using ComponentType = T;
        using ThisType = Version<T, N, Mandatory>;

        constexpr Version() = default;

        template <typename ... Other
                , typename std::enable_if_t<std::conjunction_v<std::is_constructible<T, Other>...>, int> = 0>
        constexpr Version(Other ... components)
            : m_components {{static_cast<T>(components) ...}}
        {
            static_assert((sizeof...(Other) <= N), "Too many parameters provided");
            static_assert((sizeof...(Other) >= Mandatory), "Not enough parameters provided");
        }

        /**
         * @brief Creates version from string in format "x.y.z"
         *
         * @param version Version string in format "x.y.z"
         * @throws RuntimeError if parsing fails
         */
        Version(const QString &version)
            : m_components {parseList(version.split(u'.'))}
        {
        }

        /**
         * @brief Creates version from byte array in format "x.y.z"
         *
         * @param version Version string in format "x.y.z"
         * @throws RuntimeError if parsing fails
         */
        Version(const QByteArray &version)
            : m_components {parseList(version.split('.'))}
        {
        }

        constexpr bool isValid() const
        {
            return m_components.has_value();
        }

        constexpr ComponentType majorNumber() const
        {
            static_assert((N >= 1), "The number of version components is too small");

            if (!isValid())
                throw RuntimeError(u"Object is invalid"_qs);
            return (*m_components)[0];
        }

        constexpr ComponentType minorNumber() const
        {
            static_assert((N >= 2), "The number of version components is too small");

            if (!isValid())
                throw RuntimeError(u"Object is invalid"_qs);
            return (*m_components)[1];
        }

        constexpr ComponentType revisionNumber() const
        {
            static_assert((N >= 3), "The number of version components is too small");

            if (!isValid())
                throw RuntimeError(u"Object is invalid"_qs);
            return (*m_components)[2];
        }

        constexpr ComponentType patchNumber() const
        {
            static_assert((N >= 4), "The number of version components is too small");

            if (!isValid())
                throw RuntimeError(u"Object is invalid"_qs);
            return (*m_components)[3];
        }

        constexpr ComponentType operator[](const std::size_t i) const
        {
            if (!isValid())
                throw RuntimeError(u"Object is invalid"_qs);
            return m_components->at(i);
        }

        QString toString() const override
        {
            if (!isValid())
                throw RuntimeError(u"Object is invalid"_qs);

            const ComponentsArray &components = *m_components;

            // find the last one non-zero component
            std::size_t lastSignificantIndex = N - 1;
            while ((lastSignificantIndex > 0) && (components[lastSignificantIndex] == 0))
                --lastSignificantIndex;

            if ((lastSignificantIndex + 1) < Mandatory)   // lastSignificantIndex >= 0
                lastSignificantIndex = Mandatory - 1;     // and Mandatory >= 1

            QString res = QString::number(components[0]);
            for (std::size_t i = 1; i <= lastSignificantIndex; ++i)
                res += (u'.' + QString::number(components[i]));
            return res;
        }

        // TODO: remove manually defined operators and use compiler generated `operator<=>()` in C++20
        friend bool operator==(const ThisType &left, const ThisType &right)
        {
            return (left.m_components == right.m_components);
        }

        friend bool operator<(const ThisType &left, const ThisType &right)
        {
            return (left.m_components < right.m_components);
        }

        template <typename StringClassWithSplitMethod>
        static Version tryParse(const StringClassWithSplitMethod &s, const Version &defaultVersion)
        {
            try
            {
                return Version(s);
            }
            catch (const RuntimeError &error)
            {
                qDebug() << "Error parsing version:" << error.message();
                return defaultVersion;
            }
        }

    private:
        template <typename StringList>
        static ComponentsArray parseList(const StringList &versionParts)
        {
            if ((static_cast<std::size_t>(versionParts.size()) > N)
                || (static_cast<std::size_t>(versionParts.size()) < Mandatory))
            {
                throw RuntimeError(u"Incorrect number of version components"_qs);
            }

            bool ok = false;
            ComponentsArray res {{}};
            for (std::size_t i = 0; i < static_cast<std::size_t>(versionParts.size()); ++i)
            {
                res[i] = static_cast<T>(versionParts[static_cast<typename StringList::size_type>(i)].toInt(&ok));
                if (!ok)
                    throw RuntimeError(u"Can not parse version component"_qs);
            }
            return res;
        }

        std::optional<ComponentsArray> m_components;
    };

    template <typename T, std::size_t N, std::size_t Mandatory>
    constexpr bool operator!=(const Version<T, N, Mandatory> &left, const Version<T, N, Mandatory> &right)
    {
        return !(left == right);
    }

    template <typename T, std::size_t N, std::size_t Mandatory>
    constexpr bool operator>(const Version<T, N, Mandatory> &left, const Version<T, N, Mandatory> &right)
    {
        return (right < left);
    }

    template <typename T, std::size_t N, std::size_t Mandatory>
    constexpr bool operator<=(const Version<T, N, Mandatory> &left, const Version<T, N, Mandatory> &right)
    {
        return !(left > right);
    }

    template <typename T, std::size_t N, std::size_t Mandatory>
    constexpr bool operator>=(const Version<T, N, Mandatory> &left, const Version<T, N, Mandatory> &right)
    {
        return !(left < right);
    }
}
