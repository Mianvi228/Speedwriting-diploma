#include "actionbutton.h"

ActionButton::ActionButton(const QString& actionName,
                 Qt::Key shortcut,
                 QWidget* parent,
                 const QString& description)
        : QWidget(parent), action(actionName), key(shortcut)
{
    setMinimumSize(10, 10);
    setFocusPolicy(Qt::StrongFocus);

    QWidget *widget = new QWidget(this);
    widget->setObjectName("mainLayout");

    QVBoxLayout* buttonLayout = new QVBoxLayout(widget);
    buttonLayout->setContentsMargins(5, 5, 5, 5);
    buttonLayout->setSpacing(0);

// 	Left top - key hint
    actionLabel = new QLabel(action, widget);
    actionLabel->setObjectName("actionLabel");
    actionLabel->setStyleSheet(QString("background: transparent; font-size: 16px; font-weight: bold; color: %1;").arg(actionLabelColor.name(QColor::HexArgb)));
    actionLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    actionLabel->setMinimumWidth(40);
    buttonLayout->addWidget(actionLabel);

    // Left botton - action info
    descriptionLabel = new QLabel(description, widget);
    descriptionLabel->setObjectName("descriptionLabel");
    descriptionLabel->setStyleSheet(QString("background: transparent; font-size: 14px; font-weight: bold; color: %1;").arg(descriptionLabelColor.name(QColor::HexArgb)));
    descriptionLabel->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    buttonLayout->addWidget(descriptionLabel);

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

void ActionButton::setHighlighted()
{
    highlighted = true;
    updateStyle();
}

void ActionButton::unsetHighlighted()
{
    highlighted = false;
    updateStyle();
}

void ActionButton::setActive()
{
    active = true;
    normalColor.setAlpha(255);
    actionLabelColor.setAlpha(255);
    descriptionLabelColor.setAlpha(255);
    updateStyle();
}

void ActionButton::unsetActive()
{
    active = false;
    normalColor.setAlpha(128);
    actionLabelColor.setAlpha(128);
    descriptionLabelColor.setAlpha(128);
    updateStyle();
}

bool ActionButton::eventFilter(QObject* obj, QEvent* event)
{
    if (!active)
        return false;
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* ke = static_cast<QKeyEvent*>(event);
        if (ke->key() == key && !pressed) {
            pressed = true;
            highlighted = false;
            updateStyle();
            emit triggered();
            return false;
        }
    }
    else if (event->type() == QEvent::KeyRelease) {
        QKeyEvent* ke = static_cast<QKeyEvent*>(event);
        if (ke->key() == key && pressed) {
            pressed = false;
            updateStyle();
            emit released();
            return false;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void ActionButton::updateStyle()
{
    QColor color = pressed ? pressedColor : normalColor;
    if (!highlighted)
        setStyleSheet(QString("QWidget { background-color: %1; border-radius: 8px; }"
                              "QWidget#actionLabel { background: transparent; font-size: 16px; font-weight: bold; color: %2; }"
                              "QWidget#descriptionLabel { background: transparent; font-size: 14px; font-weight: bold; color: %3; }")
                          .arg(color.name(QColor::HexArgb))
                          .arg(actionLabelColor.name(QColor::HexArgb))
                          .arg(descriptionLabelColor.name(QColor::HexArgb)));
    else
        setStyleSheet(QString("QWidget { background-color: %1; border-radius: 8px; }"
                              "QWidget#actionLabel { background: transparent; font-size: 16px; font-weight: bold; color: %2; }"
                              "QWidget#descriptionLabel { background: transparent; font-size: 14px; font-weight: bold; color: %3; }"
                              "QWidget#mainLayout { border: 5px solid %4 }")
                          .arg(color.name(QColor::HexArgb))
                          .arg(actionLabelColor.name(QColor::HexArgb))
                          .arg(descriptionLabelColor.name(QColor::HexArgb))
                          .arg(highlightColor.name()));
}
