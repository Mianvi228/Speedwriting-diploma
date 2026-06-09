#include "textexercisewidget.h"

#include "keyboardwidget.h"

#include <QFont>
#include <QKeyEvent>
#include <QLabel>
#include <QVBoxLayout>

TextExerciseWidget::TextExerciseWidget(const ExerciseDefinition &definition, QWidget *parent)
    : QWidget(parent)
    , definition(definition)
    , remaining(definition.text)
{
    setFocusPolicy(Qt::StrongFocus);

    textLabel = new QLabel(remaining, this);
    QFont font = textLabel->font();
    font.setPointSize(48);
    font.setBold(true);
    textLabel->setFont(font);
    textLabel->setAlignment(Qt::AlignCenter);

    keyboard = createKeyboardForLanguage(definition.keyboardLanguageId, this);

    auto *layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(textLabel);
    layout->addStretch();
    layout->addWidget(static_cast<QWidget *>(keyboard));

    updateKeyboardHighlight();
}

bool TextExerciseWidget::focusNextPrevChild(bool next)
{
    Q_UNUSED(next);
    return false;
}

void TextExerciseWidget::updateDisplay()
{
    textLabel->setText(remaining);
    textLabel->setStyleSheet(QString());
    updateKeyboardHighlight();
}

void TextExerciseWidget::updateKeyboardHighlight()
{
    if (keyboard == nullptr)
        return;

    if (remaining.isEmpty()) {
        keyboard->unhighlightChar(QChar('\0'));
        return;
    }

    keyboard->highlightChar(remaining.front());
}

void TextExerciseWidget::keyPressEvent(QKeyEvent *e)
{
    QString input;
    if (e->key() == Qt::Key_Tab) {
        input = QStringLiteral("\t");
    } else {
        input = e->text();
    }

    if (input.isEmpty() || remaining.isEmpty()) {
        e->accept();
        return;
    }

    if (remaining.startsWith(input)) {
        const QChar typedChar = remaining.front();
        remaining = remaining.mid(input.size());
        keyboard->unhighlightChar(typedChar);
        updateDisplay();

        if (remaining.isEmpty()) {
            textLabel->setStyleSheet(QStringLiteral("color: #2e7d32;"));
            emit exerciseCompleted();
        }
    } else {
        textLabel->setStyleSheet(QStringLiteral("color: #c62828;"));
    }

    e->accept();
}
