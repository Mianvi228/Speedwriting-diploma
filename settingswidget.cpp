#include "settingswidget.h"

#include "palettesettingswidget.h"
#include "utils.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>

namespace {
const QString kKeyboardColorsId = QStringLiteral("keyboard_colors");
}

SettingsWidget::SettingsWidget(QWidget *parent)
    : QWidget(parent)
{
    settingInfoById.insert(kKeyboardColorsId, {kKeyboardColorsId, tr("Keyboard colors")});

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
    connect(backButton, &QPushButton::clicked, this, &SettingsWidget::done);

    settingsList->setCurrentRow(0);
    onSettingChanged();
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
    }

    if (panel == nullptr)
        return nullptr;

    settingPanels.insert(settingId, panel);
    settingsStack->addWidget(panel);
    return panel;
}

void SettingsWidget::onSettingChanged()
{
    const QListWidgetItem *item = settingsList->currentItem();
    if (item == nullptr) {
        showPlaceholder();
        return;
    }

    QWidget *panel = settingsPanelForId(item->data(Qt::UserRole).toString());
    if (panel == nullptr) {
        showPlaceholder();
        return;
    }

    settingsStack->setCurrentWidget(panel);
}
