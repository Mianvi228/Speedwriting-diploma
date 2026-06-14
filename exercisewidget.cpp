#include "exercisewidget.h"

#include "savinglineedit.h"
#include "ui_exercisewidget.h"
#include "QKeyEvent"
#include "QFileDialog"
#include "QMessageBox"
#include <QTimer>
#include <QDebug>
#include <QApplication>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include "utils.h"
#include "keyboardpalette.h"

SoundManager* ExerciseWidget::soundManager = nullptr;

inline void formatEndsOfLines(QString &s)
{
    if (s.contains('\r'))
        s.replace("\r\n", "¶\n");
    else
        s.replace("\n", "¶\n");
}

void ExerciseWidget::initCounters()
{
    errorsCounter = 0;
    keysCounter = 0;
    time = 0;
    exerciseFinished = false;

    if (timer == nullptr)
    {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, [=](){
            time++;
            ui->LabelTime->setText(timeFormat(time));
            while (!q_keysPressed.isEmpty() && QDateTime::currentSecsSinceEpoch() - q_keysPressed.head() > timeWindow) {
                q_keysPressed.dequeue();
            }
            ui->LabelKPM->setText(QString::asprintf("%lli", q_keysPressed.length()));
        });
    }
    else
        timer->stop();

    if (!q_keysPressed.isEmpty()) {
        q_keysPressed.clear();
    }

    ui->LabelTime->setText(timeFormat(time));
    ui->LabelErrors->setText(QString::number(errorsCounter));
    ui->LabelKeysPressed->setText(QString::number(keysCounter));
    ui->LabelKPM->setText(QString::asprintf("%lli", q_keysPressed.length()));
}

void ExerciseWidget::loadCurrentBlock()
{
    s = fileReader.getBlock();
    formatEndsOfLines(s);
    ui->TextBox->setText(s);
}

void ExerciseWidget::finishExercise(bool withSave)
{
    exerciseFinished = true;
    timer->stop();
    if (keyboard != nullptr)
        keyboard->unhighlightChar(ui->TextBox->text().isEmpty() ? QChar('\0') : ui->TextBox->text().front());
    ui->TextBox->setText("");
    if (withSave) saveCompletedSession();
    //releaseKeyboard();
    QMessageBox::information(this, tr("Exercise finished"), tr("Exercise finished"));
    emit exerciseCompleted();
}

void ExerciseWidget::saveCompletedSession()
{
    const QString historyDirPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (historyDirPath.isEmpty())
        return;

    QDir historyDir(historyDirPath);
    if (!historyDir.exists() && !historyDir.mkpath(".")) {
        qWarning() << "Failed to create history directory:" << historyDirPath;
        return;
    }

    const QString historyPath = historyDir.filePath("completed_sessions.json");
    QJsonArray sessions;

    QFile historyFile(historyPath);
    if (historyFile.exists() && historyFile.open(QIODevice::ReadOnly)) {
        const QJsonDocument existingDoc = QJsonDocument::fromJson(historyFile.readAll());
        if (existingDoc.isArray())
            sessions = existingDoc.array();
        historyFile.close();
    }

    const int kpm = keysCounter * 60 / time;
    const double accuracy = keysCounter == 0
        ? 0.0
        : (100.0 * static_cast<double>(keysCounter - errorsCounter) / static_cast<double>(keysCounter));

    QJsonObject session;
    session["completedAt"] = QDateTime::currentDateTimeUtc().toString("dd.MM.yyyy HH.mm.ss");
    session["exerciseFilePath"] = currentFilePath;
    session["exerciseFileName"] = currentFilePath.isEmpty() ? QString("unknown") : QFileInfo(currentFilePath).fileName();
    session["durationSeconds"] = static_cast<int>(time);
    session["keysPressed"] = static_cast<int>(keysCounter);
    session["errors"] = static_cast<int>(errorsCounter);
    session["kpm"] = kpm;
    session["accuracy"] = accuracy;
    if (!exerciseId.isEmpty())
        session["exerciseId"] = exerciseId;

    sessions.append(session);

    if (!historyFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning() << "Failed to open history file for writing:" << historyPath;
        return;
    }
    historyFile.write(QJsonDocument(sessions).toJson(QJsonDocument::Indented));
    historyFile.close();
}

void ExerciseWidget::setKeyboardForLanguage(const QString &keyboardLanguageId)
{
    if (keyboard != nullptr) {
        ui->verticalLayout_3->removeWidget(static_cast<QWidget *>(keyboard));
        keyboard->deleteLater();
        keyboard = nullptr;
    }

    keyboard = createKeyboardForLanguage(keyboardLanguageId, this);
    ui->verticalLayout_3->addWidget(static_cast<QWidget *>(keyboard));
    applyKeyboardVisibility();
}

void ExerciseWidget::applyKeyboardVisibility()
{
    if (keyboard == nullptr)
        return;

    const bool visible = KeyboardPalette::loadOrCreateDefault().keyboardVisibleDuringExercise();
    static_cast<QWidget *>(keyboard)->setVisible(visible);
}

void ExerciseWidget::initHelpKeyboard(const QString &keyboardLanguageId)
{
    setKeyboardForLanguage(keyboardLanguageId);
}

void ExerciseWidget::loadText(const QString &text)
{
    s = text;
    formatEndsOfLines(s);
    ui->TextBox->setText(s);

    if (keyboard != nullptr && !s.isEmpty())
        keyboard->highlightChar(s.front());
}

void ExerciseWidget::startWithDefinition(const ExerciseDefinition &definition)
{
    definitionMode = true;
    currentDefinition = definition;
    exerciseId = definition.id;

    ui->SelectTextButton->setVisible(false);
    ui->ResetButton->setVisible(false);
    //setKeyboardForLanguage(definition.keyboardLanguageId);
    if (!definition.id.contains("Free typing"))
        loadText(definition.text);
    initCounters();
    setFocus();
    timer->start(1000);
}

void ExerciseWidget::initWindow(const QString &kKeyboardLanguageId)
{
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
    //grabKeyboard();

    initCounters();
    connect(ui->TextBox, &QLineEdit::textChanged, [=]() {
        ui->TextBox->setCursorPosition(0);

        if (keyboard != nullptr)
        {
            qDebug() << ui->TextBox->getPrevChar();
            keyboard->unhighlightChar(ui->TextBox->getPrevChar());
            if (ui->TextBox->text().length() != 0)
                keyboard->highlightChar(ui->TextBox->text()[0]);
        }
    });
    ui->TextBox->setText(s);
    initHelpKeyboard(kKeyboardLanguageId);

    ui->SelectTextButton->setVisible(false);
    ui->ResetButton->setVisible(false);

    timer->start(1000);
}

ExerciseWidget::ExerciseWidget(const QString& kKeyboardLanguageId, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ExerciseWidget)
{
    ui->setupUi(this);
    initWindow(kKeyboardLanguageId);
}

ExerciseWidget::~ExerciseWidget()
{
    //releaseKeyboard();
    delete ui;
}

bool ExerciseWidget::focusNextPrevChild(bool next)
{
    Q_UNUSED(next);
    // During the exercise we don't want Tab to move focus away.
    return false;
}

void ExerciseWidget::keyPressEvent(QKeyEvent *e)
{
    QString eventText;
    if (soundManager)
        soundManager->playClick();
    if (e->key() == Qt::Key_Tab) {
        eventText = "\t";
    } else if (e->key() == Qt::Key_Escape) {
        pause();
    } else {
        eventText = e->text();
    }
    qInfo() << e->nativeScanCode();
    if (!e->text().isEmpty())
        qInfo() << e->text()[0].unicode();
    qInfo() << e->keyCombination();
    eventText.replace("\r", "¶\n");
    if (!eventText.isEmpty())
    {
        if (exerciseFinished)
            return;

        keysCounter++;
        ui->LabelKeysPressed->setText(QString::asprintf("%d", keysCounter));
        q_keysPressed.enqueue(QDateTime::currentSecsSinceEpoch());

        if (s.startsWith(eventText))
        {
            s = s.slice(eventText.size());
            ui->TextBox->setText(s);
            if (s.isEmpty())
            {
                if (!fileReader.isEof())
                {
                    fileReader.readBlock();
                    loadCurrentBlock();
                }
                else
                {
                    finishExercise(true);
                }
            }
        }
        else
        {
            errorsCounter++;
            ui->LabelErrors->setText(QString::asprintf("%d", errorsCounter));
        }
    }

    e->accept();
}

void ExerciseWidget::pause()
{
    timer->stop();
    QMessageBox *msgBox = new QMessageBox();
    msgBox->setText("Paused");
    msgBox->setWindowTitle("Pause");

    QPushButton *continueButton = msgBox->addButton("Continue", QMessageBox::AcceptRole);
    QPushButton *rejectButton = msgBox->addButton("Exit", QMessageBox::AcceptRole);

    msgBox->setDefaultButton(continueButton);

    //releaseKeyboard();
    msgBox->show();
    msgBox->raise();
    msgBox->activateWindow();
    msgBox->exec();

    if (msgBox->clickedButton() == continueButton) {
        timer->start();
        //grabKeyboard();
    } else if (msgBox->clickedButton() == rejectButton) {
        finishExercise();
    }
}

void ExerciseWidget::on_ResetButton_clicked()
{
    if (definitionMode) {
        loadText(currentDefinition.text);
        initCounters();
        timer->start(1000);
        return;
    }

    fileReader.resetFile();
    fileReader.readBlock();
    loadCurrentBlock();
    initCounters();
    timer->start(1000);
}

void ExerciseWidget::on_SelectTextButton_clicked(const ExerciseDefinition &definition)
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open text file"), "./Texts/", tr("Text Files (*.txt)"));
    if (filename.isEmpty())
        return;

    currentFilePath = filename;
    fileReader.selectFile(filename);
    if (!isFileForCurLang())
    {
        QMessageBox::information(this, "Bad file", QString("File contains other symbols: %1.").arg(s));
        currentFilePath = "";
        return;
    }
    fileReader.resetFile();
    fileReader.readBlock();
    loadCurrentBlock();
    initCounters();
    timer->start(1000);

    emit textSelected();
}

void ExerciseWidget::setSoundManager(SoundManager* soundManager)
{
    ExerciseWidget::soundManager = soundManager;
}

const QString ExerciseWidget::getFilename() const
{
    return currentFilePath;
}

bool ExerciseWidget::isFileForCurLang()
{
    QString s = "";
    while (!fileReader.isEof())
    {
        fileReader.readBlock();
        s = fileReader.getBlock();
        for (QChar c : s)
            if (!keyboard->isCharIn(c))
            {
                this->s = c;
                return false;
            }
    }
    return true;
}
