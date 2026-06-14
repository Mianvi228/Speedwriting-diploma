#ifndef RUSSIANKEYBOARDWIDGET_H
#define RUSSIANKEYBOARDWIDGET_H

#include "keyboardwidget.h"
#include <actionbutton.h>

class RussianKeyboardWidget : public KeyboardWidget
{
    Q_OBJECT
public:
    RussianKeyboardWidget(QWidget *parent = nullptr);
    RussianKeyboardWidget() {};
    void highlightChar(QChar c) override;
    void unhighlightChar(QChar c) override;
    bool isCharIn(QChar c) override;
    void highlightKey(QKeySequence &qks) override {};
    void unhighlightKey(QKeySequence &qks) override {};
    void unhilightAll() override {};
    void setActive(QKeySequence &qks) override {};
    void setInactive(QKeySequence &qks) override {};
    void activateAll() override {};
    void deactivateAll() override {};

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
private:
    QMap<QString, ActionButton*> mapKeyToButton;
    const static QMap<QChar, QStringList> mapCharToKeys;
    void applyColumnColors();

    QWidget *ui = nullptr;
};

#endif // RUSSIANKEYBOARDWIDGET_H
