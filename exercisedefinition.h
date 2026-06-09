#ifndef EXERCISEDEFINITION_H
#define EXERCISEDEFINITION_H

#include <QString>

class KeyboardWidget;
class QWidget;

struct ExerciseDefinition
{
    QString id;
    QString title;
    QString text;
    QString keyboardLanguageId;
};

KeyboardWidget *createKeyboardForLanguage(const QString &keyboardLanguageId, QWidget *parent);

#endif // EXERCISEDEFINITION_H
