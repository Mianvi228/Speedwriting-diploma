#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QKeyEvent"
#include "QFileDialog"
#include "QMessageBox"
#include <iostream>
#include <QDebug>

std::string timeFormat(unsigned long long timeInSec)
{
    unsigned int hours = timeInSec / 3600;
    unsigned int minutes = (timeInSec % 3600) / 60;
    unsigned int seconds = timeInSec % 60;

    std::stringstream s;
    s << std::setfill('0') << std::setw(2) << hours << ':'
        << std::setw(2) << minutes << ':'
        << std::setw(2) << seconds;
    return s.str();
}

QString formatEndsOfLines(const char* s)
{
    QString newS;
    newS.replace("\n", "¶\n");
    while (*s != '\0')
    {
        if (*s == '\n')
        {
            newS += "¶";
        }
        newS += *s;
        s++;
    }

    return newS;
}

void MainWindow::initWindow()
{
    errorsCounter = 0;
    keysCounter = 0;
    time = 0;
    ui->LabelTime->setText(QString::fromStdString(timeFormat(time)));
    ui->TextBox->setText(s);
    if (timer != nullptr)
        timer->stop();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=](){
        time++;
        ui->LabelTime->setText(QString::fromStdString(timeFormat(time)));
        while (!q_keysPressed.isEmpty() && QDateTime::currentSecsSinceEpoch() - q_keysPressed.head() > timeWindow) {
            q_keysPressed.dequeue();
        }
        ui->LabelKPM->setText(QString::asprintf("%lli", q_keysPressed.length()));
    });
    connect(ui->TextBox, &QLineEdit::textChanged, [=]() {
        ui->TextBox->setCursorPosition(0);
    });
    timer->start(1000);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    QString eventText = e->text();
    if (eventText == "\r") eventText = "¶\n";
    if (!eventText.isEmpty())
    {
        keysCounter++;
        ui->LabelKeysPressed->setText(QString::asprintf("%d", keysCounter));
        q_keysPressed.enqueue(QDateTime::currentSecsSinceEpoch());

        if (s == nullptr) {
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
                    s.replace("\n", "¶\n");
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

void MainWindow::on_ResetButton_clicked()
{
    fileReader.resetFile();
    fileReader.readBlock();
    s = fileReader.getBlock();
    s.replace("\n", "¶\n");
    ui->TextBox->setText(s);
}

void MainWindow::on_SelectTextButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Image:"), "./Texts/", tr("Image Files (*.txt)"));
    fileReader.selectFile(filename);
    fileReader.readBlock();
    s = fileReader.getBlock();
    s.replace("\n", "¶\n");
    ui->TextBox->setText(s);
}
