#pragma once

#include <boost/circular_buffer.hpp>

#include <QObject>
#include <QReadWriteLock>
#include <QString>
#include <QtContainerFwd>

inline const int MAX_LOG_MESSAGES = 20000;

namespace Log {
    enum MsgType {
        ALL = -1,
        NORMAL = 0x1,
        INFO = 0x2,
        WARNING = 0x4,
        CRITICAL = 0x8 // ERROR is defined by libtorrent and results in compiler error
    };
    Q_DECLARE_FLAGS(MsgTypes, MsgType)

    struct Msg {
        int id = -1;
        MsgType type = ALL;
        qint64 timestamp = -1;
        QString message;
        QString trace;
    };

    struct Peer {
        int id = -1;
        bool blocked = false;
        qint64 timestamp = -1;
        QString ip;
        QString reason;
    };
}

Q_DECLARE_OPERATORS_FOR_FLAGS(Log::MsgTypes)

class Logger final : public QObject {
Q_OBJECT

    Q_DISABLE_COPY_MOVE(Logger)

public:
    static void initInstance();

    static void freeInstance();

    static Logger *instance();

    void addMessage(const QString &message, const Log::MsgType &type = Log::NORMAL);

    void addPeer(const QString &ip, bool blocked, const QString &reason = {});

    QVector<Log::Msg> getMessages(int lastKnownId = -1) const;

    QVector<Log::Peer> getPeers(int lastKnownId = -1) const;

signals:

    void newLogMessage(const Log::Msg &message);

    void newLogPeer(const Log::Peer &peer);

private:
    Logger();

    ~Logger() = default;

    static Logger *m_instance;
    boost::circular_buffer_space_optimized<Log::Msg> m_messages;
    boost::circular_buffer_space_optimized<Log::Peer> m_peers;
    mutable QReadWriteLock m_lock;
    int m_msgCounter = 0;
    int m_peerCounter = 0;
};

// Helper function
void LogMsg(const QString &message, const Log::MsgType &type = Log::NORMAL);
