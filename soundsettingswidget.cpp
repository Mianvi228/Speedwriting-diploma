#include "soundsettingswidget.h"

#include "soundmanager.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QtMath>

SoundSettingsWidget::SoundSettingsWidget(SoundManager *soundManager, QWidget *parent)
    : QWidget(parent)
    , soundManager(soundManager)
{
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(16, 16, 16, 16);

    clickSoundsCheckBox = new QCheckBox(tr("Play key click sounds"), this);
    root->addWidget(clickSoundsCheckBox);

    auto *volumeRow = new QHBoxLayout();
    volumeRow->addWidget(new QLabel(tr("Volume"), this));
    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    volumeValueLabel = new QLabel(this);
    volumeValueLabel->setMinimumWidth(40);
    volumeRow->addWidget(volumeSlider, 1);
    volumeRow->addWidget(volumeValueLabel);
    root->addLayout(volumeRow);

    auto *actionsRow = new QHBoxLayout();
    auto *saveBtn = new QPushButton(tr("Save"), this);
    auto *resetBtn = new QPushButton(tr("Reset defaults"), this);
    actionsRow->addWidget(saveBtn);
    actionsRow->addWidget(resetBtn);
    root->addLayout(actionsRow);
    root->addStretch(1);

    connect(clickSoundsCheckBox, &QCheckBox::toggled, this, &SoundSettingsWidget::onClickSoundsToggled);
    connect(volumeSlider, &QSlider::valueChanged, this, &SoundSettingsWidget::onVolumeChanged);
    connect(saveBtn, &QPushButton::clicked, this, &SoundSettingsWidget::onSaveClicked);
    connect(resetBtn, &QPushButton::clicked, this, &SoundSettingsWidget::onResetClicked);

    refreshControls();
}

void SoundSettingsWidget::refreshControls()
{
    if (soundManager == nullptr)
        return;

    clickSoundsCheckBox->blockSignals(true);
    volumeSlider->blockSignals(true);

    clickSoundsCheckBox->setChecked(soundManager->clickSoundsEnabled());
    volumeSlider->setValue(static_cast<int>(qRound(soundManager->volume() * 100.0)));
    volumeSlider->setEnabled(soundManager->clickSoundsEnabled());
    volumeValueLabel->setText(QStringLiteral("%1%").arg(volumeSlider->value()));

    clickSoundsCheckBox->blockSignals(false);
    volumeSlider->blockSignals(false);
}

void SoundSettingsWidget::onClickSoundsToggled(bool enabled)
{
    volumeSlider->setEnabled(enabled);
}

void SoundSettingsWidget::onVolumeChanged(int value)
{
    volumeValueLabel->setText(QStringLiteral("%1%").arg(value));
}

void SoundSettingsWidget::onSaveClicked()
{
    applyChanges();
    QMessageBox::information(this, tr("Settings saved"), tr("Sound settings have been updated."));
}

bool SoundSettingsWidget::hasUnsavedChanges() const
{
    if (soundManager == nullptr)
        return false;

    const bool clickSoundsEnabled = clickSoundsCheckBox->isChecked();
    const int volumePercent = volumeSlider->value();

    return clickSoundsEnabled != soundManager->clickSoundsEnabled()
        || volumePercent != static_cast<int>(qRound(soundManager->volume() * 100.0));
}

void SoundSettingsWidget::applyChanges()
{
    if (soundManager == nullptr)
        return;

    soundManager->setClickSoundsEnabled(clickSoundsCheckBox->isChecked());
    soundManager->setVolume(static_cast<qreal>(volumeSlider->value()) / 100.0);
    soundManager->saveSettings();
}

void SoundSettingsWidget::discardChanges()
{
    refreshControls();
}

void SoundSettingsWidget::onResetClicked()
{
    clickSoundsCheckBox->blockSignals(true);
    volumeSlider->blockSignals(true);

    clickSoundsCheckBox->setChecked(true);
    volumeSlider->setValue(100);
    volumeSlider->setEnabled(true);
    volumeValueLabel->setText(QStringLiteral("100%"));

    clickSoundsCheckBox->blockSignals(false);
    volumeSlider->blockSignals(false);
}
