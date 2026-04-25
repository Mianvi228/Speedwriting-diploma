#ifndef ACTIONBUTTON_H
#define ACTIONBUTTON_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QEvent>
#include <QKeyEvent>

class ActionButton : public QWidget {
    Q_OBJECT

private:
    Qt::Key key;
    QString action;
    QString description;
    QColor normalColor = QColor(58, 110, 165, 255);
    QColor pressedColor = QColor(90, 140, 229, 255);
    QColor highlightColor = QColor(255, 242, 0, 255);
    QColor actionLabelColor = QColor(255, 215, 0, 255);
    QColor descriptionLabelColor = QColor(255, 255, 255, 255);
    bool active = true;
    bool highlighted = false;
    bool pressed = false;

    QLabel* actionLabel;
    QLabel* descriptionLabel;

public:
    explicit ActionButton(QWidget *parent);

    ActionButton(const QString& actionName,
                 Qt::Key shortcut,
                 QWidget* parent = nullptr,
                 const QString& description = "");

    void setColors(const QString& normal, const QString& pressed);
    void setActive();
    void unsetActive();
    void setHighlighted();
    void unsetHighlighted();

signals:
    void triggered();
    void released();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

public:
    void updateStyle();
};


#endif // ACTIONBUTTON_H
