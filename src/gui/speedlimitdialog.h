#pragma once

#include <QDialog>

#include "base/settingvalue.h"

namespace Ui {
    class SpeedLimitDialog;
}

class SpeedLimitDialog final : public QDialog {
Q_OBJECT

    Q_DISABLE_COPY_MOVE(SpeedLimitDialog)

public:
    explicit SpeedLimitDialog(QWidget *parent);

    ~SpeedLimitDialog() override;

public slots:

    void accept() override;

private:
    Ui::SpeedLimitDialog *m_ui = nullptr;
    SettingValue<QSize> m_storeDialogSize;
    struct {
        int uploadSpeedLimit;
        int downloadSpeedLimit;
        int altUploadSpeedLimit;
        int altDownloadSpeedLimit;
    } m_initialValues;
};
