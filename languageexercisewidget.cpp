#include "languageexercisewidget.h"

#include "exercisewidget.h"
#include "exerciseoverviewwidget.h"
#include "utils.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QStackedWidget>
#include <QVBoxLayout>

namespace {
const QString kEnglishId = QStringLiteral("english");
const QString kFreeTypingId = QStringLiteral("free_typing");
const QString kJfId = QStringLiteral("jf");
}

LanguageExerciseWidget::LanguageExerciseWidget(QWidget *parent)
    : QWidget(parent)
{
    exercisesByLanguage.insert(kEnglishId, {kFreeTypingId, kJfId});
    exerciseDefinitions.insert(kFreeTypingId, {kFreeTypingId, tr("Free typing"), QString(), kEnglishId});
    exerciseDefinitions.insert(kJfId, {kJfId, tr("jf keys"), R"(jf fj ff jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf
fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff
fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj
jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf
fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff
jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf
jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff
jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj
fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf
fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj
jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf
fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff
jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj
fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf
jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf
fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf fjj ffj jff fjf fjjf jffj fjf jf jff jffj fjf jfj ff jf jfjf)"
                                       , kEnglishId});

    auto *rootLayout = new QHBoxLayout(this);

    languageList = new QListWidget(this);
    configureSideListWidget(languageList);
    languageList->addItem(tr("English"));
    languageList->item(0)->setData(Qt::UserRole, kEnglishId);

    auto *rightPanel = new QWidget(this);
    auto *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    exerciseList = new QListWidget(rightPanel);
    configureSideListWidget(exerciseList);
    exerciseStack = new QStackedWidget(rightPanel);

    rightLayout->addWidget(new QLabel(tr("Exercises"), rightPanel));
    rightLayout->addWidget(exerciseList, 1);
    rightLayout->addWidget(exerciseStack, 3);

    rootLayout->addWidget(languageList, 1);
    rootLayout->addWidget(rightPanel, 3);

    connect(languageList, &QListWidget::currentRowChanged, this, &LanguageExerciseWidget::onLanguageChanged);
    connect(exerciseList, &QListWidget::currentRowChanged, this, &LanguageExerciseWidget::onExerciseChanged);

    languageList->setCurrentRow(0);
    showPlaceholder();
}

void LanguageExerciseWidget::showPlaceholder()
{
    const QString placeholderId = QStringLiteral("placeholder");
    if (!exerciseWidgets.contains(placeholderId)) {
        auto *placeholder = new QWidget(this);
        auto *layout = new QVBoxLayout(placeholder);
        auto *label = new QLabel(tr("Select an exercise"), placeholder);
        label->setAlignment(Qt::AlignCenter);
        layout->addWidget(label);
        exerciseWidgets.insert(placeholderId, placeholder);
        exerciseStack->addWidget(placeholder);
    }
    exerciseStack->setCurrentWidget(exerciseWidgets.value(placeholderId));
}

void LanguageExerciseWidget::populateExercisesForLanguage(const QString &languageId)
{
    exerciseList->clear();

    const QStringList exerciseIds = exercisesByLanguage.value(languageId);
    for (const QString &exerciseId : exerciseIds) {
        const ExerciseDefinition definition = exerciseDefinitions.value(exerciseId);
        auto *item = new QListWidgetItem(definition.title, exerciseList);
        item->setData(Qt::UserRole, exerciseId);
    }

    showPlaceholder();
}

void LanguageExerciseWidget::onLanguageChanged()
{
    const QListWidgetItem *item = languageList->currentItem();
    if (item == nullptr) {
        exerciseList->clear();
        showPlaceholder();
        return;
    }

    populateExercisesForLanguage(item->data(Qt::UserRole).toString());
}

QWidget *LanguageExerciseWidget::exerciseWidgetForId(const QString &exerciseId)
{
    if (exerciseWidgets.contains(exerciseId))
        return exerciseWidgets.value(exerciseId);

    QWidget *widget = nullptr;
    if (exerciseId == kFreeTypingId) {
        freeTypingWidget = new ExerciseWidget(this);
        connect(freeTypingWidget, &ExerciseWidget::exerciseCompleted, this, &LanguageExerciseWidget::exerciseCompleted);
        widget = freeTypingWidget;
    } else if (exerciseDefinitions.contains(exerciseId) && !exerciseDefinitions.value(exerciseId).text.isEmpty()) {
        auto *overviewWidget = new ExerciseOverviewWidget(exerciseDefinitions.value(exerciseId), this);
        connect(overviewWidget, &ExerciseOverviewWidget::startExerciseRequested,
                this, &LanguageExerciseWidget::onStartDefinitionExercise);
        widget = overviewWidget;
    }

    if (widget == nullptr)
        return nullptr;

    exerciseWidgets.insert(exerciseId, widget);
    exerciseStack->addWidget(widget);
    return widget;
}

void LanguageExerciseWidget::onExerciseChanged()
{
    const QListWidgetItem *item = exerciseList->currentItem();
    if (item == nullptr) {
        showPlaceholder();
        return;
    }

    QWidget *widget = exerciseWidgetForId(item->data(Qt::UserRole).toString());
    if (widget == nullptr) {
        showPlaceholder();
        return;
    }

    exerciseStack->setCurrentWidget(widget);
    widget->setFocus();
}

void LanguageExerciseWidget::onStartDefinitionExercise(const ExerciseDefinition &definition)
{
    emit startDefinitionExercise(definition);
}
