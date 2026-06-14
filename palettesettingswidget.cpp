#include "palettesettingswidget.h"

#include "keyboardpalette.h"

#include <QColor>
#include <QColorDialog>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>

PaletteSettingsWidget::PaletteSettingsWidget(QWidget *parent)
    : QWidget(parent)
{
    palette = KeyboardPalette::loadOrCreateDefault();

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(16, 16, 16, 16);

    // Default color row (keys not in any group)
    {
        auto *row = new QHBoxLayout();
        auto *label = new QLabel(tr("Обычные клавиши"));
        auto *btn = new QPushButton();
        defaultColorButton = btn;
        btn->setFixedSize(56, 28);
        btn->setObjectName("defaultColorBtn");
        connect(btn, &QPushButton::clicked, this, [this, btn]() {
            QColor picked = QColorDialog::getColor(palette.defaultColor(), this, tr("Выберите обычный цвет"));
            if (!picked.isValid())
                return;
            palette.setDefaultColor(picked);
            btn->setStyleSheet(QString("QPushButton { background-color: %1; border-radius: 4px; }")
                               .arg(picked.name(QColor::HexRgb)));
        });

        row->addWidget(label);
        row->addStretch(1);
        row->addWidget(btn);
        root->addLayout(row);

        refreshDefaultColorButton();
    }

    for (int i = 0; i < KeyboardPalette::GroupCount; ++i) {
        auto *row = new QHBoxLayout();
        auto *label = new QLabel(QString("Группа %1 (%2)").arg(i + 1).arg(groupKeys(i)));
        auto *btn = new QPushButton();
        btn->setFixedSize(56, 28);
        btn->setObjectName(QString("groupColorBtn_%1").arg(i));
        connect(btn, &QPushButton::clicked, this, [this, i]() { onPickColor(i); });
        colorButtons.push_back(btn);

        row->addWidget(label);
        row->addStretch(1);
        row->addWidget(btn);
        root->addLayout(row);
    }

    {
        auto *row = new QHBoxLayout();
        showKeyboardCheckBox = new QCheckBox(tr("Показывать клавиатуру во время упражнения"), this);
        row->addWidget(showKeyboardCheckBox);
        row->addStretch(1);
        root->addLayout(row);

        connect(showKeyboardCheckBox, &QCheckBox::toggled, this, [this](bool checked) {
            palette.setKeyboardVisibleDuringExercise(checked);
        });
    }

    auto *actionsRow = new QHBoxLayout();
    auto *saveBtn = new QPushButton(tr("Сохранить"));
    auto *resetBtn = new QPushButton(tr("Сбросить значения"));
    actionsRow->addWidget(saveBtn);
    actionsRow->addWidget(resetBtn);

    root->addLayout(actionsRow);

    connect(saveBtn, &QPushButton::clicked, this, &PaletteSettingsWidget::onSaveClicked);
    connect(resetBtn, &QPushButton::clicked, this, &PaletteSettingsWidget::onResetClicked);

    refreshButtons();
    refreshVisibilityToggle();
}

QString PaletteSettingsWidget::groupKeys(int groupIndex)
{
    switch (groupIndex) {
    case 0: return QStringLiteral("Левая рука мизинец");
    case 1: return QStringLiteral("Левая рука безымянный палец");
    case 2: return QStringLiteral("Левая рука средний палец");
    case 3: return QStringLiteral("Левая рука указательный палец");
    case 4: return QStringLiteral("Правая рука указательный палец");
    case 5: return QStringLiteral("Правая рука средний палец");
    case 6: return QStringLiteral("Правая рука безымянный палец");
    case 7: return QStringLiteral("Правая рука мезинец");
    default: return QString();
    }
}

void PaletteSettingsWidget::refreshDefaultColorButton()
{
    if (defaultColorButton == nullptr)
        return;

    defaultColorButton->setStyleSheet(QString("QPushButton { background-color: %1; border-radius: 4px; }")
                                      .arg(palette.defaultColor().name(QColor::HexRgb)));
}

void PaletteSettingsWidget::refreshButtons()
{
    const QVector<QColor> colors = palette.normalColors();
    for (int i = 0; i < colorButtons.size(); ++i) {
        if (i < colors.size())
            setButtonColor(i, colors[i]);
    }
}

void PaletteSettingsWidget::setButtonColor(int groupIndex, const QColor &color)
{
    if (groupIndex < 0 || groupIndex >= colorButtons.size())
        return;

    QPushButton *btn = colorButtons[groupIndex];
    const QString bg = color.name(QColor::HexRgb);
    btn->setStyleSheet(QString("QPushButton { background-color: %1; border-radius: 4px; }")
                       .arg(bg));
}

void PaletteSettingsWidget::refreshVisibilityToggle()
{
    if (showKeyboardCheckBox == nullptr)
        return;

    showKeyboardCheckBox->blockSignals(true);
    showKeyboardCheckBox->setChecked(palette.keyboardVisibleDuringExercise());
    showKeyboardCheckBox->blockSignals(false);
}

void PaletteSettingsWidget::onPickColor(int groupIndex)
{
    const QVector<QColor> colors = palette.normalColors();
    const QColor current = (groupIndex >= 0 && groupIndex < colors.size()) ? colors[groupIndex] : QColor("#000000");
    QColor picked = QColorDialog::getColor(current, this, tr("Выберите цвет для группы %1").arg(groupIndex + 1));
    if (!picked.isValid())
        return;

    palette.setNormalColor(groupIndex, picked);
    setButtonColor(groupIndex, picked);
}

void PaletteSettingsWidget::onSaveClicked()
{
    applyChanges();
    QMessageBox::information(this, tr("Настройки сохранены"), tr("Цвета клавиатуры обновлены."));
    emit done();
}

bool PaletteSettingsWidget::hasUnsavedChanges() const
{
    const KeyboardPalette saved = KeyboardPalette::loadOrCreateDefault();

    if (palette.defaultColor() != saved.defaultColor())
        return true;
    if (palette.keyboardVisibleDuringExercise() != saved.keyboardVisibleDuringExercise())
        return true;

    const QVector<QColor> currentColors = palette.normalColors();
    const QVector<QColor> savedColors = saved.normalColors();
    return currentColors != savedColors;
}

void PaletteSettingsWidget::applyChanges()
{
    palette.save();
}

void PaletteSettingsWidget::discardChanges()
{
    palette = KeyboardPalette::loadOrCreateDefault();
    refreshDefaultColorButton();
    refreshButtons();
    refreshVisibilityToggle();
}

void PaletteSettingsWidget::onResetClicked()
{
    palette = KeyboardPalette::defaultPalette();
    refreshDefaultColorButton();
    refreshButtons();
    refreshVisibilityToggle();
}

