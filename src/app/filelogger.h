#pragma once

#include <QFile>
#include <QObject>
#include <QTimer>

#include "base/path.h"

namespace Log {
    struct Msg;
}

class FileLogger : public QObject {
Q_OBJECT

    Q_DISABLE_COPY_MOVE(FileLogger)

public:
    enum FileLogAgeType {
        DAYS,
        MONTHS,
        YEARS
    };

    FileLogger(const Path &path, bool backup, int maxSize, bool deleteOld, int age, FileLogAgeType ageType);

    ~FileLogger();

    void changePath(const Path &newPath);

    void deleteOld(int age, FileLogAgeType ageType);

    void setBackup(bool value);

    void setMaxSize(int value);

private slots:

    void addLogMessage(const Log::Msg &msg);

    void flushLog();

private:
    void openLogFile();

    void closeLogFile();

    Path m_path;
    bool m_backup;
    int m_maxSize;
    QFile m_logFile;
    QTimer m_flusher;
};
