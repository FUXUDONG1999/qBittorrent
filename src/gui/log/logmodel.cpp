#include "logmodel.h"

#include <QApplication>
#include <QDateTime>
#include <QColor>

#include "base/global.h"
#include "gui/uithememanager.h"

namespace {
    const int MAX_VISIBLE_MESSAGES = 20000;

    QColor getTimestampColor() {
        return UIThemeManager::instance()->getColor(u"Log.TimeStamp"_s);
    }

    QColor getLogNormalColor() {
        return UIThemeManager::instance()->getColor(u"Log.Normal"_s);
    }

    QColor getLogInfoColor() {
        return UIThemeManager::instance()->getColor(u"Log.Info"_s);
    }

    QColor getLogWarningColor() {
        return UIThemeManager::instance()->getColor(u"Log.Warning"_s);
    }

    QColor getLogCriticalColor() {
        return UIThemeManager::instance()->getColor(u"Log.Critical"_s);
    }

    QColor getPeerBannedColor() {
        return UIThemeManager::instance()->getColor(u"Log.BannedPeer"_s);
    }
}

BaseLogModel::Message::Message(const QString &time, const QString &message, const QColor &foreground, const Log::MsgType type)
        : m_time(time), m_message(message), m_foreground(foreground), m_type(type) {
}

QVariant BaseLogModel::Message::time() const {
    return m_time;
}

QVariant BaseLogModel::Message::message() const {
    return m_message;
}

QVariant BaseLogModel::Message::foreground() const {
    return m_foreground;
}

QVariant BaseLogModel::Message::type() const {
    return m_type;
}

BaseLogModel::BaseLogModel(QObject *parent)
        : QAbstractListModel(parent), m_messages(MAX_VISIBLE_MESSAGES), m_timeForeground(getTimestampColor()) {
}

int BaseLogModel::rowCount(const QModelIndex &) const {
    return static_cast<int>(m_messages.size());
}

int BaseLogModel::columnCount(const QModelIndex &) const {
    return 1;
}

QVariant BaseLogModel::data(const QModelIndex &index, const int role) const {
    if (!index.isValid())
        return {};

    const int messageIndex = index.row();
    if ((messageIndex < 0) || (messageIndex >= static_cast<int>(m_messages.size())))
        return {};

    const Message &message = m_messages[messageIndex];
    switch (role) {
        case TimeRole:
            return message.time();
        case MessageRole:
            return message.message();
        case TimeForegroundRole:
            return m_timeForeground;
        case MessageForegroundRole:
            return message.foreground();
        case TypeRole:
            return message.type();
        default:
            return {};
    }
}

void BaseLogModel::addNewMessage(const BaseLogModel::Message &message) {
    // if row is inserted on filled up buffer, the size will not change
    // but because of calling of beginInsertRows function we'll have ghost rows.
    if (m_messages.size() == MAX_VISIBLE_MESSAGES) {
        const int lastMessage = static_cast<int>(m_messages.size()) - 1;
        beginRemoveRows(QModelIndex(), lastMessage, lastMessage);
        m_messages.pop_back();
        endRemoveRows();
    }

    beginInsertRows(QModelIndex(), 0, 0);
    m_messages.push_front(message);
    endInsertRows();
}

void BaseLogModel::reset() {
    beginResetModel();
    m_messages.clear();
    endResetModel();
}

LogMessageModel::LogMessageModel(QObject *parent)
        : BaseLogModel(parent), m_foregroundForMessageTypes
        {
                {Log::NORMAL,   getLogNormalColor()},
                {Log::INFO,     getLogInfoColor()},
                {Log::WARNING,  getLogWarningColor()},
                {Log::CRITICAL, getLogCriticalColor()}
        } {
    for (const Log::Msg &msg: asConst(Logger::instance()->getMessages()))
        handleNewMessage(msg);
    connect(Logger::instance(), &Logger::newLogMessage, this, &LogMessageModel::handleNewMessage);
}

void LogMessageModel::handleNewMessage(const Log::Msg &message) {
    const QString time = QLocale::system().toString(QDateTime::fromSecsSinceEpoch(message.timestamp), QLocale::ShortFormat);
    const QString messageText = message.message;
    const QColor foreground = m_foregroundForMessageTypes[message.type];

    addNewMessage({time, messageText, foreground, message.type});
}

LogPeerModel::LogPeerModel(QObject *parent)
        : BaseLogModel(parent), m_bannedPeerForeground(getPeerBannedColor()) {
    for (const Log::Peer &peer: asConst(Logger::instance()->getPeers()))
        handleNewMessage(peer);
    connect(Logger::instance(), &Logger::newLogPeer, this, &LogPeerModel::handleNewMessage);
}

void LogPeerModel::handleNewMessage(const Log::Peer &peer) {
    const QString time = QLocale::system().toString(QDateTime::fromSecsSinceEpoch(peer.timestamp), QLocale::ShortFormat);
    const QString message = peer.blocked
                            ? tr("%1 was blocked. Reason: %2.", "0.0.0.0 was blocked. Reason: reason for blocking.").arg(peer.ip, peer.reason)
                            : tr("%1 was banned", "0.0.0.0 was banned").arg(peer.ip);

    addNewMessage({time, message, m_bannedPeerForeground, Log::NORMAL});
}
