#include "exerciseoverviewwidget.h"

#include "historywidget.h"

#include <QPushButton>
#include <QVBoxLayout>

namespace {
const QString kFreeTypingEnId = QStringLiteral("Free typing en");
}

ExerciseOverviewWidget::ExerciseOverviewWidget(const ExerciseDefinition &definition, QWidget *parent)
    : QWidget(parent)
    , definition(definition)
{
    historyChart = new HistoryWidget(this, definition.id);
    if (definition.id == kFreeTypingEnId) {
        startButton = new QPushButton(tr("Select text"), this);
        startButton->setMinimumHeight(40);
        connect(startButton, &QPushButton::clicked, this, &ExerciseOverviewWidget::onSelectText);
    } else {
        startButton = new QPushButton(tr("Start exercise"), this);
        startButton->setMinimumHeight(40);
        connect(startButton, &QPushButton::clicked, this, &ExerciseOverviewWidget::onStartExercise);
    }

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(historyChart, 1);
    layout->addWidget(startButton);

}

void ExerciseOverviewWidget::onStartExercise()
{
    emit startExerciseRequested(definition);
}

void ExerciseOverviewWidget::onSelectText()
{
    emit selectTextRequested(definition);
}