#include "deletionconfirmationdialog.h"

#include <QPushButton>

#include "base/global.h"
#include "base/preferences.h"
#include "uithememanager.h"
#include "utils.h"

DeletionConfirmationDialog::DeletionConfirmationDialog(QWidget *parent, const int size, const QString &name, const bool defaultDeleteFiles)
        : QDialog(parent), m_ui(new Ui::DeletionConfirmationDialog) {
    m_ui->setupUi(this);

    if (size == 1)
        m_ui->label->setText(tr("Are you sure you want to remove '%1' from the transfer list?",
                                "Are you sure you want to remove 'ubuntu-linux-iso' from the transfer list?").arg(name.toHtmlEscaped()));
    else
        m_ui->label->setText(tr("Are you sure you want to remove these %1 torrents from the transfer list?",
                                "Are you sure you want to remove these 5 torrents from the transfer list?").arg(QString::number(size)));

    // Icons
    const QSize iconSize = Utils::Gui::largeIconSize();
    m_ui->labelWarning->setPixmap(UIThemeManager::instance()->getIcon(u"dialog-warning"_s).pixmap(iconSize));
    m_ui->labelWarning->setFixedWidth(iconSize.width());
    m_ui->rememberBtn->setIcon(UIThemeManager::instance()->getIcon(u"object-locked"_s));
    m_ui->rememberBtn->setIconSize(Utils::Gui::mediumIconSize());

    m_ui->checkPermDelete->setChecked(defaultDeleteFiles || Preferences::instance()->deleteTorrentFilesAsDefault());
    connect(m_ui->checkPermDelete, &QCheckBox::clicked, this, &DeletionConfirmationDialog::updateRememberButtonState);
    m_ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Remove"));
    m_ui->buttonBox->button(QDialogButtonBox::Cancel)->setFocus();

    connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

DeletionConfirmationDialog::~DeletionConfirmationDialog() {
    delete m_ui;
}

bool DeletionConfirmationDialog::isDeleteFileSelected() const {
    return m_ui->checkPermDelete->isChecked();
}

void DeletionConfirmationDialog::updateRememberButtonState() {
    m_ui->rememberBtn->setEnabled(m_ui->checkPermDelete->isChecked() != Preferences::instance()->deleteTorrentFilesAsDefault());
}

void DeletionConfirmationDialog::on_rememberBtn_clicked() {
    Preferences::instance()->setDeleteTorrentFilesAsDefault(m_ui->checkPermDelete->isChecked());
    m_ui->rememberBtn->setEnabled(false);
}
