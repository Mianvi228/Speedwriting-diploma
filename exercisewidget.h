#ifndef EXERCISEWIDGET_H
#define EXERCISEWIDGET_H

#include <QWidget>
#include <QQueue>
#include "FileBlockReader.h"

namespace Ui {
class ExerciseWidget;
}

class ExerciseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExerciseWidget(QWidget *parent = nullptr);
    ~ExerciseWidget();

protected:
    void keyPressEvent(QKeyEvent *e);

private slots:
    void on_ResetButton_clicked();

    void on_SelectTextButton_clicked();

private:
    Ui::ExerciseWidget *ui;
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

#endif // EXERCISEWIDGET_H
