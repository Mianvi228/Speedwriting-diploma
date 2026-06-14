#include "exerciseoverviewwidget.h"

#include "historywidget.h"

#include <QPushButton>
#include <QVBoxLayout>

namespace {
const QString kEnFreeTypingId = QStringLiteral("Free typing en");
const QString kRuFreeTypingId = QStringLiteral("Free typing ru");
}

ExerciseOverviewWidget::ExerciseOverviewWidget(const ExerciseDefinition &definition, QWidget *parent)
    : QWidget(parent)
    , definition(definition)
{
    historyChart = new HistoryWidget(this, definition.id);
    if (definition.id == kEnFreeTypingId ||
        definition.id == kRuFreeTypingId) {
        startButton = new QPushButton(tr("Выбрать текст"), this);
        startButton->setMinimumHeight(40);
        connect(startButton, &QPushButton::clicked, this, &ExerciseOverviewWidget::onSelectText);
    } else {
        startButton = new QPushButton(tr("Начать упражнение"), this);
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