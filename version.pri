PROJECT_NAME = qbittorrent

# Define version numbers here
VER_MAJOR = 3
VER_MINOR = 4
VER_BUGFIX = 0
VER_BUILD = 0
VER_STATUS = alpha # Should be empty for stable releases!

# Don't touch the rest part
PROJECT_VERSION = $${VER_MAJOR}.$${VER_MINOR}.$${VER_BUGFIX}

!equals(VER_BUILD, 0) {
    PROJECT_VERSION = $${PROJECT_VERSION}.$${VER_BUILD}
}

PROJECT_VERSION = $${PROJECT_VERSION}$${VER_STATUS}

DEFINES += VERSION_MAJOR=$${VER_MAJOR}
DEFINES += VERSION_MINOR=$${VER_MINOR}
DEFINES += VERSION_BUGFIX=$${VER_BUGFIX}
DEFINES += VERSION_BUILD=$${VER_BUILD}

win32: contains(QMAKE_TARGET.arch, x86_64) {
# append (64-bit) for Windows 64-bit version
    DEFINES += VERSION=\\\"v$${PROJECT_VERSION}\040(64-bit)\\\"
} else {
    os2 {
        DEFINES += VERSION=\'\"v$${PROJECT_VERSION}\"\'
    } else {
        DEFINES += VERSION=\\\"v$${PROJECT_VERSION}\\\"
    }
}
