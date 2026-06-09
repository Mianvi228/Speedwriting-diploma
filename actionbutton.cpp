#include "actionbutton.h"

#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileInfo>

ActionButton::ActionButton(const QString& actionName,
                 Qt::Key *mainKey,
                 QWidget* parent,
                 const QString& description,
                 Qt::Key *subKey,
                 quint32 code)
    : QWidget(parent), action(actionName), mainKey(mainKey), subKey(subKey), code(code)
{
    loadOrCreateGlobalStyle(*this);

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
    QColor n(normal);
    QColor p(pressed);
    if (n.isValid()) normalColor = n;
    if (p.isValid()) pressedColor = p;
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

void ActionButton::setMonitorParent(QWidget *monitorTarget)
{
    monitorTarget->installEventFilter(this);
}

bool ActionButton::eventFilter(QObject* obj, QEvent* event)
{
    /*
      if (!active)
        return false;
    */
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* ke = static_cast<QKeyEvent*>(event);
        if (((code != 0 && ke->nativeScanCode() == code) ||
             (mainKey && ke->key() == *mainKey) ||
             (subKey && ke->key() == *subKey)) &&
             !pressed) {
            pressed = true;
            //highlighted = false;
            updateStyle();
            emit triggered();
            return false;
        }
    }
    else if (event->type() == QEvent::KeyRelease) {
        QKeyEvent* ke = static_cast<QKeyEvent*>(event);
        if (((code != 0 && ke->nativeScanCode() == code) ||
             (mainKey && ke->key() == *mainKey) ||
             (subKey && ke->key() == *subKey)) &&
             pressed) {
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
    QString style("QWidget { background-color: %1; border-radius: %4px; }"
                  "QWidget#actionLabel { background: transparent; font-size: 16px; font-weight: bold; color: %2; }"
                  "QWidget#descriptionLabel { background: transparent; font-size: 14px; font-weight: bold; color: %3; }");
    style = style
                    .arg(color.name(QColor::HexArgb))
                    .arg(actionLabelColor.name(QColor::HexArgb))
                    .arg(descriptionLabelColor.name(QColor::HexArgb))
                    .arg(borderRadius);
    if (highlighted)
        style += QString("QWidget#mainLayout { border: 5px solid %4 }")
                     .arg(highlightColor.name(QColor::HexArgb));
    setStyleSheet(style);
}

void ActionButton::loadOrCreateGlobalStyle(ActionButton &btn)
{
    const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (dir.isEmpty())
        return;

    QDir().mkpath(dir);
    const QString path = dir + "/action_button_style.json";

    // Create file with defaults if missing.
    if (!QFileInfo::exists(path)) {
        QJsonObject obj;
        obj["normalColor"] = btn.normalColor.name(QColor::HexRgb);
        obj["pressedColor"] = btn.pressedColor.name(QColor::HexRgb);
        obj["highlightColor"] = btn.highlightColor.name(QColor::HexRgb);
        obj["actionLabelColor"] = btn.actionLabelColor.name(QColor::HexRgb);
        obj["descriptionLabelColor"] = btn.descriptionLabelColor.name(QColor::HexRgb);
        obj["borderRadius"] = btn.borderRadius;
        obj["version"] = 1;

        QFile out(path);
        if (out.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            out.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
            out.close();
        }
        return;
    }

    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
        return;

    const QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
    f.close();
    if (!doc.isObject())
        return;

    const QJsonObject obj = doc.object();
    const auto colorFrom = [](const QJsonValue &v, const QColor &fallback) {
        const QString hex = v.toString();
        QColor c(hex);
        return c.isValid() ? c : fallback;
    };

    btn.normalColor = colorFrom(obj.value("normalColor"), btn.normalColor);
    btn.pressedColor = colorFrom(obj.value("pressedColor"), btn.pressedColor);
    btn.highlightColor = colorFrom(obj.value("highlightColor"), btn.highlightColor);
    btn.actionLabelColor = colorFrom(obj.value("actionLabelColor"), btn.actionLabelColor);
    btn.descriptionLabelColor = colorFrom(obj.value("descriptionLabelColor"), btn.descriptionLabelColor);
    btn.borderRadius = qBound(0, obj.value("borderRadius").toInt(btn.borderRadius), 64);
}
