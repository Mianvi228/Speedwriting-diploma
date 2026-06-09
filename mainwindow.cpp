#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "languageexercisewidget.h"
#include "exercisedefinition.h"
#include "exercisewidget.h"
#include "historywidget.h"
#include "settingswidget.h"
#include <QLabel>
#include <QFont>
#include <QKeyEvent>
#include <QShortcut>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>

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

    stackedLayout->addWidget(new QWidget(this));

    pageOneButton = new QPushButton("Menu");
    historyButton = new QPushButton("History");

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(pageOneButton);
    buttonLayout->addWidget(historyButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(stackContainer);

    connect(pageOneButton, &QPushButton::clicked, this, &MainWindow::pageOne);
    connect(historyButton, &QPushButton::clicked, this, &MainWindow::pageHistory);

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

    pageOne();
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
    QWidget *homePage = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(homePage);
    QPushButton *startExerciseButton = new QPushButton("Start exercise");
    QPushButton *openHistoryButton = new QPushButton("View history metrics");
    QPushButton *openSettingsButton = new QPushButton(tr("Settings"));
    layout->addWidget(startExerciseButton);
    layout->addWidget(openHistoryButton);
    layout->addWidget(openSettingsButton);
    connect(startExerciseButton, &QPushButton::clicked, this, &MainWindow::pageTwo);
    connect(openHistoryButton, &QPushButton::clicked, this, &MainWindow::pageHistory);
    connect(openSettingsButton, &QPushButton::clicked, this, &MainWindow::pageSettings);
    replaceCurrentPage(homePage);
}

void MainWindow::pageTwo()
{
    LanguageExerciseWidget *exercisePage = new LanguageExerciseWidget(this);
    connect(exercisePage, &LanguageExerciseWidget::exerciseCompleted, this, &MainWindow::pageOne);
    connect(exercisePage, &LanguageExerciseWidget::startDefinitionExercise,
            this, &MainWindow::pageDefinitionExercise);
    replaceCurrentPage(exercisePage);
}

void MainWindow::pageDefinitionExercise(const ExerciseDefinition &definition)
{
    ExerciseWidget *exercisePage = new ExerciseWidget(this);
    connect(exercisePage, &ExerciseWidget::exerciseCompleted, this, &MainWindow::pageTwo);
    replaceCurrentPage(exercisePage);
    exercisePage->startWithDefinition(definition);
    exercisePage->setFocus();
}

void MainWindow::pageHistory()
{
    HistoryWidget *historyPage = new HistoryWidget(this);
    replaceCurrentPage(historyPage);
}

void MainWindow::pageSettings()
{
    SettingsWidget *settingsPage = new SettingsWidget(this);
    connect(settingsPage, &SettingsWidget::done, this, &MainWindow::pageOne);
    replaceCurrentPage(settingsPage);
}

void MainWindow::replaceCurrentPage(QWidget *newPage)
{
    QWidget *current = stackedLayout->currentWidget();
    if (current != nullptr) {
        stackedLayout->removeWidget(current);
        current->deleteLater();
    }

    stackedLayout->addWidget(newPage);
    stackedLayout->setCurrentWidget(newPage);
    newPage->setFocus();
}
