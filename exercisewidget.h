#ifndef EXERCISEWIDGET_H
#define EXERCISEWIDGET_H

#include "exercisedefinition.h"

#include <QWidget>
#include <QQueue>
#include <QMap>
#include <QString>
#include "FileBlockReader.h"
#include "actionbutton.h"
#include "keyboardwidget.h"
#include "soundmanager.h"

namespace Ui {
class ExerciseWidget;
}

class ExerciseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExerciseWidget(QWidget *parent = nullptr);
    ~ExerciseWidget();

    void startWithDefinition(const ExerciseDefinition &definition);
    static void setSoundManager(SoundManager* soundManager);
    const QString getFilename() const;

signals:
    void textSelected();
    void exerciseCompleted();

protected:
    void keyPressEvent(QKeyEvent *e);
    bool focusNextPrevChild(bool next) override;

private slots:
    void on_ResetButton_clicked();

public slots:
    void on_SelectTextButton_clicked();

private:
    Ui::ExerciseWidget *ui;
    FileBlockReader<8096> fileReader;
    QTimer *timer = nullptr;
    unsigned int time;
    unsigned int keysCounter;
    unsigned int errorsCounter;
    QMap<QString, ActionButton*> mapKeyToButton;
    KeyboardWidget *keyboard = nullptr;

    static SoundManager *soundManager;

    QString s = "";
    ExerciseDefinition currentDefinition;
    QString exerciseId;
    bool definitionMode = false;
    QChar prevSym = '\0';
    int sPos = 0;
    QQueue<qint64> q_keysPressed;
    const int timeWindow = 60;
    bool exerciseFinished = false;
    QString currentFilePath;

    void initCounters();
    void initHelpKeyboard(const QString &keyboardLanguageId = QStringLiteral("english"));
    void initWindow();
    void loadText(const QString &text);
    void setKeyboardForLanguage(const QString &keyboardLanguageId);
    void applyKeyboardVisibility();
    void finishExercise(bool withSave = false);
    void loadCurrentBlock();
    void saveCompletedSession();
    void pause();
};

#endif // EXERCISEWIDGET_H
