#include "exercisedefinition.h"

#include "englishkeyboardwidget.h"
#include "russiankeyboardwidget.h"

namespace {
const QString kEnKeyboardId = "english";
const QString kRuKeyboardId = "russian";
}

KeyboardWidget *createKeyboardForLanguage(const QString &keyboardLanguageId, QWidget *parent)
{
    Q_UNUSED(keyboardLanguageId);
    KeyboardWidget *keyboard = nullptr;
    if (keyboardLanguageId == kEnKeyboardId)
        keyboard = new EnglishKeyboardWidget(parent);
    else if (keyboardLanguageId == kRuKeyboardId)
        keyboard = new RussianKeyboardWidget(parent);
    return keyboard;
}
