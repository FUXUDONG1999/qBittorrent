#pragma once

#include <libtorrent/ip_filter.hpp>

#include <QThread>

#include "base/path.h"

class QDataStream;

class FilterParserThread final : public QThread {
Q_OBJECT

    Q_DISABLE_COPY_MOVE(FilterParserThread)

public:
    FilterParserThread(QObject *parent = nullptr);

    ~FilterParserThread();

    void processFilterFile(const Path &filePath);

    lt::ip_filter IPfilter();

signals:

    void IPFilterParsed(int ruleCount);

    void IPFilterError();

protected:
    void run() override;

private:
    int findAndNullDelimiter(char *data, char delimiter, int start, int end, bool reverse = false);

    int trim(char *data, int start, int end);

    int parseDATFilterFile();

    int parseP2PFilterFile();

    int getlineInStream(QDataStream &stream, std::string &name, char delim);

    int parseP2BFilterFile();

    bool m_abort = false;
    Path m_filePath;
    lt::ip_filter m_filter;
};
