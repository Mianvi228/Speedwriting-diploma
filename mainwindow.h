#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedLayout>
#include <QPushButton>

#include "exercisedefinition.h"

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
    void pageHistory();
    void pageSettings();
    void pageDefinitionExercise(const ExerciseDefinition &definition);

private:
    void replaceCurrentPage(QWidget *newPage);
    Ui::MainWindow *ui;
    QStackedLayout *stackedLayout;
    QPushButton *pageOneButton;
    QPushButton *pageTwoButton;
    QPushButton *historyButton;
};

#endif // MAINWINDOW_H
