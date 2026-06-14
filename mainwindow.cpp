#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "languageexercisewidget.h"
#include "helpinformationwidget.h"
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

    soundManager = new SoundManager();

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    stackedLayout = new QStackedLayout;

    QWidget *stackContainer = new QWidget;
    stackContainer->setLayout(stackedLayout);

    stackedLayout->addWidget(new QWidget(this));

    mainMenuButton = new QPushButton("Меню");

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(mainMenuButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(stackContainer);

    connect(mainMenuButton, &QPushButton::clicked, this, &MainWindow::mainMenu);

    mainMenu();

    ExerciseWidget::setSoundManager(soundManager);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
}

void MainWindow::mainMenu()
{
    QWidget *homePage = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(homePage);
    QPushButton *startExerciseButton = new QPushButton("Меню упражнений");
    QPushButton *openHelpInformationButton = new QPushButton("Полезные статьи");
    QPushButton *openHistoryButton = new QPushButton("История упражнений");
    QPushButton *openSettingsButton = new QPushButton(tr("Настройки"));
    startExerciseButton->setMinimumHeight(40);
    openHelpInformationButton->setMinimumHeight(40);
    openHistoryButton->setMinimumHeight(40);
    openSettingsButton->setMinimumHeight(40);
    layout->addWidget(startExerciseButton);
    layout->addWidget(openHelpInformationButton);
    layout->addWidget(openHistoryButton);
    layout->addWidget(openSettingsButton);
    connect(startExerciseButton, &QPushButton::clicked, this, &MainWindow::exerciseMenu);
    connect(openHelpInformationButton, &QPushButton::clicked, this, &MainWindow::helpInformationMenu);
    connect(openHistoryButton, &QPushButton::clicked, this, &MainWindow::pageHistory);
    connect(openSettingsButton, &QPushButton::clicked, this, &MainWindow::pageSettings);
    replaceCurrentPage(homePage);
}

void MainWindow::exerciseMenu()
{
    LanguageExerciseWidget *exercisePage = new LanguageExerciseWidget(this);
    connect(exercisePage, &LanguageExerciseWidget::exerciseCompleted, this, &MainWindow::mainMenu);
    connect(exercisePage, &LanguageExerciseWidget::startDefinitionExercise,
            this, &MainWindow::pageDefinitionExercise);
    connect(exercisePage, &LanguageExerciseWidget::selectText,
            this, &MainWindow::pageSelectedText);
    replaceCurrentPage(exercisePage);
}

void MainWindow::helpInformationMenu()
{
    HelpInformationWidget *helpInformationPage = new HelpInformationWidget(this);
    replaceCurrentPage(helpInformationPage);
}

void MainWindow::pageDefinitionExercise(const ExerciseDefinition &definition)
{
    ExerciseWidget *exercisePage = new ExerciseWidget(definition.keyboardLanguageId, this);
    connect(exercisePage, &ExerciseWidget::exerciseCompleted, this, &MainWindow::exerciseMenu);
    replaceCurrentPage(exercisePage);
    exercisePage->startWithDefinition(definition);
    exercisePage->setFocus();
}

void MainWindow::pageSelectedText(const ExerciseDefinition &definition)
{
    ExerciseWidget *exercisePage = new ExerciseWidget(definition.keyboardLanguageId, this);
    exercisePage->on_SelectTextButton_clicked(definition);
    if (exercisePage->getFilename().isEmpty())
        return;
    connect(exercisePage, &ExerciseWidget::exerciseCompleted, this, &MainWindow::exerciseMenu);
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
    SettingsWidget *settingsPage = new SettingsWidget(soundManager, this);
    connect(settingsPage, &SettingsWidget::done, this, &MainWindow::mainMenu);
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
