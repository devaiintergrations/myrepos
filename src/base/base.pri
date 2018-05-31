HEADERS += \
    $$PWD/algorithm.h \
    $$PWD/asyncfilestorage.h \
    $$PWD/bittorrent/addtorrentparams.h  \
    $$PWD/bittorrent/cachestatus.h \
    $$PWD/bittorrent/infohash.h \
    $$PWD/bittorrent/magneturi.h \
    $$PWD/bittorrent/peerinfo.h \
    $$PWD/bittorrent/private/bandwidthscheduler.h \
    $$PWD/bittorrent/private/filterparserthread.h \
    $$PWD/bittorrent/private/resumedataloadingmanager.h \
    $$PWD/bittorrent/private/resumedatasavingmanager.h \
    $$PWD/bittorrent/private/speedmonitor.h \
    $$PWD/bittorrent/private/statistics.h \
    $$PWD/bittorrent/session.h \
    $$PWD/bittorrent/sessionstatus.h \
    $$PWD/bittorrent/torrentcreatorthread.h \
    $$PWD/bittorrent/torrenthandle.h \
    $$PWD/bittorrent/torrentinfo.h \
    $$PWD/bittorrent/tracker.h \
    $$PWD/bittorrent/trackerentry.h \
    $$PWD/exceptions.h \
    $$PWD/filesystemwatcher.h \
    $$PWD/global.h \
    $$PWD/http/connection.h \
    $$PWD/http/httperror.h \
    $$PWD/http/irequesthandler.h \
    $$PWD/http/requestparser.h \
    $$PWD/http/responsebuilder.h \
    $$PWD/http/responsegenerator.h \
    $$PWD/http/server.h \
    $$PWD/http/types.h \
    $$PWD/iconprovider.h \
    $$PWD/indexrange.h \
    $$PWD/logger.h \
    $$PWD/net/dnsupdater.h \
    $$PWD/net/downloadhandler.h \
    $$PWD/net/downloadmanager.h \
    $$PWD/net/geoipmanager.h \
    $$PWD/net/portforwarder.h \
    $$PWD/net/private/geoipdatabase.h \
    $$PWD/net/proxyconfigurationmanager.h \
    $$PWD/net/reverseresolution.h \
    $$PWD/net/smtp.h \
    $$PWD/preferences.h \
    $$PWD/private/profile_p.h \
    $$PWD/profile.h \
    $$PWD/rss/private/rss_parser.h \
    $$PWD/rss/rss_article.h \
    $$PWD/rss/rss_autodownloader.h \
    $$PWD/rss/rss_autodownloadrule.h \
    $$PWD/rss/rss_feed.h \
    $$PWD/rss/rss_folder.h \
    $$PWD/rss/rss_item.h \
    $$PWD/rss/rss_session.h \
    $$PWD/scanfoldersmodel.h \
    $$PWD/search/searchhandler.h \
    $$PWD/search/searchdownloadhandler.h \
    $$PWD/search/searchpluginmanager.h \
    $$PWD/settingsstorage.h \
    $$PWD/settingvalue.h \
    $$PWD/torrentfileguard.h \
    $$PWD/torrentfilter.h \
    $$PWD/tristatebool.h \
    $$PWD/types.h \
    $$PWD/unicodestrings.h \
    $$PWD/utils/bytearray.h \
    $$PWD/utils/foreignapps.h \
    $$PWD/utils/fs.h \
    $$PWD/utils/gzip.h \
    $$PWD/utils/misc.h \
    $$PWD/utils/net.h \
    $$PWD/utils/random.h \
    $$PWD/utils/string.h \
    $$PWD/utils/version.h

SOURCES += \
    $$PWD/asyncfilestorage.cpp \
    $$PWD/bittorrent/infohash.cpp \
    $$PWD/bittorrent/magneturi.cpp \
    $$PWD/bittorrent/peerinfo.cpp \
    $$PWD/bittorrent/private/bandwidthscheduler.cpp \
    $$PWD/bittorrent/private/filterparserthread.cpp \
    $$PWD/bittorrent/private/resumedataloadingmanager.cpp \
    $$PWD/bittorrent/private/resumedatasavingmanager.cpp \
    $$PWD/bittorrent/private/speedmonitor.cpp \
    $$PWD/bittorrent/private/statistics.cpp \
    $$PWD/bittorrent/session.cpp \
    $$PWD/bittorrent/torrentcreatorthread.cpp \
    $$PWD/bittorrent/torrenthandle.cpp \
    $$PWD/bittorrent/torrentinfo.cpp \
    $$PWD/bittorrent/tracker.cpp \
    $$PWD/bittorrent/trackerentry.cpp \
    $$PWD/exceptions.cpp \
    $$PWD/filesystemwatcher.cpp \
    $$PWD/http/connection.cpp \
    $$PWD/http/httperror.cpp \
    $$PWD/http/requestparser.cpp \
    $$PWD/http/responsebuilder.cpp \
    $$PWD/http/responsegenerator.cpp \
    $$PWD/http/server.cpp \
    $$PWD/iconprovider.cpp \
    $$PWD/logger.cpp \
    $$PWD/net/dnsupdater.cpp \
    $$PWD/net/downloadhandler.cpp \
    $$PWD/net/downloadmanager.cpp \
    $$PWD/net/geoipmanager.cpp \
    $$PWD/net/portforwarder.cpp \
    $$PWD/net/private/geoipdatabase.cpp \
    $$PWD/net/proxyconfigurationmanager.cpp \
    $$PWD/net/reverseresolution.cpp \
    $$PWD/net/smtp.cpp \
    $$PWD/preferences.cpp \
    $$PWD/private/profile_p.cpp \
    $$PWD/profile.cpp \
    $$PWD/rss/private/rss_parser.cpp \
    $$PWD/rss/rss_article.cpp \
    $$PWD/rss/rss_autodownloader.cpp \
    $$PWD/rss/rss_autodownloadrule.cpp \
    $$PWD/rss/rss_feed.cpp \
    $$PWD/rss/rss_folder.cpp \
    $$PWD/rss/rss_item.cpp \
    $$PWD/rss/rss_session.cpp \
    $$PWD/scanfoldersmodel.cpp \
    $$PWD/search/searchdownloadhandler.cpp \
    $$PWD/search/searchhandler.cpp \
    $$PWD/search/searchpluginmanager.cpp \
    $$PWD/settingsstorage.cpp \
    $$PWD/torrentfileguard.cpp \
    $$PWD/torrentfilter.cpp \
    $$PWD/tristatebool.cpp \
    $$PWD/utils/bytearray.cpp \
    $$PWD/utils/foreignapps.cpp \
    $$PWD/utils/fs.cpp \
    $$PWD/utils/gzip.cpp \
    $$PWD/utils/misc.cpp \
    $$PWD/utils/net.cpp \
    $$PWD/utils/random.cpp \
    $$PWD/utils/string.cpp
