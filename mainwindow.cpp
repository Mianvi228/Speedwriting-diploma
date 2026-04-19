#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "exercisewidget.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
 //   ui->setupUi(this);

 //   QWidget *central = this->centralWidget();

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QWidget *somePage = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(somePage);
    layout->addWidget(new QLabel("Hola"));

    stackedLayout = new QStackedLayout;
    ExerciseWidget *widget = new ExerciseWidget(this);
    stackedLayout->addWidget(somePage);
    stackedLayout->addWidget(widget);

    QWidget *stackContainer = new QWidget;
    stackContainer->setLayout(stackedLayout);

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pageOne()
{
    stackedLayout->setCurrentIndex(0);
}

void MainWindow::pageTwo()
{
    stackedLayout->setCurrentIndex(1);
}
