#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "exercisewidget.h"
#include <QLabel>
#include <QKeyEvent>
#include <QShortcut>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
 //   ui->setupUi(this);

 //   QWidget *central = this->centralWidget();

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    stackedLayout = new QStackedLayout;

    QWidget *stackContainer = new QWidget;
    stackContainer->setLayout(stackedLayout);

    QWidget *widget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->addWidget(new QLabel("Hola"));

    stackedLayout->addWidget(widget);

    ExerciseWidget *widgetEx = new ExerciseWidget(this);

    stackedLayout->addWidget(widgetEx);

    pageOneButton = new QPushButton("Page 1");
    pageTwoButton = new QPushButton("Page 2");

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(pageOneButton);
    buttonLayout->addWidget(pageTwoButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(stackContainer);

    connect(pageOneButton, &QPushButton::clicked, this, &MainWindow::pageOne);
    connect(pageTwoButton, &QPushButton::clicked, this, &MainWindow::pageTwo);

    QShortcut *leftShortcut = new QShortcut(QKeySequence(Qt::Key_Left), this);
    QShortcut *rightShortcut = new QShortcut(QKeySequence(Qt::Key_Right) , this);

    connect(leftShortcut, &QShortcut::activated, [this]() {
        int prev = stackedLayout->currentIndex() - 1;
        qInfo() << prev;
        if (prev >= 0) stackedLayout->setCurrentIndex(prev);
        stackedLayout->currentWidget()->setFocus();
    });

    connect(rightShortcut, &QShortcut::activated, [this]() {
        int next = stackedLayout->currentIndex() + 1;
        qInfo() << next;
        if (next < stackedLayout->count()) stackedLayout->setCurrentIndex(next);
        stackedLayout->currentWidget()->setFocus();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
}

void MainWindow::pageOne()
{
    if (stackedLayout->currentWidget() != nullptr)
        stackedLayout->removeWidget(stackedLayout->currentWidget());

    QWidget *widget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->addWidget(new QLabel("Hola"));

    stackedLayout->addWidget(widget);

    stackedLayout->currentWidget()->setFocus();
}

void MainWindow::pageTwo()
{
    if (stackedLayout->currentWidget() != nullptr)
        stackedLayout->removeWidget(stackedLayout->currentWidget());

    ExerciseWidget *widget = new ExerciseWidget(this);

    stackedLayout->addWidget(widget);

    stackedLayout->currentWidget()->setFocus();
}
