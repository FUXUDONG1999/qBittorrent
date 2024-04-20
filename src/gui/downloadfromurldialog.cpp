#include "downloadfromurldialog.h"

#include <QClipboard>
#include <QKeyEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QRegularExpression>
#include <QSet>
#include <QString>
#include <QStringList>
#include <QStringView>

#include "base/net/downloadmanager.h"
#include "ui_downloadfromurldialog.h"
#include "utils.h"

#define SETTINGS_KEY(name) u"DownloadFromURLDialog/" name

namespace {
    bool isDownloadable(const QString &str) {
        return (Net::DownloadManager::hasSupportedScheme(str)
                || str.startsWith(u"magnet:", Qt::CaseInsensitive)
                #ifdef QBT_USES_LIBTORRENT2
                || ((str.size() == 64) && !str.contains(QRegularExpression(u"[^0-9A-Fa-f]"_s))) // v2 hex-encoded SHA-256 info-hash
                #endif
                || ((str.size() == 40) && !str.contains(QRegularExpression(u"[^0-9A-Fa-f]"_s))) // v1 hex-encoded SHA-1 info-hash
                || ((str.size() == 32) && !str.contains(QRegularExpression(u"[^2-7A-Za-z]"_s)))); // v1 Base32 encoded SHA-1 info-hash
    }
}

DownloadFromURLDialog::DownloadFromURLDialog(QWidget *parent)
        : QDialog(parent), m_ui(new Ui::DownloadFromURLDialog), m_storeDialogSize(SETTINGS_KEY(u"Size"_s)) {
    m_ui->setupUi(this);

    m_ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Download"));
    connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &DownloadFromURLDialog::onSubmit);
    connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    m_ui->textUrls->setWordWrapMode(QTextOption::NoWrap);

    // Paste clipboard if there is an URL in it
    const QString clipboardText = qApp->clipboard()->text();
    const QList<QStringView> clipboardList = QStringView(clipboardText).split(u'\n');

    // show urls in the original order as from the clipboard
    QStringList urls;
    urls.reserve(clipboardList.size());

    for (QStringView url: clipboardList) {
        url = url.trimmed();

        if (url.isEmpty())
            continue;

        if (const QString urlString = url.toString(); isDownloadable(urlString))
            urls << urlString;
    }

    const QString text = urls.join(u'\n')
                         + (!urls.isEmpty() ? u"\n" : u"");

    m_ui->textUrls->setText(text);
    m_ui->textUrls->moveCursor(QTextCursor::End);

    if (const QSize dialogSize = m_storeDialogSize; dialogSize.isValid())
        resize(dialogSize);
}

DownloadFromURLDialog::~DownloadFromURLDialog() {
    m_storeDialogSize = size();
    delete m_ui;
}

void DownloadFromURLDialog::onSubmit() {
    const QString plainText = m_ui->textUrls->toPlainText();
    const QList<QStringView> urls = QStringView(plainText).split(u'\n');

    // process urls in the original order as from the widget
    QSet<QStringView> uniqueURLs;
    QStringList urlList;
    urlList.reserve(urls.size());

    for (QStringView url: urls) {
        url = url.trimmed();

        if (url.isEmpty())
            continue;

        if (!uniqueURLs.contains(url)) {
            uniqueURLs.insert(url);
            urlList << url.toString();
        }
    }

    if (urlList.isEmpty()) {
        QMessageBox::warning(this, tr("No URL entered"), tr("Please type at least one URL."));
        return;
    }

    emit urlsReadyToBeDownloaded(urlList);
    accept();
}

void DownloadFromURLDialog::keyPressEvent(QKeyEvent *event) {
    if ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_Return)) {
        onSubmit();
        return;
    }

    QDialog::keyPressEvent(event);
}
