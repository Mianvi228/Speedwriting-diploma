#include "exercisewidget.h"
#include "ui_exercisewidget.h"
#include "QKeyEvent"
#include "QFileDialog"
#include "QMessageBox"
#include <QTimer>
#include <QDebug>
#include "utils.h"

inline void formatEndsOfLines(QString &s)
{
#ifdef Q_OS_LINUX
    s.replace("\n", "¶\n");
#elif defined(Q_OS_WIN)
    s.replace("\r\n", "¶\n");
#endif
}

void ExerciseWidget::initCounters()
{
    errorsCounter = 0;
    keysCounter = 0;
    time = 0;

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

void ExerciseWidget::initWindow()
{
    initCounters();
    ui->TextBox->setText(s);
    connect(ui->TextBox, &QLineEdit::textChanged, [=]() {
        ui->TextBox->setCursorPosition(0);
    });
    timer->start(1000);
}

ExerciseWidget::ExerciseWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ExerciseWidget)
{
    ui->setupUi(this);
    initWindow();
}

ExerciseWidget::~ExerciseWidget()
{
    delete ui;
}

void ExerciseWidget::keyPressEvent(QKeyEvent *e)
{
    QString eventText = e->text();
    eventText.replace("\r", "¶\n");
    if (!eventText.isEmpty())
    {
        keysCounter++;
        ui->LabelKeysPressed->setText(QString::asprintf("%d", keysCounter));
        q_keysPressed.enqueue(QDateTime::currentSecsSinceEpoch());

        if (s == nullptr) {
            errorsCounter++;
            QMessageBox msgBox;
            msgBox.setText("The end");
            msgBox.exec();
        }
        else if (s.startsWith(eventText))
        {
            ui->TextBox->setText(s.slice(eventText.size()));
            if (s.size() == 0)
            {
                if (!fileReader.isEof())
                {
                    fileReader.readBlock();
                    s = fileReader.getBlock();
                    formatEndsOfLines(s);
                    ui->TextBox->setText(s);
                }
                else
                {
                    QMessageBox msgBox;
                    msgBox.setText("The end");
                    msgBox.exec();
                    s = nullptr;
                }
            }
        }
        else
        {
            errorsCounter++;
            ui->LabelErrors->setText(QString::asprintf("%d", errorsCounter));
        }
    }
}

void ExerciseWidget::on_ResetButton_clicked()
{
    fileReader.resetFile();
    fileReader.readBlock();
    s = fileReader.getBlock();
    formatEndsOfLines(s);
    ui->TextBox->setText(s);
    initCounters();
    timer->start(1000);
}

void ExerciseWidget::on_SelectTextButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Image:"), "./Texts/", tr("Image Files (*.txt)"));
    fileReader.selectFile(filename);
    fileReader.readBlock();
    s = fileReader.getBlock();
    formatEndsOfLines(s);
    ui->TextBox->setText(s);
    initCounters();
    timer->start(1000);
}
