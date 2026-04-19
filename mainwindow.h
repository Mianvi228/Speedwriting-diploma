#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "FileBlockReader.h"
#include <QQueue>
#include <QElapsedTimer>

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
    void on_ResetButton_clicked();

    void on_SelectTextButton_clicked();

private:
    Ui::MainWindow *ui;
    FileBlockReader<8096> fileReader;
    QTimer *timer = nullptr;
    unsigned int time;
    unsigned int keysCounter;
    unsigned int errorsCounter;

    QString s = "¶\n";
    int sPos = 0;
    QQueue<qint64> q_keysPressed;
    const int timeWindow = 60;

    void initCounters();
    void initWindow();
};

#endif // MAINWINDOW_H
