#pragma once

#include <boost/circular_buffer.hpp>

#include <QAbstractListModel>
#include <QColor>
#include <QHash>

#include "base/logger.h"

class BaseLogModel : public QAbstractListModel {
    Q_DISABLE_COPY_MOVE(BaseLogModel)

public:
    enum MessageTypeRole {
        TimeRole = Qt::UserRole,
        MessageRole,
        TimeForegroundRole,
        MessageForegroundRole,
        TypeRole
    };

    explicit BaseLogModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = {}) const override;

    int columnCount(const QModelIndex &parent = {}) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void reset();

protected:
    class Message {
    public:
        Message(const QString &time, const QString &message, const QColor &foreground, Log::MsgType type);

        QVariant time() const;

        QVariant message() const;

        QVariant foreground() const;

        QVariant type() const;

    private:
        QVariant m_time;
        QVariant m_message;
        QVariant m_foreground;
        QVariant m_type;
    };

    void addNewMessage(const Message &message);

private:
    boost::circular_buffer_space_optimized<Message> m_messages;
    const QColor m_timeForeground;
};

class LogMessageModel : public BaseLogModel {
Q_OBJECT

    Q_DISABLE_COPY_MOVE(LogMessageModel)

public:
    explicit LogMessageModel(QObject *parent = nullptr);

private slots:

    void handleNewMessage(const Log::Msg &message);

private:
    const QHash<int, QColor> m_foregroundForMessageTypes;
};

class LogPeerModel : public BaseLogModel {
Q_OBJECT

    Q_DISABLE_COPY_MOVE(LogPeerModel)

public:
    explicit LogPeerModel(QObject *parent = nullptr);

private slots:

    void handleNewMessage(const Log::Peer &peer);

private:
    const QColor m_bannedPeerForeground;
};
