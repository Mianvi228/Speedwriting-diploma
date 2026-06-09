#ifndef SAVINGLINEEDIT_H
#define SAVINGLINEEDIT_H

#include <QObject>
#include <QLineEdit>

class SavingLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    void setText(const QString& text) {
        if (!QLineEdit::text().isEmpty())
            savedChar = QLineEdit::text()[0];
        QLineEdit::setText(text);
    };
    QChar getPrevChar() {
        return savedChar;
    }
    SavingLineEdit(QWidget* parent = nullptr);
private:
    QChar savedChar = '\0';
};

#endif // SAVINGLINEEDIT_H
