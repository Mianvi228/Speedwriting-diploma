#ifndef PALETTESETTINGSWIDGET_H
#define PALETTESETTINGSWIDGET_H

#include <QWidget>
#include <QVector>
#include "keyboardpalette.h"

class QPushButton;
class QCheckBox;

class PaletteSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PaletteSettingsWidget(QWidget *parent = nullptr);

signals:
    void done();

private:
    KeyboardPalette palette;
    QVector<QPushButton *> colorButtons;
    QCheckBox *showKeyboardCheckBox = nullptr;

    void refreshButtons();
    void refreshVisibilityToggle();
    static QString groupKeys(int groupIndex);
    void setButtonColor(int groupIndex, const QColor &color);

private slots:
    void onPickColor(int groupIndex);
    void onSaveClicked();
    void onResetClicked();
};

#endif // PALETTESETTINGSWIDGET_H

