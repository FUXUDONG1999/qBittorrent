#include "filelogger.h"

#include <chrono>

#include <QDateTime>
#include <QDir>
#include <QTextStream>
#include <QVector>
#include <iostream>

#include "base/global.h"
#include "base/logger.h"
#include "base/utils/fs.h"

namespace {
    const std::chrono::seconds FLUSH_INTERVAL{2};
}

FileLogger::FileLogger(const Path &path, const bool backup, const int maxSize, const bool deleteOld, const int age, const FileLogAgeType ageType)
        : m_backup(backup), m_maxSize(maxSize) {
    m_flusher.setInterval(FLUSH_INTERVAL);
    m_flusher.setSingleShot(true);
    connect(&m_flusher, &QTimer::timeout, this, &FileLogger::flushLog);

    changePath(path);
    if (deleteOld)
        this->deleteOld(age, ageType);

    const Logger *const logger = Logger::instance();
    for (const Log::Msg &msg: asConst(logger->getMessages()))
        addLogMessage(msg);

    connect(logger, &Logger::newLogMessage, this, &FileLogger::addLogMessage);
}

FileLogger::~FileLogger() {
    closeLogFile();
}

void FileLogger::changePath(const Path &newPath) {
    // compare paths as strings to perform case sensitive comparison on all the platforms
    if (newPath.data() == m_path.parentPath().data())
        return;

    closeLogFile();

    m_path = newPath / Path(u"qbittorrent.log"_s);
    m_logFile.setFileName(m_path.data());

    Utils::Fs::mkpath(newPath);
    openLogFile();
}

void FileLogger::deleteOld(const int age, const FileLogAgeType ageType) {
    const QDateTime date = QDateTime::currentDateTime();
    const QDir dir{m_path.parentPath().data()};
    const QFileInfoList fileList = dir.entryInfoList(QStringList(u"qbittorrent.log.bak*"_s), (QDir::Files | QDir::Writable), (QDir::Time | QDir::Reversed));

    for (const QFileInfo &file: fileList) {
        QDateTime modificationDate = file.lastModified();
        switch (ageType) {
            case DAYS:
                modificationDate = modificationDate.addDays(age);
                break;
            case MONTHS:
                modificationDate = modificationDate.addMonths(age);
                break;
            default:
                modificationDate = modificationDate.addYears(age);
        }
        if (modificationDate > date)
            break;
        Utils::Fs::removeFile(Path(file.absoluteFilePath()));
    }
}

void FileLogger::setBackup(const bool value) {
    m_backup = value;
}

void FileLogger::setMaxSize(const int value) {
    m_maxSize = value;
}

void FileLogger::addLogMessage(const Log::Msg &msg) {
    if (!m_logFile.isOpen()) return;

    QTextStream stream(&m_logFile);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    stream.setCodec("UTF-8");
#endif

    switch (msg.type) {
        case Log::INFO:
            stream << QStringView(u"(I) ");
            break;
        case Log::WARNING:
            stream << QStringView(u"(W) ");
            break;
        case Log::CRITICAL:
            stream << QStringView(u"(C) ");
            break;
        default:
            stream << QStringView(u"(N) ");
    }

    QString timeStamp = QDateTime::fromSecsSinceEpoch(msg.timestamp).toString(Qt::ISODate);

    // 控制台打印信息
    std::cout << timeStamp.toStdString()
              << " - "
              << msg.trace.toStdString()
              << " - "
              << msg.message.toStdString()
              << "\n";
    std::cout.flush();

    stream << timeStamp
           << QStringView(u" - ")
           << msg.trace
           << QStringView(u" - ")
           << msg.message
           << QChar(u'\n');

    if (m_backup && (m_logFile.size() >= m_maxSize)) {
        closeLogFile();
        int counter = 0;
        Path backupLogFilename = m_path + u".bak";

        while (backupLogFilename.exists()) {
            ++counter;
            backupLogFilename = m_path + u".bak" + QString::number(counter);
        }

        Utils::Fs::renameFile(m_path, backupLogFilename);
        openLogFile();
    } else {
        if (!m_flusher.isActive())
            m_flusher.start();
    }
}

void FileLogger::flushLog() {
    if (m_logFile.isOpen())
        m_logFile.flush();
}

void FileLogger::openLogFile() {
    if (!m_logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)
        || !m_logFile.setPermissions(QFile::ReadOwner | QFile::WriteOwner)) {
        m_logFile.close();
        LogMsg(tr("An error occurred while trying to open the log file. Logging to file is disabled."), Log::CRITICAL);
    }
}

void FileLogger::closeLogFile() {
    m_flusher.stop();
    m_logFile.close();
}
