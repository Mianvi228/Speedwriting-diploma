#include "exercisedefinition.h"

#include "englishkeyboardwidget.h"

KeyboardWidget *createKeyboardForLanguage(const QString &keyboardLanguageId, QWidget *parent)
{
    Q_UNUSED(keyboardLanguageId);
    return new EnglishKeyboardWidget(parent);
}
