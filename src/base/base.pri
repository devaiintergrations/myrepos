HEADERS += \
    $$PWD/algorithm.h \
    $$PWD/asyncfilestorage.h \
    $$PWD/bittorrent/abstractfilestorage.h \
    $$PWD/bittorrent/addtorrentparams.h \
    $$PWD/bittorrent/bencoderesumedatastorage.h \
    $$PWD/bittorrent/cachestatus.h \
    $$PWD/bittorrent/common.h \
    $$PWD/bittorrent/customstorage.h \
    $$PWD/bittorrent/downloadpriority.h \
    $$PWD/bittorrent/dbresumedatastorage.h \
    $$PWD/bittorrent/filesearcher.h \
    $$PWD/bittorrent/filterparserthread.h \
    $$PWD/bittorrent/infohash.h \
    $$PWD/bittorrent/loadtorrentparams.h \
    $$PWD/bittorrent/ltqhash.h \
    $$PWD/bittorrent/ltunderlyingtype.h \
    $$PWD/bittorrent/magneturi.h \
    $$PWD/bittorrent/nativesessionextension.h \
    $$PWD/bittorrent/nativetorrentextension.h \
    $$PWD/bittorrent/peeraddress.h \
    $$PWD/bittorrent/peerinfo.h \
    $$PWD/bittorrent/portforwarderimpl.h \
    $$PWD/bittorrent/resumedatastorage.h \
    $$PWD/bittorrent/scheduler/bandwidthscheduler.h \
    $$PWD/bittorrent/scheduler/scheduleday.h \
    $$PWD/bittorrent/scheduler/scheduleentry.h \
    $$PWD/bittorrent/session.h \
    $$PWD/bittorrent/sessionstatus.h \
    $$PWD/bittorrent/speedmonitor.h \
    $$PWD/bittorrent/statistics.h \
    $$PWD/bittorrent/torrent.h \
    $$PWD/bittorrent/torrentcontentlayout.h \
    $$PWD/bittorrent/torrentcreatorthread.h \
    $$PWD/bittorrent/torrentimpl.h \
    $$PWD/bittorrent/torrentinfo.h \
    $$PWD/bittorrent/tracker.h \
    $$PWD/bittorrent/trackerentry.h \
    $$PWD/digest32.h \
    $$PWD/exceptions.h \
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
    $$PWD/net/downloadhandlerimpl.h \
    $$PWD/net/downloadmanager.h \
    $$PWD/net/geoipdatabase.h \
    $$PWD/net/geoipmanager.h \
    $$PWD/net/portforwarder.h \
    $$PWD/net/proxyconfigurationmanager.h \
    $$PWD/net/reverseresolution.h \
    $$PWD/net/smtp.h \
    $$PWD/orderedset.h \
    $$PWD/preferences.h \
    $$PWD/profile.h \
    $$PWD/profile_p.h \
    $$PWD/rss/rss_article.h \
    $$PWD/rss/rss_autodownloader.h \
    $$PWD/rss/rss_autodownloadrule.h \
    $$PWD/rss/rss_feed.h \
    $$PWD/rss/rss_folder.h \
    $$PWD/rss/rss_item.h \
    $$PWD/rss/rss_parser.h \
    $$PWD/rss/rss_session.h \
    $$PWD/search/searchdownloadhandler.h \
    $$PWD/search/searchhandler.h \
    $$PWD/search/searchpluginmanager.h \
    $$PWD/settingsstorage.h \
    $$PWD/settingvalue.h \
    $$PWD/tagset.h \
    $$PWD/torrentfileguard.h \
    $$PWD/torrentfileswatcher.h \
    $$PWD/torrentfilter.h \
    $$PWD/types.h \
    $$PWD/unicodestrings.h \
    $$PWD/utils/bytearray.h \
    $$PWD/utils/compare.h \
    $$PWD/utils/foreignapps.h \
    $$PWD/utils/fs.h \
    $$PWD/utils/gzip.h \
    $$PWD/utils/io.h \
    $$PWD/utils/misc.h \
    $$PWD/utils/net.h \
    $$PWD/utils/password.h \
    $$PWD/utils/random.h \
    $$PWD/utils/string.h \
    $$PWD/utils/version.h \
    $$PWD/version.h

SOURCES += \
    $$PWD/asyncfilestorage.cpp \
    $$PWD/bittorrent/abstractfilestorage.cpp \
    $$PWD/bittorrent/bencoderesumedatastorage.cpp \
    $$PWD/bittorrent/customstorage.cpp \
    $$PWD/bittorrent/dbresumedatastorage.cpp \
    $$PWD/bittorrent/downloadpriority.cpp \
    $$PWD/bittorrent/filesearcher.cpp \
    $$PWD/bittorrent/filterparserthread.cpp \
    $$PWD/bittorrent/infohash.cpp \
    $$PWD/bittorrent/magneturi.cpp \
    $$PWD/bittorrent/nativesessionextension.cpp \
    $$PWD/bittorrent/nativetorrentextension.cpp \
    $$PWD/bittorrent/peeraddress.cpp \
    $$PWD/bittorrent/peerinfo.cpp \
    $$PWD/bittorrent/portforwarderimpl.cpp \
    $$PWD/bittorrent/scheduler/bandwidthscheduler.cpp \
    $$PWD/bittorrent/scheduler/scheduleday.cpp \
    $$PWD/bittorrent/scheduler/scheduleentry.cpp \
    $$PWD/bittorrent/session.cpp \
    $$PWD/bittorrent/speedmonitor.cpp \
    $$PWD/bittorrent/statistics.cpp \
    $$PWD/bittorrent/torrent.cpp \
    $$PWD/bittorrent/torrentcreatorthread.cpp \
    $$PWD/bittorrent/torrentimpl.cpp \
    $$PWD/bittorrent/torrentinfo.cpp \
    $$PWD/bittorrent/tracker.cpp \
    $$PWD/bittorrent/trackerentry.cpp \
    $$PWD/exceptions.cpp \
    $$PWD/http/connection.cpp \
    $$PWD/http/httperror.cpp \
    $$PWD/http/requestparser.cpp \
    $$PWD/http/responsebuilder.cpp \
    $$PWD/http/responsegenerator.cpp \
    $$PWD/http/server.cpp \
    $$PWD/iconprovider.cpp \
    $$PWD/logger.cpp \
    $$PWD/net/dnsupdater.cpp \
    $$PWD/net/downloadhandlerimpl.cpp \
    $$PWD/net/downloadmanager.cpp \
    $$PWD/net/geoipdatabase.cpp \
    $$PWD/net/geoipmanager.cpp \
    $$PWD/net/portforwarder.cpp \
    $$PWD/net/proxyconfigurationmanager.cpp \
    $$PWD/net/reverseresolution.cpp \
    $$PWD/net/smtp.cpp \
    $$PWD/preferences.cpp \
    $$PWD/profile.cpp \
    $$PWD/profile_p.cpp \
    $$PWD/rss/rss_article.cpp \
    $$PWD/rss/rss_autodownloader.cpp \
    $$PWD/rss/rss_autodownloadrule.cpp \
    $$PWD/rss/rss_feed.cpp \
    $$PWD/rss/rss_folder.cpp \
    $$PWD/rss/rss_item.cpp \
    $$PWD/rss/rss_parser.cpp \
    $$PWD/rss/rss_session.cpp \
    $$PWD/search/searchdownloadhandler.cpp \
    $$PWD/search/searchhandler.cpp \
    $$PWD/search/searchpluginmanager.cpp \
    $$PWD/settingsstorage.cpp \
    $$PWD/tagset.cpp \
    $$PWD/torrentfileguard.cpp \
    $$PWD/torrentfileswatcher.cpp \
    $$PWD/torrentfilter.cpp \
    $$PWD/utils/bytearray.cpp \
    $$PWD/utils/compare.cpp \
    $$PWD/utils/foreignapps.cpp \
    $$PWD/utils/fs.cpp \
    $$PWD/utils/gzip.cpp \
    $$PWD/utils/io.cpp \
    $$PWD/utils/misc.cpp \
    $$PWD/utils/net.cpp \
    $$PWD/utils/password.cpp \
    $$PWD/utils/random.cpp \
    $$PWD/utils/string.cpp
