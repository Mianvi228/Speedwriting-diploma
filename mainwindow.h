#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedLayout>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *e);

private slots:
    void pageOne();
    void pageTwo();

private:
    Ui::MainWindow *ui;
    QStackedLayout *stackedLayout;
    QPushButton *pageOneButton;
    QPushButton *pageTwoButton;
};

#endif // MAINWINDOW_H
