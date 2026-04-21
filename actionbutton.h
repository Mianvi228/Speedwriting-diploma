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
    QString normalColor = "#3a6ea5";
    QString pressedColor = "#5a8ec5";
    bool active = false;

    QLabel* actionLabel;
    QLabel* keyLabel;

public:
    explicit ActionButton(QWidget *parent);

    ActionButton(const QString& actionName,
                 Qt::Key shortcut,
                 QWidget* parent = nullptr);

    void setColors(const QString& normal, const QString& pressed);

signals:
    void triggered();
    void released();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    void updateStyle();
};


#endif // ACTIONBUTTON_H
