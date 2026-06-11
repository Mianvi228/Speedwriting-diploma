#ifndef EXERCISEOVERVIEWWIDGET_H
#define EXERCISEOVERVIEWWIDGET_H

#include "exercisedefinition.h"

#include <QWidget>

class ExerciseWidget;
class HistoryWidget;
class QPushButton;
class QStackedWidget;

class ExerciseOverviewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExerciseOverviewWidget(const ExerciseDefinition &definition, QWidget *parent = nullptr);

    const ExerciseDefinition &exerciseDefinition() const { return definition; }

signals:
    void selectTextRequested(const ExerciseDefinition &definition);
    void startExerciseRequested(const ExerciseDefinition &definition);

private slots:
    void onStartExercise();
    void onSelectText();

private:
    ExerciseDefinition definition;
    HistoryWidget *historyChart = nullptr;
    QPushButton *startButton = nullptr;
};

#endif // EXERCISEOVERVIEWWIDGET_H
