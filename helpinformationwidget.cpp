#include "helpinformationwidget.h"
#include "utils.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPlainTextEdit>

namespace {
const QString kGuideId = "guide";
const QString kHelpConsistencyId = "consistency";
}

HelpInformationWidget::HelpInformationWidget(QWidget *parent) {
    init();

    auto *rootLayout = new QHBoxLayout(this);

    informationList = new QListWidget(this);
    configureSideListWidget(informationList);
    informationList->setWordWrap(true);
    informationList->addItem(helpInformations.value(kGuideId).title);
    informationList->addItem(helpInformations.value(kHelpConsistencyId).title);
    informationList->item(0)->setData(Qt::UserRole, kGuideId);
    informationList->item(1)->setData(Qt::UserRole, kHelpConsistencyId);

    auto *rightPanel = new QWidget(this);
    auto *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    informationStack = new QStackedWidget(rightPanel);
    rightLayout->addWidget(informationStack);

    rootLayout->addWidget(informationList, 1);
    rootLayout->addWidget(rightPanel, 4);

    connect(informationList, &QListWidget::currentRowChanged, this, &HelpInformationWidget::onHelpInformationChanged);

    showPlaceholder();
}

void HelpInformationWidget::init()
{
    helpInformations.insert(kHelpConsistencyId, {kHelpConsistencyId, "Важность постоянства", R"(Мозг устроен так, чтобы лучше всего усваивать информацию небольшими, но постоянными порциями. Когда вы занимаетесь ежедневно по 15–30 минут, вы создаёте устойчивые нейронные связи.

Это похоже на тропинку в лесу: если ходить по ней каждый день, она становится широкой и удобной. Если же ходить редко, тропинка зарастает, и каждый раз приходится прокладывать её заново. Так и с навыками: короткие, но регулярные занятия запускают процесс автоматизации. Руки и память «запоминают» движения или правила, и со временем они начинают выполняться без сознательного контроля.

При редких, но интенсивных занятиях мозг перегружается, устаёт и начинает сопротивляться. После многочасовой тренировки или изучения большого объёма правил часто наступает чувство опустошения, а на следующий день — отвращение к процессу. Это прямой путь к выгоранию и пропуску занятий на недели или месяцы. Регулярные же короткие подходы создают «эффект накопления»: каждый следующий раз вы начинаете не с нуля, а с того места, где остановились вчера. Прогресс становится стабильным, а сам процесс — привычным и даже приятным. Именно поэтому в обучении печати, спорте или языках главный принцип один: регулярность важнее объёма.)"
                                                });

    helpInformations.insert(kGuideId, {kGuideId, "Важные сведения о приложении", R"(Символ ¶ означает перенос строки, поэтому, если он был встречен в тексте, необходимо нажать Enter.

Каждое упражнение содержит около 2000 символов, поэтому в новичка на упражнение в среднем будет уходить 15-20 минут. Стоит это учитывать перед началом занятия.

При наборе текста следует полагаться на мышечную память и не сильно полагаться на вспомогательную клавиатуру. Клавиатура полезна в случае если непонятно как нажимать определенные клавиши.)"
                                      });
}

void HelpInformationWidget::onHelpInformationChanged()
{
    const QListWidgetItem *item = informationList->currentItem();
    if (item == nullptr) {
        showPlaceholder();
        return;
    }

    showHelpInformation(item->data(Qt::UserRole).toString());
}

void HelpInformationWidget::showPlaceholder()
{
    const QString placeholderId = QStringLiteral("placeholder");
    if (!informationWidgets.contains(placeholderId)) {
        auto *placeholder = new QWidget(this);
        auto *layout = new QVBoxLayout(placeholder);
        auto *label = new QLabel(tr("Выберите статью"), placeholder);
        label->setAlignment(Qt::AlignCenter);
        layout->addWidget(label);
        informationWidgets.insert(placeholderId, placeholder);
        informationStack->addWidget(placeholder);
    }
    informationStack->setCurrentWidget(informationWidgets.value(placeholderId));
}

void HelpInformationWidget::showHelpInformation(const QString& helpInformationId)
{
    const QListWidgetItem *item = informationList->currentItem();
    if (item == nullptr)
    {
        showPlaceholder();
        return;
    }

    QWidget* widget = informationWidgetForId(item->data(Qt::UserRole).toString());
    if (widget == nullptr)
    {
        showPlaceholder();
        return;
    }

    informationStack->setCurrentWidget(widget);
    widget->setFocus();
}


QWidget *HelpInformationWidget::informationWidgetForId(const QString &informationId)
{
    if (informationWidgets.contains(informationId))
        return informationWidgets.value(informationId);

    const HelpInformationDefinition helpInformationDef = helpInformations.value(informationId);

    auto *textEdit = new QTextEdit(this);
    textEdit->setPlainText(helpInformationDef.text);
    textEdit->setReadOnly(true);

    QFont font = textEdit->font();
    font.setPointSize(25);
    textEdit->setFont(font);
    textEdit->document()->setDocumentMargin(30);

    textEdit->setCursor(Qt::ArrowCursor);
    textEdit->viewport()->setCursor(Qt::ArrowCursor);

    setDocumentAlignmentJustify(textEdit);

    //textEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    informationWidgets.insert(informationId, textEdit);
    informationStack->addWidget(textEdit);

    return textEdit;
}

void HelpInformationWidget::setDocumentAlignmentJustify(QTextEdit *textEdit)
{
    if (!textEdit) return;

    QTextCursor cursor = textEdit->textCursor();
    cursor.select(QTextCursor::Document);

    QTextBlockFormat blockFormat;
    blockFormat.setAlignment(Qt::AlignJustify);

    cursor.mergeBlockFormat(blockFormat);
    cursor.clearSelection();

 //   textEdit->setTextCursor(cursor);
}