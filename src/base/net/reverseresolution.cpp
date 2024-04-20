#include "reverseresolution.h"

#include <QHostInfo>
#include <QString>

const int CACHE_SIZE = 2048;

using namespace Net;

namespace {
    bool isUsefulHostName(const QString &hostname, const QHostAddress &ip) {
        return (!hostname.isEmpty() && (hostname != ip.toString()));
    }
}

ReverseResolution::ReverseResolution(QObject *parent)
        : QObject(parent) {
    m_cache.setMaxCost(CACHE_SIZE);
}

ReverseResolution::~ReverseResolution() {
    // abort on-going lookups instead of waiting them
    for (auto iter = m_lookups.cbegin(); iter != m_lookups.cend(); ++iter)
        QHostInfo::abortHostLookup(iter.key());
}

void ReverseResolution::resolve(const QHostAddress &ip) {
    const QString *hostname = m_cache.object(ip);
    if (hostname) {
        emit ipResolved(ip, *hostname);
        return;
    }

    // do reverse lookup: IP -> hostname
    const int lookupId = QHostInfo::lookupHost(ip.toString(), this, &ReverseResolution::hostResolved);
    m_lookups.insert(lookupId, ip);
}

void ReverseResolution::hostResolved(const QHostInfo &host) {
    const QHostAddress ip = m_lookups.take(host.lookupId());

    if (host.error() != QHostInfo::NoError) {
        emit ipResolved(ip, {});
        return;
    }

    const QString hostname = isUsefulHostName(host.hostName(), ip)
                             ? host.hostName()
                             : QString();
    m_cache.insert(ip, new QString(hostname));
    emit ipResolved(ip, hostname);
}
