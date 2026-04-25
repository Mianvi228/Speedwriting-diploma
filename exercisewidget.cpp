#include "exercisewidget.h"
#include "ui_exercisewidget.h"
#include "QKeyEvent"
#include "QFileDialog"
#include "QMessageBox"
#include <QTimer>
#include <QDebug>
#include "utils.h"
#include <array>

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

void ExerciseWidget::initHelpKeyboard()
{
    ActionButton *button = nullptr;
    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName("keyboardLine1");
    std::array<int, 14> stretch;

    button = new ActionButton("`", Qt::Key_QuoteLeft, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("`", button);
    button = new ActionButton("1", Qt::Key_1, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("1", button);
    button = new ActionButton("2", Qt::Key_2, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("2", button);
    button = new ActionButton("3", Qt::Key_3, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("3", button);
    button = new ActionButton("4", Qt::Key_4, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("4", button);
    button = new ActionButton("5", Qt::Key_5, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("5", button);
    button = new ActionButton("6", Qt::Key_6, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("6", button);
    button = new ActionButton("7", Qt::Key_7, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("7", button);
    button = new ActionButton("8", Qt::Key_8, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("8", button);
    button = new ActionButton("9", Qt::Key_9, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("9", button);
    button = new ActionButton("0", Qt::Key_0, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("0", button);
    button = new ActionButton("-", Qt::Key_Minus, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("-", button);
    button = new ActionButton("=", Qt::Key_Equal, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("=", button);
    button = new ActionButton("⌫", Qt::Key_Backspace, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("\b", button);

    stretch = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 8};
    for (int i = 0; i < horizontalLayout->count(); i++)
        horizontalLayout->setStretch(i, stretch[i]);

    ui->verticalLayout_3->addLayout(horizontalLayout);
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName("keyboardLine2");

    button = new ActionButton("Tab", Qt::Key_Tab, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("Tab", button);
    button = new ActionButton("Q", Qt::Key_Q, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("Q", button);
    button = new ActionButton("W", Qt::Key_W, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("W", button);
    button = new ActionButton("E", Qt::Key_E, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("E", button);
    button = new ActionButton("R", Qt::Key_R, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("R", button);
    button = new ActionButton("T", Qt::Key_T, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("T", button);
    button = new ActionButton("Y", Qt::Key_Y, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("Y", button);
    button = new ActionButton("U", Qt::Key_U, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("U", button);
    button = new ActionButton("I", Qt::Key_I, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("I", button);
    button = new ActionButton("O", Qt::Key_O, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("O", button);
    button = new ActionButton("P", Qt::Key_P, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("P", button);
    button = new ActionButton("[", Qt::Key_BracketLeft, this, "{");
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("[", button);
    button = new ActionButton("]", Qt::Key_BracketRight, this, "}");
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("]", button);
    button = new ActionButton("\\", Qt::Key_Backslash, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("\\", button);

    stretch = {7, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7};
    for (int i = 0; i < horizontalLayout->count(); i++)
        horizontalLayout->setStretch(i, stretch[i]);

    ui->verticalLayout_3->addLayout(horizontalLayout);
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName("keyboardLine3");

    button = new ActionButton("Caps", Qt::Key_CapsLock, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("Caps", button);
    button = new ActionButton("A", Qt::Key_A, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("A", button);
    button = new ActionButton("S", Qt::Key_S, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("S", button);
    button = new ActionButton("D", Qt::Key_D, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("D", button);
    button = new ActionButton("F", Qt::Key_F, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("F", button);
    button = new ActionButton("G", Qt::Key_G, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("G", button);
    button = new ActionButton("H", Qt::Key_H, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("H", button);
    button = new ActionButton("J", Qt::Key_J, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("J", button);
    button = new ActionButton("K", Qt::Key_K, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("K", button);
    button = new ActionButton("L", Qt::Key_L, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("L", button);
    button = new ActionButton(";", Qt::Key_Semicolon, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert(";", button);
    button = new ActionButton("\"", Qt::Key_Apostrophe, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("\"", button);
    button = new ActionButton("⏎", Qt::Key_Return, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("\n", button);

    stretch = {10, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 10};
    for (int i = 0; i < horizontalLayout->count(); i++)
        horizontalLayout->setStretch(i, stretch[i]);

    ui->verticalLayout_3->addLayout(horizontalLayout);
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName("keyboardLine4");

    button = new ActionButton("⇪", Qt::Key_Shift, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("LShift", button);
    button = new ActionButton("Z", Qt::Key_Z, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("Z", button);
    button = new ActionButton("X", Qt::Key_X, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("X", button);
    button = new ActionButton("C", Qt::Key_C, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("C", button);
    button = new ActionButton("V", Qt::Key_V, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("V", button);
    button = new ActionButton("B", Qt::Key_B, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("B", button);
    button = new ActionButton("N", Qt::Key_N, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("N", button);
    button = new ActionButton("M", Qt::Key_M, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("M", button);
    button = new ActionButton(",", Qt::Key_Comma, this, "<");
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert(",", button);
    button = new ActionButton(".", Qt::Key_Period, this, ">");
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert(".", button);
    button = new ActionButton("/", Qt::Key_Slash, this, "?");
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("/", button);
    button = new ActionButton("⇪", Qt::Key_Shift, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("RShift", button);

    stretch = {12, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 12};
    for (int i = 0; i < horizontalLayout->count(); i++)
        horizontalLayout->setStretch(i, stretch[i]);

    ui->verticalLayout_3->addLayout(horizontalLayout);
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName("keyboardLine5");

    button = new ActionButton("Ctrl", Qt::Key_Control, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("LCtrl", button);
    button = new ActionButton("Super", Qt::Key_Meta, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("LSuper", button);
    button = new ActionButton("Alt", Qt::Key_Alt, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("LAlt", button);
    button = new ActionButton(" ", Qt::Key_Space, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert(" ", button);
    button = new ActionButton("Alt", Qt::Key_Alt, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("RAlt", button);
    button = new ActionButton("Ctrl", Qt::Key_Control, this);
    horizontalLayout->addWidget(button);
    mapKeyToButton.insert("RCtrl", button);

    stretch = {5, 5, 5, 26, 5, 5};
    for (int i = 0; i < horizontalLayout->count(); i++)
        horizontalLayout->setStretch(i, stretch[i]);

    ui->verticalLayout_3->addLayout(horizontalLayout);

    QTimer::singleShot(0, [&]() {
        if (!s.isEmpty())
            for (auto iter = mapKeyToButton.begin(); iter != mapKeyToButton.end(); iter++)
            {
                if (iter.key() == s[0].toUpper())
                    iter.value()->setHighlighted();
                else
                    iter.value()->updateStyle();
            }
    });

}

void ExerciseWidget::initWindow()
{
    initCounters();
    connect(ui->TextBox, &QLineEdit::textChanged, [=]() {
        ui->TextBox->setCursorPosition(0);
    });
    ui->TextBox->setText(s);
    initHelpKeyboard();

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
    qInfo() << e->nativeScanCode();
    qInfo() << e->keyCombination();
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
            else
            {
                for (auto iter = mapKeyToButton.begin(); iter != mapKeyToButton.end(); iter++)
                {
                    if (iter.key() == s[0].toUpper())
                    {
                        iter.value()->setHighlighted();
                        break;
                    }
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
