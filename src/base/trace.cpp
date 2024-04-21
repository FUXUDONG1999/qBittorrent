#include "trace.h"

#ifdef WIN32

#include <windows.h>
#include <dbghelp.h>

std::vector<QString> getFrames() {
    static const int MAX_STACK_FRAMES = 12;
    static char szStackInfo[8192 * MAX_STACK_FRAMES];
    static char szFrameInfo[8192];

    HANDLE process = GetCurrentProcess();
    SymInitialize(process, nullptr, TRUE);

    void *pStack[MAX_STACK_FRAMES];
    const WORD frames = CaptureStackBackTrace(0, MAX_STACK_FRAMES, pStack, nullptr);
    std::vector<QString> result;

    for (WORD i = 0; i < frames; ++i) {
        auto address = (DWORD64) (pStack[i]);

        DWORD64 displacementSym = 0;
        char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
        auto pSymbol = (PSYMBOL_INFO) buffer;
        pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        pSymbol->MaxNameLen = MAX_SYM_NAME;

        DWORD displacementLine = 0;
        IMAGEHLP_LINE64 line;
        line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

        if (SymFromAddr(process, address, &displacementSym, pSymbol) && SymGetLineFromAddr64(process, address, &displacementLine, &line)) {
            std::string str{line.FileName};
            snprintf(szFrameInfo, sizeof(szFrameInfo), "%s:%lu:%s", str.replace(0, str.find("src\\"), "").c_str(), line.LineNumber,
                     pSymbol->Name);
        } else {
            snprintf(szFrameInfo, sizeof(szFrameInfo), "error: %lu", GetLastError());
        }

        result.emplace_back(QString::fromStdString(szFrameInfo));
    }

    return result;
}

#endif

#ifdef __linux__

#include <execinfo.h>
#include <unistd.h>

std::vector<QString> getFrames() {
    void *array[10];
    size_t size;

    size = backtrace(array, 10);
    std::vector<QString> result;

    char **strings = backtrace_symbols(array, size);

    for (int i = 0; i < size; i++) {
        result.emplace_back(QString::fromStdString(strings[i]));
    }

    free(strings);

    return result;
}

#endif

QString getTrace(int deepth) {
    std::vector<QString> frames = getFrames();
    if (frames.empty() || frames.size() < deepth) {
        return {};
    }

    return frames[deepth];
}
