#ifndef KEYBOARDWIDGET_H
#define KEYBOARDWIDGET_H

#include <QMap>
#include <QList>
#include <QKeySequence>
#include <QWidget>

class KeyboardWidget : public QWidget
{
    Q_OBJECT
public:
    virtual ~KeyboardWidget() {};
    virtual void highlightChar(QChar c) = 0;
    virtual void unhighlightChar(QChar c) = 0;
    virtual bool isCharIn(QChar c) = 0;
    virtual void highlightKey(QKeySequence &qks) = 0;
    virtual void unhighlightKey(QKeySequence &qks) = 0;
    virtual void unhilightAll() = 0;
    virtual void setActive(QKeySequence &qks) = 0;
    virtual void setInactive(QKeySequence &qks) = 0;
    virtual void activateAll() = 0;
    virtual void deactivateAll() = 0;
};

#endif // KEYBOARDWIDGET_H
