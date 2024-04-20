#pragma once

#include <QCache>
#include <QHostAddress>
#include <QObject>

class QHostInfo;

class QString;

namespace Net {
    class ReverseResolution : public QObject {
    Q_OBJECT

        Q_DISABLE_COPY_MOVE(ReverseResolution)

    public:
        explicit ReverseResolution(QObject *parent = nullptr);

        ~ReverseResolution();

        void resolve(const QHostAddress &ip);

    signals:

        void ipResolved(const QHostAddress &ip, const QString &hostname);

    private slots:

        void hostResolved(const QHostInfo &host);

    private:
        QHash<int, QHostAddress> m_lookups;  // <LookupID, IP>
        QCache<QHostAddress, QString> m_cache;  // <IP, HostName>
    };
}
