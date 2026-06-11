#include "settingswidget.h"

#include "palettesettingswidget.h"
#include "soundsettingswidget.h"
#include "utils.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>

namespace {
const QString kKeyboardColorsId = QStringLiteral("keyboard_colors");
const QString kSoundSettingsId = QStringLiteral("sound_setting");
}

SettingsWidget::SettingsWidget(SoundManager *soundManager, QWidget *parent)
    : QWidget(parent)
    , soundManager(soundManager)
{
    settingInfoById.insert(kKeyboardColorsId, {kKeyboardColorsId, tr("Keyboard colors")});
    settingInfoById.insert(kSoundSettingsId, {kSoundSettingsId, tr("Sound")});

    auto *rootLayout = new QVBoxLayout(this);

    settingsList = new QListWidget(this);
    configureSideListWidget(settingsList);
    for (const SettingInfo &info : settingInfoById) {
        auto *item = new QListWidgetItem(info.title, settingsList);
        item->setData(Qt::UserRole, info.id);
    }

    auto *rightPanel = new QWidget(this);
    auto *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->addWidget(new QLabel(tr("Settings"), rightPanel));

    settingsStack = new QStackedWidget(rightPanel);
    rightLayout->addWidget(settingsStack, 1);

    auto *contentLayout = new QHBoxLayout();
    contentLayout->addWidget(settingsList, 1);
    contentLayout->addWidget(rightPanel, 3);
    rootLayout->addLayout(contentLayout, 1);

    auto *backButton = new QPushButton(tr("Back to menu"), this);
    rootLayout->addWidget(backButton);

    connect(settingsList, &QListWidget::currentRowChanged, this, &SettingsWidget::onSettingChanged);
    connect(backButton, &QPushButton::clicked, this, &SettingsWidget::onBackToMenu);

    settingsList->setCurrentRow(0);
}

bool SettingsWidget::hasUnsavedChangesForSetting(const QString &settingId) const
{
    if (settingId == kKeyboardColorsId && paletteSettingsWidget != nullptr)
        return paletteSettingsWidget->hasUnsavedChanges();
    if (settingId == kSoundSettingsId && soundSettingsWidget != nullptr)
        return soundSettingsWidget->hasUnsavedChanges();
    return false;
}

void SettingsWidget::saveSetting(const QString &settingId)
{
    if (settingId == kKeyboardColorsId && paletteSettingsWidget != nullptr)
        paletteSettingsWidget->applyChanges();
    else if (settingId == kSoundSettingsId && soundSettingsWidget != nullptr)
        soundSettingsWidget->applyChanges();
}

void SettingsWidget::discardSetting(const QString &settingId)
{
    if (settingId == kKeyboardColorsId && paletteSettingsWidget != nullptr)
        paletteSettingsWidget->discardChanges();
    else if (settingId == kSoundSettingsId && soundSettingsWidget != nullptr)
        soundSettingsWidget->discardChanges();
}

void SettingsWidget::promptSaveChangesForSetting(const QString &settingId)
{
    if (settingId.isEmpty() || !hasUnsavedChangesForSetting(settingId))
        return;

    const QMessageBox::StandardButton answer = QMessageBox::question(
        this,
        tr("Save settings"),
        tr("Save changes before leaving this setting?"),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::Yes);

    if (answer == QMessageBox::Yes)
        saveSetting(settingId);
    else
        discardSetting(settingId);
}

void SettingsWidget::showSettingPanel(const QString &settingId)
{
    QWidget *panel = settingsPanelForId(settingId);
    if (panel == nullptr) {
        showPlaceholder();
        return;
    }

    settingsStack->setCurrentWidget(panel);
}

void SettingsWidget::showPlaceholder()
{
    const QString placeholderId = QStringLiteral("placeholder");
    if (!settingPanels.contains(placeholderId)) {
        auto *placeholder = new QWidget(this);
        auto *layout = new QVBoxLayout(placeholder);
        auto *label = new QLabel(tr("Select a setting"), placeholder);
        label->setAlignment(Qt::AlignCenter);
        layout->addWidget(label);
        settingPanels.insert(placeholderId, placeholder);
        settingsStack->addWidget(placeholder);
    }
    settingsStack->setCurrentWidget(settingPanels.value(placeholderId));
}

QWidget *SettingsWidget::settingsPanelForId(const QString &settingId)
{
    if (settingPanels.contains(settingId))
        return settingPanels.value(settingId);

    QWidget *panel = nullptr;
    if (settingId == kKeyboardColorsId) {
        paletteSettingsWidget = new PaletteSettingsWidget(this);
        connect(paletteSettingsWidget, &PaletteSettingsWidget::done, this, &SettingsWidget::done);
        panel = paletteSettingsWidget;
    } else if (settingId == kSoundSettingsId) {
        soundSettingsWidget = new SoundSettingsWidget(soundManager, this);
        panel = soundSettingsWidget;
    }

    if (panel == nullptr)
        return nullptr;

    settingPanels.insert(settingId, panel);
    settingsStack->addWidget(panel);
    return panel;
}

void SettingsWidget::onSettingChanged(int row)
{
    if (row < 0)
        return;

    const QListWidgetItem *item = settingsList->item(row);
    if (item == nullptr) {
        showPlaceholder();
        return;
    }

    const QString newSettingId = item->data(Qt::UserRole).toString();
    if (newSettingId == activeSettingId)
        return;

    if (!activeSettingId.isEmpty())
        promptSaveChangesForSetting(activeSettingId);

    activeSettingId = newSettingId;
    showSettingPanel(newSettingId);
}

void SettingsWidget::onBackToMenu()
{
    if (!activeSettingId.isEmpty())
        promptSaveChangesForSetting(activeSettingId);

    emit done();
}
