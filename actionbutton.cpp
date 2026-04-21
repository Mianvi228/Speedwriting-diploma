#include "actionbutton.h"

ActionButton::ActionButton(const QString& actionName,
                 Qt::Key shortcut,
                 QWidget* parent)
        : QWidget(parent), action(actionName), key(shortcut)
{
    setMinimumSize(100, 100);
    setFocusPolicy(Qt::StrongFocus);

    QWidget *widget = new QWidget(this);

    QVBoxLayout* buttonLayout = new QVBoxLayout(widget);
    buttonLayout->setContentsMargins(5, 5, 5, 5);
    buttonLayout->setSpacing(0);

// 	Left top - key hint
    keyLabel = new QLabel(QKeySequence(key).toString(), widget);
    keyLabel->setStyleSheet("background: transparent; font-size: 16px; font-weight: bold; color: #FFD700;");
    keyLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    keyLabel->setMinimumWidth(40);
    buttonLayout->addWidget(keyLabel);

    // Left botton - action info
    actionLabel = new QLabel(actionName, widget);
    actionLabel->setStyleSheet("background: transparent; font-size: 14px; font-weight: bold; color: white;");
    actionLabel->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    buttonLayout->addWidget(actionLabel);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(widget);

    updateStyle();

    if (parent) parent->installEventFilter(this);
}

void ActionButton::setColors(const QString& normal, const QString& pressed)
{
    normalColor = normal;
    pressedColor = pressed;
    updateStyle();
}

bool ActionButton::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* ke = static_cast<QKeyEvent*>(event);
        if (ke->key() == key && !active) {
            active = true;
            updateStyle();
            emit triggered();
            return false;
        }
    }
    else if (event->type() == QEvent::KeyRelease) {
        QKeyEvent* ke = static_cast<QKeyEvent*>(event);
        if (ke->key() == key && active) {
            active = false;
            updateStyle();
            emit released();
            return false;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void ActionButton::updateStyle()
{
    QString color = active ? pressedColor : normalColor;
    setStyleSheet(QString("QWidget { background-color: %1; border-radius: 8px; }").arg(color));
}
