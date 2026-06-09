#ifndef ENGLISHKEYBOARDWIDGET_H
#define ENGLISHKEYBOARDWIDGET_H

#include "keyboardwidget.h"
#include <QStringList>


class EnglishKeyboardWidget : public KeyboardWidget
{
    Q_OBJECT
public:
    EnglishKeyboardWidget(QWidget *parent = nullptr);
    ~EnglishKeyboardWidget() {};
    void highlightChar(QChar c) override {
        if (mapCharToKeys.contains(c))
        {
            QStringList sl = mapCharToKeys[c];
            foreach (auto iter, sl)
            {
                ActionButton *button = mapKeyToButton[iter];

                if (button)
                    button->setHighlighted();
            }
        }
    };
    void unhighlightChar(QChar c) override  {
        if (mapCharToKeys.contains(c))
        {
            QStringList sl = mapCharToKeys[c];
            foreach (auto iter, sl)
            {
                ActionButton *button = mapKeyToButton[iter];

                if (button)
                    button->unsetHighlighted();
            }
        }
    };
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

#endif // ENGLISHKEYBOARDWIDGET_H
