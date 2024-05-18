cmake_minimum_required(VERSION 3.16 FATAL_ERROR) # Policies <= CMP0097 default to NEW

project(qBittorrent
        DESCRIPTION "The qBittorrent BitTorrent client"
        HOMEPAGE_URL "https://www.qbittorrent.org/"
        LANGUAGES CXX
)

# 开发调试库
include_directories("D://program//visualstudio2022//ide//VC//vcpkg//buildtrees//libtorrent//src//v2.0.9-b2c835423e.clean//src")

# version requirements - older versions may work, but you are on your own
set(minBoostVersion 1.71)
set(minQt5Version 5.15.2)
set(minQt6Version 6.7)
set(minOpenSSLVersion 1.1.1)
set(minLibtorrent1Version 1.2.19)
set(minLibtorrentVersion 2.0.9)
set(minZlibVersion 1.2.11)
set(QT6 ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include(CheckCXXSourceCompiles)
include(GNUInstallDirs)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
include(FeatureSummary)
include(FeatureOptionsSetup)

# features, list is loosely sorted by user's interests
feature_option(GUI "Build GUI application" ON)
feature_option(WEBUI "Enable built-in HTTP server for remote control" ON)
feature_option(QT6 "Build with Qt 6 instead of Qt 5" OFF)
feature_option(STACKTRACE "Enable stacktrace support" ON)
feature_option(TESTING "Build internal testing suite" OFF)
feature_option(VERBOSE_CONFIGURE "Show information about PACKAGES_FOUND and PACKAGES_NOT_FOUND in the configure output (only useful for debugging the CMake build scripts)" OFF)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    feature_option_dependent(DBUS
            "Enable support for notifications and power-management features via D-Bus"
            ON "GUI" OFF
    )
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    feature_option_dependent(SYSTEMD
            "Install systemd service file. Target directory is overridable with `SYSTEMD_SERVICES_INSTALL_DIR` variable"
            OFF "NOT GUI" OFF
    )
endif ()

if (MSVC)
    feature_option(MSVC_RUNTIME_DYNAMIC "Use MSVC dynamic runtime library (-MD) instead of static (-MT)" ON)
endif ()

if (VERBOSE_CONFIGURE)
    feature_summary(WHAT ALL)
else ()
    feature_summary(WHAT ENABLED_FEATURES DISABLED_FEATURES)
endif ()

include(CheckPackages)
include(CommonConfig)

configure_file("src/base/version.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/src/base/version.h" @ONLY)

add_subdirectory(src)
add_subdirectory(dist)

if (TESTING)
    add_subdirectory(test)
endif ()
