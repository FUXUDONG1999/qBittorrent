#include "speedlimitdialog.h"

#include <algorithm>

#include <QStyle>

#include "base/bittorrent/session.h"
#include "ui_speedlimitdialog.h"
#include "uithememanager.h"
#include "utils.h"

#define SETTINGS_KEY(name) u"SpeedLimitDialog/" name

namespace {
    void updateSliderValue(QSlider *slider, const int value) {
        if (value > slider->maximum())
            slider->setMaximum(value);
        slider->setValue(value);
    }
}

SpeedLimitDialog::SpeedLimitDialog(QWidget *parent)
        : QDialog{parent}, m_ui{new Ui::SpeedLimitDialog}, m_storeDialogSize{SETTINGS_KEY(u"Size"_s)} {
    m_ui->setupUi(this);

    connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    m_ui->labelGlobalSpeedIcon->setPixmap(
            UIThemeManager::instance()->getScaledPixmap(u"slow_off"_s, Utils::Gui::mediumIconSize(this).height()));
    m_ui->labelAltGlobalSpeedIcon->setPixmap(
            UIThemeManager::instance()->getScaledPixmap(u"slow"_s, Utils::Gui::mediumIconSize(this).height()));

    const auto initSlider = [](QSlider *slider, const int value, const int maximum) {
        slider->setMaximum(maximum);
        slider->setValue(value);
    };
    const auto *session = BitTorrent::Session::instance();
    const int uploadVal = std::max(0, (session->globalUploadSpeedLimit() / 1024));
    const int downloadVal = std::max(0, (session->globalDownloadSpeedLimit() / 1024));
    const int maxUpload = std::max(10000, (session->globalUploadSpeedLimit() / 1024));
    const int maxDownload = std::max(10000, (session->globalDownloadSpeedLimit() / 1024));
    initSlider(m_ui->sliderUploadLimit, uploadVal, maxUpload);
    initSlider(m_ui->sliderDownloadLimit, downloadVal, maxDownload);

    const int altUploadVal = std::max(0, (session->altGlobalUploadSpeedLimit() / 1024));
    const int altDownloadVal = std::max(0, (session->altGlobalDownloadSpeedLimit() / 1024));
    const int altMaxUpload = std::max(10000, (session->altGlobalUploadSpeedLimit() / 1024));
    const int altMaxDownload = std::max(10000, (session->altGlobalDownloadSpeedLimit() / 1024));
    initSlider(m_ui->sliderAltUploadLimit, altUploadVal, altMaxUpload);
    initSlider(m_ui->sliderAltDownloadLimit, altDownloadVal, altMaxDownload);

    m_ui->spinUploadLimit->setValue(uploadVal);
    m_ui->spinDownloadLimit->setValue(downloadVal);
    m_ui->spinAltUploadLimit->setValue(altUploadVal);
    m_ui->spinAltDownloadLimit->setValue(altDownloadVal);

    m_initialValues =
            {
                    m_ui->spinUploadLimit->value(),
                    m_ui->spinDownloadLimit->value(),
                    m_ui->spinAltUploadLimit->value(),
                    m_ui->spinAltDownloadLimit->value()
            };

    // Sync up/down speed limit sliders with their corresponding spinboxes
    connect(m_ui->sliderUploadLimit, &QSlider::valueChanged, m_ui->spinUploadLimit, &QSpinBox::setValue);
    connect(m_ui->sliderDownloadLimit, &QSlider::valueChanged, m_ui->spinDownloadLimit, &QSpinBox::setValue);
    connect(m_ui->spinUploadLimit, qOverload<int>(&QSpinBox::valueChanged), this,
            [this](const int value) { updateSliderValue(m_ui->sliderUploadLimit, value); });
    connect(m_ui->spinDownloadLimit, qOverload<int>(&QSpinBox::valueChanged), this,
            [this](const int value) { updateSliderValue(m_ui->sliderDownloadLimit, value); });
    connect(m_ui->sliderAltUploadLimit, &QSlider::valueChanged, m_ui->spinAltUploadLimit, &QSpinBox::setValue);
    connect(m_ui->sliderAltDownloadLimit, &QSlider::valueChanged, m_ui->spinAltDownloadLimit, &QSpinBox::setValue);
    connect(m_ui->spinAltUploadLimit, qOverload<int>(&QSpinBox::valueChanged), this,
            [this](const int value) { updateSliderValue(m_ui->sliderAltUploadLimit, value); });
    connect(m_ui->spinAltDownloadLimit, qOverload<int>(&QSpinBox::valueChanged), this,
            [this](const int value) { updateSliderValue(m_ui->sliderAltDownloadLimit, value); });

    if (const QSize dialogSize = m_storeDialogSize; dialogSize.isValid())
        resize(dialogSize);
}

SpeedLimitDialog::~SpeedLimitDialog() {
    m_storeDialogSize = size();
    delete m_ui;
}

void SpeedLimitDialog::accept() {
    auto *session = BitTorrent::Session::instance();
    const int uploadLimit = (m_ui->spinUploadLimit->value() * 1024);
    if (m_initialValues.uploadSpeedLimit != m_ui->spinUploadLimit->value())
        session->setGlobalUploadSpeedLimit(uploadLimit);

    const int downloadLimit = (m_ui->spinDownloadLimit->value() * 1024);
    if (m_initialValues.downloadSpeedLimit != m_ui->spinDownloadLimit->value())
        session->setGlobalDownloadSpeedLimit(downloadLimit);

    const int altUploadLimit = (m_ui->spinAltUploadLimit->value() * 1024);
    if (m_initialValues.altUploadSpeedLimit != m_ui->spinAltUploadLimit->value())
        session->setAltGlobalUploadSpeedLimit(altUploadLimit);

    const int altDownloadLimit = (m_ui->spinAltDownloadLimit->value() * 1024);
    if (m_initialValues.altDownloadSpeedLimit != m_ui->spinAltDownloadLimit->value())
        session->setAltGlobalDownloadSpeedLimit(altDownloadLimit);

    QDialog::accept();
}
