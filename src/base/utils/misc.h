#pragma once

#include <QtGlobal>

#ifdef Q_OS_WIN

#include <Windows.h>

#endif

#include <QString>

#include "base/path.h"

enum class ShutdownDialogAction;

/*  Miscellaneous functions that can be useful */

namespace Utils::Misc {
    // use binary prefix standards from IEC 60027-2
    // see http://en.wikipedia.org/wiki/Kilobyte
    enum class SizeUnit {
        Byte,       // 1024^0,
        KibiByte,   // 1024^1,
        MebiByte,   // 1024^2,
        GibiByte,   // 1024^3,
        TebiByte,   // 1024^4,
        PebiByte,   // 1024^5,
        ExbiByte    // 1024^6,
        // int64 is used for sizes and thus the next units can not be handled
        // ZebiByte,   // 1024^7,
        // YobiByte,   // 1024^8
    };

    QString parseHtmlLinks(const QString &rawText);

    void shutdownComputer(const ShutdownDialogAction &action);

    QString osName();

    QString boostVersionString();

    QString libtorrentVersionString();

    QString opensslVersionString();

    QString zlibVersionString();

    QString unitString(SizeUnit unit, bool isSpeed = false);

    // return the best user friendly storage unit (B, KiB, MiB, GiB, TiB)
    // value must be given in bytes
    QString friendlyUnit(qint64 bytes, bool isSpeed = false);

    int friendlyUnitPrecision(SizeUnit unit);

    qint64 sizeInBytes(qreal size, SizeUnit unit);

    bool isPreviewable(const Path &filePath);

    // Take a number of seconds and return a user-friendly
    // time duration like "1d 2h 10m".
    QString userFriendlyDuration(qlonglong seconds, qlonglong maxCap = -1);

    QString getUserIDString();

    QString languageToLocalizedString(const QString &localeStr);

#ifdef Q_OS_WIN

    Path windowsSystemPath();

    template<typename T>
    T loadWinAPI(const QString &source, const char *funcName) {
        const std::wstring path = (windowsSystemPath() / Path(source)).toString().toStdWString();
        return reinterpret_cast<T>(::GetProcAddress(::LoadLibraryW(path.c_str()), funcName));
    }

#endif // Q_OS_WIN
}
