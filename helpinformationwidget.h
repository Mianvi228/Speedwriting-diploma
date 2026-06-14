#ifndef HELPINFORMATIONWIDGET_H
#define HELPINFORMATIONWIDGET_H

#include <QObject>
#include <QMap>
#include <QStackedWidget>
#include <QListWidget>
#include "helpInformationDefinition.h"

class QTextEdit;

class HelpInformationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HelpInformationWidget(QWidget *parent = nullptr);

private slots:
    void onHelpInformationChanged();

private:
    void init();
    void populateExercisesForLanguage(const QString &languageId);
    void showPlaceholder();
    void showHelpInformation(const QString& helpInformationId);
    QWidget *informationWidgetForId(const QString &informationId);
    void setDocumentAlignmentJustify(QTextEdit *textEdit);

    QStackedWidget *informationStack = nullptr;

    QListWidget *informationList = nullptr;

    QMap<QString, HelpInformationDefinition> helpInformations;
    QMap<QString, QWidget *> informationWidgets;
};

#endif // HELPINFORMATIONWIDGET_H
