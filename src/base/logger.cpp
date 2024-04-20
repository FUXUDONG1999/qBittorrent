#include "logger.h"

#include <algorithm>

#include <QDateTime>
#include <QVector>

namespace {
    template<typename T>
    QVector<T> loadFromBuffer(const boost::circular_buffer_space_optimized<T> &src, const int offset = 0) {
        QVector<T> ret;
        ret.reserve(static_cast<typename decltype(ret)::size_type>(src.size()) - offset);
        std::copy((src.begin() + offset), src.end(), std::back_inserter(ret));
        return ret;
    }
}

Logger *Logger::m_instance = nullptr;

Logger::Logger()
        : m_messages(MAX_LOG_MESSAGES), m_peers(MAX_LOG_MESSAGES) {
}

Logger *Logger::instance() {
    return m_instance;
}

void Logger::initInstance() {
    if (!m_instance)
        m_instance = new Logger;
}

void Logger::freeInstance() {
    delete m_instance;
    m_instance = nullptr;
}

void Logger::addMessage(const QString &message, const Log::MsgType &type) {
    QWriteLocker locker(&m_lock);
    const Log::Msg msg = {m_msgCounter++, type, QDateTime::currentSecsSinceEpoch(), message};
    m_messages.push_back(msg);
    locker.unlock();

    emit newLogMessage(msg);
}

void Logger::addPeer(const QString &ip, const bool blocked, const QString &reason) {
    QWriteLocker locker(&m_lock);
    const Log::Peer msg = {m_peerCounter++, blocked, QDateTime::currentSecsSinceEpoch(), ip, reason};
    m_peers.push_back(msg);
    locker.unlock();

    emit newLogPeer(msg);
}

QVector<Log::Msg> Logger::getMessages(const int lastKnownId) const {
    const QReadLocker locker(&m_lock);

    const int diff = m_msgCounter - lastKnownId - 1;
    const int size = static_cast<int>(m_messages.size());

    if ((lastKnownId == -1) || (diff >= size))
        return loadFromBuffer(m_messages);

    if (diff <= 0)
        return {};

    return loadFromBuffer(m_messages, (size - diff));
}

QVector<Log::Peer> Logger::getPeers(const int lastKnownId) const {
    const QReadLocker locker(&m_lock);

    const int diff = m_peerCounter - lastKnownId - 1;
    const int size = static_cast<int>(m_peers.size());

    if ((lastKnownId == -1) || (diff >= size))
        return loadFromBuffer(m_peers);

    if (diff <= 0)
        return {};

    return loadFromBuffer(m_peers, (size - diff));
}

void LogMsg(const QString &message, const Log::MsgType &type) {
    Logger::instance()->addMessage(message, type);
}
