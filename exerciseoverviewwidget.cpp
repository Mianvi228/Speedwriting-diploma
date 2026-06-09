#include "exerciseoverviewwidget.h"

#include "historywidget.h"

#include <QPushButton>
#include <QVBoxLayout>

ExerciseOverviewWidget::ExerciseOverviewWidget(const ExerciseDefinition &definition, QWidget *parent)
    : QWidget(parent)
    , definition(definition)
{
    historyChart = new HistoryWidget(this, definition.id);
    startButton = new QPushButton(tr("Start exercise"), this);
    startButton->setMinimumHeight(40);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(historyChart, 1);
    layout->addWidget(startButton);

    connect(startButton, &QPushButton::clicked, this, &ExerciseOverviewWidget::onStartExercise);
}

void ExerciseOverviewWidget::onStartExercise()
{
    emit startExerciseRequested(definition);
}
