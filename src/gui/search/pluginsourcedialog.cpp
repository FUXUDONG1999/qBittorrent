#include "pluginsourcedialog.h"

#include "gui/utils.h"
#include "ui_pluginsourcedialog.h"

#define SETTINGS_KEY(name) u"SearchPluginSourceDialog/" name

PluginSourceDialog::PluginSourceDialog(QWidget *parent)
        : QDialog(parent), m_ui(new Ui::PluginSourceDialog), m_storeDialogSize(SETTINGS_KEY(u"Size"_s)) {
    m_ui->setupUi(this);

    if (const QSize dialogSize = m_storeDialogSize; dialogSize.isValid())
        resize(dialogSize);
}

PluginSourceDialog::~PluginSourceDialog() {
    m_storeDialogSize = size();
    delete m_ui;
}

void PluginSourceDialog::on_localButton_clicked() {
    emit askForLocalFile();
    close();
}

void PluginSourceDialog::on_urlButton_clicked() {
    emit askForUrl();
    close();
}
