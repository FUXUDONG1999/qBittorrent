#pragma once

#include <QDialog>

#include "ui_deletionconfirmationdialog.h"

namespace Ui {
    class DeletionConfirmationDialog;
}

class DeletionConfirmationDialog : public QDialog {
Q_OBJECT

    Q_DISABLE_COPY_MOVE(DeletionConfirmationDialog)

public:
    DeletionConfirmationDialog(QWidget *parent, int size, const QString &name, bool defaultDeleteFiles);

    ~DeletionConfirmationDialog();

    bool isDeleteFileSelected() const;

private slots:

    void updateRememberButtonState();

    void on_rememberBtn_clicked();

private:
    Ui::DeletionConfirmationDialog *m_ui = nullptr;
};
