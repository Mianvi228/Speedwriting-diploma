#ifndef LANGUAGEEXERCISEWIDGET_H
#define LANGUAGEEXERCISEWIDGET_H

#include "exercisedefinition.h"

#include <QWidget>
#include <QMap>
#include <QStringList>

class QListWidget;
class QStackedWidget;
class ExerciseWidget;

class LanguageExerciseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LanguageExerciseWidget(QWidget *parent = nullptr);

signals:
    void exerciseCompleted();
    void selectText(const ExerciseDefinition &definition);
    void startDefinitionExercise(const ExerciseDefinition &definition);

private slots:
    void onLanguageChanged();
    void onExerciseChanged();
    void onSelectText(const ExerciseDefinition &definition);
    void onStartDefinitionExercise(const ExerciseDefinition &definition);

private:
    void populateExercisesForLanguage(const QString &languageId);
    QWidget *exerciseWidgetForId(const QString &exerciseId);
    void showPlaceholder();

    QListWidget *languageList = nullptr;
    QListWidget *exerciseList = nullptr;
    QStackedWidget *exerciseStack = nullptr;

    QMap<QString, QStringList> exercisesByLanguage;
    QMap<QString, ExerciseDefinition> exerciseDefinitions;
    QMap<QString, QWidget *> exerciseWidgets;
    ExerciseWidget *freeTypingWidget = nullptr;
};

#endif // LANGUAGEEXERCISEWIDGET_H
