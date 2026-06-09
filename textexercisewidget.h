#ifndef TEXTEXERCISEWIDGET_H
#define TEXTEXERCISEWIDGET_H

#include "exercisedefinition.h"

#include <QWidget>

class QLabel;
class KeyboardWidget;

class TextExerciseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TextExerciseWidget(const ExerciseDefinition &definition, QWidget *parent = nullptr);

    QString exerciseId() const { return definition.id; }

signals:
    void exerciseCompleted();

protected:
    void keyPressEvent(QKeyEvent *e) override;
    bool focusNextPrevChild(bool next) override;

private:
    void updateDisplay();
    void updateKeyboardHighlight();

    ExerciseDefinition definition;
    QLabel *textLabel = nullptr;
    KeyboardWidget *keyboard = nullptr;
    QString remaining;
};

#endif // TEXTEXERCISEWIDGET_H
