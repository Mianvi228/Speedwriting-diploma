#ifndef VIRTUALKEYBOARD_H
#define VIRTUALKEYBOARD_H

#include <QWidget>
#include "actionbutton.h"

class VirtualKeyboard : public QWidget
{
    Q_OBJECT
public:
    explicit VirtualKeyboard(QWidget *parent = nullptr);

signals:
};

#endif // VIRTUALKEYBOARD_H
