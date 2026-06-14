#include "russiankeyboardwidget.h"

#include "keyboardpalette.h"

RussianKeyboardWidget::RussianKeyboardWidget(QWidget *parent) {
    ActionButton *button = nullptr;

    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    verticalLayout->setObjectName("keyboardLayout");

    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName("keyboardLine1");

    std::array<int, 14> stretch;

    mapKeyToButton.insert("ё", new ActionButton("ё", new Qt::Key(Qt::Key(1025)), parent, "Ё"));
    mapKeyToButton.insert("Ё", mapKeyToButton["ё"]);
    horizontalLayout->addWidget(mapKeyToButton["ё"]);
    mapKeyToButton.insert("1", new ActionButton("1", new Qt::Key(Qt::Key_1), parent, "!", new Qt::Key(Qt::Key_Exclam)));
    mapKeyToButton.insert("!", mapKeyToButton["1"]);
    horizontalLayout->addWidget(mapKeyToButton["1"]);
    mapKeyToButton.insert("2", new ActionButton("2", new Qt::Key(Qt::Key_2), parent, "\"", new Qt::Key(Qt::Key_QuoteDbl)));
    mapKeyToButton.insert("\"", mapKeyToButton["2"]);
    horizontalLayout->addWidget(mapKeyToButton["2"]);
    mapKeyToButton.insert("3", new ActionButton("3", new Qt::Key(Qt::Key_3), parent, "№", new Qt::Key(Qt::Key(8470))));
    mapKeyToButton.insert("№", mapKeyToButton["3"]);
    horizontalLayout->addWidget(mapKeyToButton["3"]);
    mapKeyToButton.insert("4", new ActionButton("4", new Qt::Key(Qt::Key_4), parent, ";", new Qt::Key(Qt::Key_Semicolon)));
    mapKeyToButton.insert(";", mapKeyToButton["4"]);
    horizontalLayout->addWidget(mapKeyToButton["4"]);
    mapKeyToButton.insert("5", new ActionButton("5", new Qt::Key(Qt::Key_5), parent, "%", new Qt::Key(Qt::Key_Percent)));
    mapKeyToButton.insert("%", mapKeyToButton["5"]);
    horizontalLayout->addWidget(mapKeyToButton["5"]);
    mapKeyToButton.insert("6", new ActionButton("6", new Qt::Key(Qt::Key_6), parent, ":", new Qt::Key(Qt::Key_Colon)));
    mapKeyToButton.insert(":", mapKeyToButton["6"]);
    horizontalLayout->addWidget(mapKeyToButton["6"]);
    mapKeyToButton.insert("7", new ActionButton("7", new Qt::Key(Qt::Key_7), parent, "?", new Qt::Key(Qt::Key_Question)));
    mapKeyToButton.insert("?", mapKeyToButton["7"]);
    horizontalLayout->addWidget(mapKeyToButton["7"]);
    mapKeyToButton.insert("8", new ActionButton("8", new Qt::Key(Qt::Key_8), parent, "*", new Qt::Key(Qt::Key_Asterisk)));
    mapKeyToButton.insert("*", mapKeyToButton["8"]);
    horizontalLayout->addWidget(mapKeyToButton["8"]);
    mapKeyToButton.insert("9", new ActionButton("9", new Qt::Key(Qt::Key_9), parent, "(", new Qt::Key(Qt::Key_ParenLeft)));
    mapKeyToButton.insert("(", mapKeyToButton["9"]);
    horizontalLayout->addWidget(mapKeyToButton["9"]);
    mapKeyToButton.insert("0", new ActionButton("0", new Qt::Key(Qt::Key_0), parent, ")", new Qt::Key(Qt::Key_ParenRight)));
    mapKeyToButton.insert(")", mapKeyToButton["0"]);
    horizontalLayout->addWidget(mapKeyToButton["0"]);
    mapKeyToButton.insert("-", new ActionButton("-", new Qt::Key(Qt::Key_Minus), parent, "_", new Qt::Key(Qt::Key_Underscore)));
    mapKeyToButton.insert("_", mapKeyToButton["-"]);
    horizontalLayout->addWidget(mapKeyToButton["-"]);
    mapKeyToButton.insert("=", new ActionButton("=", new Qt::Key(Qt::Key_Equal), parent, "+", new Qt::Key(Qt::Key_Plus)));
    mapKeyToButton.insert("+", mapKeyToButton["="]);
    horizontalLayout->addWidget(mapKeyToButton["="]);
    mapKeyToButton.insert("Backspace", new ActionButton("Backspace", new Qt::Key(Qt::Key_Backspace), parent, ""));
    mapKeyToButton.insert("\b", mapKeyToButton["Backspace"]);
    horizontalLayout->addWidget(mapKeyToButton["Backspace"]);

    stretch = {3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 10};
    for (int i = 0; i < horizontalLayout->count(); i++)
        horizontalLayout->setStretch(i, stretch[i]);

    verticalLayout->addLayout(horizontalLayout);
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName("keyboardLine2");

    mapKeyToButton.insert("Tab", new ActionButton("Tab", new Qt::Key(Qt::Key_Tab), parent, ""));
    mapKeyToButton.insert("\t", mapKeyToButton["Tab"]);
    horizontalLayout->addWidget(mapKeyToButton["Tab"]);
    mapKeyToButton.insert("й", new ActionButton("Й", new Qt::Key(Qt::Key(1049)), parent, "й"));
    mapKeyToButton.insert("Й", mapKeyToButton["й"]);
    horizontalLayout->addWidget(mapKeyToButton["й"]);
    mapKeyToButton.insert("ц", new ActionButton("Ц", new Qt::Key(Qt::Key(1062)), parent, "ц"));
    mapKeyToButton.insert("Ц", mapKeyToButton["ц"]);
    horizontalLayout->addWidget(mapKeyToButton["ц"]);
    mapKeyToButton.insert("у", new ActionButton("У", new Qt::Key(Qt::Key(1059)), parent, "у"));
    mapKeyToButton.insert("У", mapKeyToButton["у"]);
    horizontalLayout->addWidget(mapKeyToButton["у"]);
    mapKeyToButton.insert("к", new ActionButton("К", new Qt::Key(Qt::Key(1050)), parent, "к"));
    mapKeyToButton.insert("К", mapKeyToButton["к"]);
    horizontalLayout->addWidget(mapKeyToButton["к"]);
    mapKeyToButton.insert("е", new ActionButton("Е", new Qt::Key(Qt::Key(1045)), parent, "е"));
    mapKeyToButton.insert("Е", mapKeyToButton["е"]);
    horizontalLayout->addWidget(mapKeyToButton["е"]);
    mapKeyToButton.insert("н", new ActionButton("Н", new Qt::Key(Qt::Key(1053)), parent, "н"));
    mapKeyToButton.insert("Н", mapKeyToButton["н"]);
    horizontalLayout->addWidget(mapKeyToButton["н"]);
    mapKeyToButton.insert("г", new ActionButton("Г", new Qt::Key(Qt::Key(1043)), parent, "г"));
    mapKeyToButton.insert("Г", mapKeyToButton["г"]);
    horizontalLayout->addWidget(mapKeyToButton["г"]);
    mapKeyToButton.insert("ш", new ActionButton("Ш", new Qt::Key(Qt::Key(1064)), parent, "ш"));
    mapKeyToButton.insert("Ш", mapKeyToButton["ш"]);
    horizontalLayout->addWidget(mapKeyToButton["ш"]);
    mapKeyToButton.insert("щ", new ActionButton("Щ", new Qt::Key(Qt::Key(1065)), parent, "щ"));
    mapKeyToButton.insert("Щ", mapKeyToButton["щ"]);
    horizontalLayout->addWidget(mapKeyToButton["щ"]);
    mapKeyToButton.insert("з", new ActionButton("З", new Qt::Key(Qt::Key(1047)), parent, "з"));
    mapKeyToButton.insert("З", mapKeyToButton["з"]);
    horizontalLayout->addWidget(mapKeyToButton["з"]);
    mapKeyToButton.insert("х", new ActionButton("Х", new Qt::Key(Qt::Key(1061)), parent, "х"));
    mapKeyToButton.insert("Х", mapKeyToButton["х"]);
    horizontalLayout->addWidget(mapKeyToButton["х"]);
    mapKeyToButton.insert("ъ", new ActionButton("Ъ", new Qt::Key(Qt::Key(1066)), parent, "ъ"));
    mapKeyToButton.insert("Ъ", mapKeyToButton["ъ"]);
    horizontalLayout->addWidget(mapKeyToButton["ъ"]);
    mapKeyToButton.insert("\\", new ActionButton("/", new Qt::Key(Qt::Key_Backslash), parent, "\\", new Qt::Key(Qt::Key_Slash)));
    mapKeyToButton.insert("/", mapKeyToButton["\\"]);
    horizontalLayout->addWidget(mapKeyToButton["\\"]);

    stretch = {6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6};
    for (int i = 0; i < horizontalLayout->count(); i++)
        horizontalLayout->setStretch(i, stretch[i]);

    verticalLayout->addLayout(horizontalLayout);
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName("keyboardLine3");

    mapKeyToButton.insert("CapsLock", new ActionButton("CapsLock", new Qt::Key(Qt::Key_CapsLock), parent, ""));
    horizontalLayout->addWidget(mapKeyToButton["CapsLock"]);
    mapKeyToButton.insert("ф", new ActionButton("Ф", new Qt::Key(Qt::Key(1060)), parent, "ф"));
    mapKeyToButton.insert("Ф", mapKeyToButton["ф"]);
    horizontalLayout->addWidget(mapKeyToButton["ф"]);
    mapKeyToButton.insert("ы", new ActionButton("Ы", new Qt::Key(Qt::Key(1067)), parent, "ы"));
    mapKeyToButton.insert("Ы", mapKeyToButton["ы"]);
    horizontalLayout->addWidget(mapKeyToButton["ы"]);
    mapKeyToButton.insert("в", new ActionButton("В", new Qt::Key(Qt::Key(1042)), parent, "в"));
    mapKeyToButton.insert("В", mapKeyToButton["в"]);
    horizontalLayout->addWidget(mapKeyToButton["в"]);
    mapKeyToButton.insert("а", new ActionButton("А", new Qt::Key(Qt::Key(1040)), parent, "а"));
    mapKeyToButton.insert("А", mapKeyToButton["а"]);
    horizontalLayout->addWidget(mapKeyToButton["а"]);
    mapKeyToButton.insert("п", new ActionButton("П", new Qt::Key(Qt::Key(1055)), parent, "п"));
    mapKeyToButton.insert("П", mapKeyToButton["п"]);
    horizontalLayout->addWidget(mapKeyToButton["п"]);
    mapKeyToButton.insert("р", new ActionButton("Р", new Qt::Key(Qt::Key(1056)), parent, "р"));
    mapKeyToButton.insert("Р", mapKeyToButton["р"]);
    horizontalLayout->addWidget(mapKeyToButton["р"]);
    mapKeyToButton.insert("о", new ActionButton("О", new Qt::Key(Qt::Key(1054)), parent, "о"));
    mapKeyToButton.insert("О", mapKeyToButton["о"]);
    horizontalLayout->addWidget(mapKeyToButton["о"]);
    mapKeyToButton.insert("л", new ActionButton("Л", new Qt::Key(Qt::Key(1051)), parent, "л"));
    mapKeyToButton.insert("Л", mapKeyToButton["л"]);
    horizontalLayout->addWidget(mapKeyToButton["л"]);
    mapKeyToButton.insert("д", new ActionButton("Д", new Qt::Key(Qt::Key(1044)), parent, "д"));
    mapKeyToButton.insert("Д", mapKeyToButton["д"]);
    horizontalLayout->addWidget(mapKeyToButton["д"]);
    mapKeyToButton.insert("ж", new ActionButton("Ж", new Qt::Key(Qt::Key(1046)), parent, "ж"));
    mapKeyToButton.insert("Ж", mapKeyToButton["ж"]);
    horizontalLayout->addWidget(mapKeyToButton["ж"]);
    mapKeyToButton.insert("э", new ActionButton("Э", new Qt::Key(Qt::Key(1069)), parent, "э"));
    mapKeyToButton.insert("Э", mapKeyToButton["э"]);
    horizontalLayout->addWidget(mapKeyToButton["э"]);
    mapKeyToButton.insert("Enter", new ActionButton("Enter", new Qt::Key(Qt::Key_Return), parent, ""));
    mapKeyToButton.insert("¶", mapKeyToButton["Enter"]);
    horizontalLayout->addWidget(mapKeyToButton["Enter"]);

    stretch = {8, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 10};
    for (int i = 0; i < horizontalLayout->count(); i++)
        horizontalLayout->setStretch(i, stretch[i]);

    verticalLayout->addLayout(horizontalLayout);
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName("keyboardLine4");

#ifdef Q_OS_LINUX
    mapKeyToButton.insert("LShift", new ActionButton("Shift", nullptr, parent, "", nullptr, 50));
#elif defined(Q_OS_WIN)
    mapKeyToButton.insert("LShift", new ActionButton("Shift", nullptr, parent, "", nullptr, 42));
#endif
    horizontalLayout->addWidget(mapKeyToButton["LShift"]);
    mapKeyToButton.insert("я", new ActionButton("Я", new Qt::Key(Qt::Key(1071)), parent, "я"));
    mapKeyToButton.insert("Я", mapKeyToButton["я"]);
    horizontalLayout->addWidget(mapKeyToButton["я"]);
    mapKeyToButton.insert("ч", new ActionButton("Ч", new Qt::Key(Qt::Key(1063)), parent, "ч"));
    mapKeyToButton.insert("Ч", mapKeyToButton["ч"]);
    horizontalLayout->addWidget(mapKeyToButton["ч"]);
    mapKeyToButton.insert("с", new ActionButton("С", new Qt::Key(Qt::Key(1057)), parent, "с"));
    mapKeyToButton.insert("С", mapKeyToButton["с"]);
    horizontalLayout->addWidget(mapKeyToButton["с"]);
    mapKeyToButton.insert("м", new ActionButton("М", new Qt::Key(Qt::Key(1052)), parent, "м"));
    mapKeyToButton.insert("М", mapKeyToButton["м"]);
    horizontalLayout->addWidget(mapKeyToButton["м"]);
    mapKeyToButton.insert("и", new ActionButton("И", new Qt::Key(Qt::Key(1048)), parent, "и"));
    mapKeyToButton.insert("И", mapKeyToButton["и"]);
    horizontalLayout->addWidget(mapKeyToButton["и"]);
    mapKeyToButton.insert("т", new ActionButton("Т", new Qt::Key(Qt::Key(1058)), parent, "т"));
    mapKeyToButton.insert("Т", mapKeyToButton["т"]);
    horizontalLayout->addWidget(mapKeyToButton["т"]);
    mapKeyToButton.insert("ь", new ActionButton("Ь", new Qt::Key(Qt::Key(1068)), parent, "ь"));
    mapKeyToButton.insert("Ь", mapKeyToButton["ь"]);
    horizontalLayout->addWidget(mapKeyToButton["ь"]);
    mapKeyToButton.insert("б", new ActionButton("Б", new Qt::Key(Qt::Key(1041)), parent, "б"));
    mapKeyToButton.insert("Б", mapKeyToButton["б"]);
    horizontalLayout->addWidget(mapKeyToButton["б"]);
    mapKeyToButton.insert("ю", new ActionButton("Ю", new Qt::Key(Qt::Key(1070)), parent, "ю"));
    mapKeyToButton.insert("Ю", mapKeyToButton["ю"]);
    horizontalLayout->addWidget(mapKeyToButton["ю"]);
    mapKeyToButton.insert(".", new ActionButton(",", new Qt::Key(Qt::Key_Period), parent, ".", new Qt::Key(Qt::Key_Comma)));
    mapKeyToButton.insert(",", mapKeyToButton["."]);
    horizontalLayout->addWidget(mapKeyToButton["."]);
#ifdef Q_OS_LINUX
    mapKeyToButton.insert("RShift", new ActionButton("Shift", nullptr, parent, "", nullptr, 62));
#elif defined(Q_OS_WIN)
    mapKeyToButton.insert("RShift", new ActionButton("Shift", nullptr, parent, "", nullptr, 54));
#endif
    horizontalLayout->addWidget(mapKeyToButton["RShift"]);

    stretch = {11, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 11};
    for (int i = 0; i < horizontalLayout->count(); i++)
        horizontalLayout->setStretch(i, stretch[i]);

    verticalLayout->addLayout(horizontalLayout);
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName("keyboardLine5");

#ifdef Q_OS_LINUX
    mapKeyToButton.insert("LCtrl", new ActionButton("Ctrl", nullptr, parent, "", nullptr, 37));
#elif defined(Q_OS_WIN)
    mapKeyToButton.insert("LCtrl", new ActionButton("Ctrl", nullptr, parent, "", nullptr, 29));
#endif
    horizontalLayout->addWidget(mapKeyToButton["LCtrl"]);
    mapKeyToButton.insert("Meta", new ActionButton("Meta", new Qt::Key(Qt::Key_Meta), parent, ""));
    horizontalLayout->addWidget(mapKeyToButton["Meta"]);
#ifdef Q_OS_LINUX
    mapKeyToButton.insert("LAlt", new ActionButton("Alt", nullptr, parent, "", nullptr, 64));
#elif defined(Q_OS_WIN)
    mapKeyToButton.insert("LAlt", new ActionButton("Alt", nullptr, parent, "", nullptr, 56));
#endif
    horizontalLayout->addWidget(mapKeyToButton["LAlt"]);
    mapKeyToButton.insert("Space", new ActionButton(" ", new Qt::Key(Qt::Key_Space), parent, ""));
    mapKeyToButton.insert(" ", mapKeyToButton["Space"]);
    horizontalLayout->addWidget(mapKeyToButton["Space"]);
#ifdef Q_OS_LINUX
    mapKeyToButton.insert("RAlt", new ActionButton("Alt", nullptr, parent, "", nullptr, 108));
#elif defined (Q_OS_WIN)
    mapKeyToButton.insert("RAlt", new ActionButton("Alt", nullptr, parent, "", nullptr, 57400));
#endif
    horizontalLayout->addWidget(mapKeyToButton["RAlt"]);
#ifdef Q_OS_LINUX
    mapKeyToButton.insert("RCtrl", new ActionButton("Ctrl", nullptr, parent, "", nullptr, 105));
#elif defined(Q_OS_WIN)
    mapKeyToButton.insert("RCtrl", new ActionButton("Ctrl", nullptr, parent, "", nullptr, 57373));
#endif
    horizontalLayout->addWidget(mapKeyToButton["RCtrl"]);

    stretch = {5, 5, 5, 30, 5, 5};
    for (int i = 0; i < horizontalLayout->count(); i++)
        horizontalLayout->setStretch(i, stretch[i]);

    verticalLayout->addLayout(horizontalLayout);
    if (parent) parent->installEventFilter(this);
    applyColumnColors();
}

void RussianKeyboardWidget::applyColumnColors()
{
    const KeyboardPalette palette = KeyboardPalette::loadOrCreateDefault();
    const QVector<QColor> colors = palette.normalColors();

    const auto pressedFromNormal = [](const QColor &normal) {
        QColor pressed = normal.lighter(120);
        pressed.setAlpha(255);
        return pressed;
    };

    // First apply default color to all keys (keys not in any group).
    const QColor defaultNormal = palette.defaultColor();
    const QColor defaultPressed = pressedFromNormal(defaultNormal);
    for (auto it = mapKeyToButton.begin(); it != mapKeyToButton.end(); ++it) {
        it.value()->setColors(defaultNormal.name(QColor::HexRgb), defaultPressed.name(QColor::HexRgb));
    }

    const auto applyGroup = [this](const QString &keys, const QColor &normalColor, const QColor &pressedColor) {
        for (const QChar &c : keys) {
            const QString key(c);
            if (!mapKeyToButton.contains(key))
                continue;
            mapKeyToButton[key]->setColors(normalColor.name(QColor::HexRgb), pressedColor.name(QColor::HexRgb));
        }
    };

    applyGroup("ё1йфя", colors[0], pressedFromNormal(colors[0]));
    applyGroup("2цыч", colors[1], pressedFromNormal(colors[1]));
    applyGroup("3увс", colors[2], pressedFromNormal(colors[2]));
    applyGroup("4кам5епи", colors[3], pressedFromNormal(colors[3]));
    applyGroup("6нрт7гоь", colors[4], pressedFromNormal(colors[4]));
    applyGroup("8шлб", colors[5], pressedFromNormal(colors[5]));
    applyGroup("9щдю", colors[6], pressedFromNormal(colors[6]));
    applyGroup("0зж.-хэ=ъ\\", colors[7], pressedFromNormal(colors[7]));
}

const QMap<QChar, QStringList> RussianKeyboardWidget::mapCharToKeys = []() {
    QMap<QChar, QStringList> map;

    map.insert(QChar(0x0430), QStringList{"а"});
    map.insert(QChar(0x0431), QStringList{"б"});
    map.insert(QChar(0x0432), QStringList{"в"});
    map.insert(QChar(0x0433), QStringList{"г"});
    map.insert(QChar(0x0434), QStringList{"д"});
    map.insert(QChar(0x0435), QStringList{"е"});
    map.insert(QChar(0x0451), QStringList{"ё"});
    map.insert(QChar(0x0436), QStringList{"ж"});
    map.insert(QChar(0x0437), QStringList{"з"});
    map.insert(QChar(0x0438), QStringList{"и"});
    map.insert(QChar(0x0439), QStringList{"й"});
    map.insert(QChar(0x043a), QStringList{"к"});
    map.insert(QChar(0x043b), QStringList{"л"});
    map.insert(QChar(0x043c), QStringList{"м"});
    map.insert(QChar(0x043d), QStringList{"н"});
    map.insert(QChar(0x043e), QStringList{"о"});
    map.insert(QChar(0x043f), QStringList{"п"});
    map.insert(QChar(0x0440), QStringList{"р"});
    map.insert(QChar(0x0441), QStringList{"с"});
    map.insert(QChar(0x0442), QStringList{"т"});
    map.insert(QChar(0x0443), QStringList{"у"});
    map.insert(QChar(0x0444), QStringList{"ф"});
    map.insert(QChar(0x0445), QStringList{"х"});
    map.insert(QChar(0x0446), QStringList{"ц"});
    map.insert(QChar(0x0447), QStringList{"ч"});
    map.insert(QChar(0x0448), QStringList{"ш"});
    map.insert(QChar(0x0449), QStringList{"щ"});
    map.insert(QChar(0x044a), QStringList{"ъ"});
    map.insert(QChar(0x044b), QStringList{"ы"});
    map.insert(QChar(0x044c), QStringList{"ь"});
    map.insert(QChar(0x044d), QStringList{"э"});
    map.insert(QChar(0x044e), QStringList{"ю"});
    map.insert(QChar(0x044f), QStringList{"я"});

    map.insert(QChar(0x0410), QStringList{"RShift", "а"});
    map.insert(QChar(0x0411), QStringList{"LShift", "б"});
    map.insert(QChar(0x0412), QStringList{"RShift", "в"});
    map.insert(QChar(0x0413), QStringList{"LShift", "г"});
    map.insert(QChar(0x0414), QStringList{"LShift", "д"});
    map.insert(QChar(0x0415), QStringList{"RShift", "е"});
    map.insert(QChar(0x0401), QStringList{"RShift", "ё"});
    map.insert(QChar(0x0416), QStringList{"LShift", "ж"});
    map.insert(QChar(0x0417), QStringList{"LShift", "з"});
    map.insert(QChar(0x0418), QStringList{"RShift", "и"});
    map.insert(QChar(0x0419), QStringList{"RShift", "й"});
    map.insert(QChar(0x041a), QStringList{"RShift", "к"});
    map.insert(QChar(0x041b), QStringList{"LShift", "л"});
    map.insert(QChar(0x041c), QStringList{"RShift", "м"});
    map.insert(QChar(0x041d), QStringList{"LShift", "н"});
    map.insert(QChar(0x041e), QStringList{"LShift", "о"});
    map.insert(QChar(0x041f), QStringList{"RShift", "п"});
    map.insert(QChar(0x0420), QStringList{"LShift", "р"});
    map.insert(QChar(0x0421), QStringList{"RShift", "с"});
    map.insert(QChar(0x0422), QStringList{"LShift", "т"});
    map.insert(QChar(0x0423), QStringList{"RShift", "у"});
    map.insert(QChar(0x0424), QStringList{"RShift", "ф"});
    map.insert(QChar(0x0425), QStringList{"LShift", "х"});
    map.insert(QChar(0x0426), QStringList{"RShift", "ц"});
    map.insert(QChar(0x0427), QStringList{"RShift", "ч"});
    map.insert(QChar(0x0428), QStringList{"LShift", "ш"});
    map.insert(QChar(0x0429), QStringList{"LShift", "щ"});
    map.insert(QChar(0x042a), QStringList{"LShift", "ъ"});
    map.insert(QChar(0x042b), QStringList{"RShift", "ы"});
    map.insert(QChar(0x042c), QStringList{"LShift", "ь"});
    map.insert(QChar(0x042d), QStringList{"LShift", "э"});
    map.insert(QChar(0x042e), QStringList{"LShift", "ю"});
    map.insert(QChar(0x042f), QStringList{"RShift", "я"});

    map.insert('1', QStringList{"1"});
    map.insert('!', QStringList{"RShift", "1"});
    map.insert('2', QStringList{"2"});
    map.insert('\"', QStringList{"RShift", "2"});
    map.insert('3', QStringList{"3"});
    map.insert(QChar(0x2116), QStringList{"RShift", "3"});
    map.insert('4', QStringList{"4"});
    map.insert(';', QStringList{"RShift", "4"});
    map.insert('5', QStringList{"5"});
    map.insert('%', QStringList{"RShift", "5"});
    map.insert('6', QStringList{"6"});
    map.insert(':', QStringList{"LShift", "6"});
    map.insert('7', QStringList{"7"});
    map.insert('?', QStringList{"LShift", "7"});
    map.insert('8', QStringList{"8"});
    map.insert('*', QStringList{"LShift", "8"});
    map.insert('9', QStringList{"9"});
    map.insert('(', QStringList{"LShift", "9"});
    map.insert('0', QStringList{"0"});
    map.insert(')', QStringList{"LShift", "0"});
    map.insert('-', QStringList{"-"});
    map.insert('_', QStringList{"LShift", "-"});
    map.insert('=', QStringList{"="});
    map.insert('+', QStringList{"LShift", "="});

    map.insert('\\', QStringList{"\\"});
    map.insert('/', QStringList{"LShift", "\\"});

    map.insert('.', QStringList{"."});
    map.insert(',', QStringList{"LShift", "."});

    map.insert(QChar(0x00B6), QStringList{"Enter"});
    map.insert('\n', map[QChar(0x0086)]);
    map.insert(' ', QStringList{"Space"});
    map.insert('\t', QStringList{"Tab"});

    return map;
}();


bool RussianKeyboardWidget::eventFilter(QObject* obj, QEvent* event)
{
    return QWidget::eventFilter(obj, event);
}

void RussianKeyboardWidget::highlightChar(QChar c) {
    if (mapCharToKeys.contains(c))
    {
        QStringList sl = mapCharToKeys[c];
        foreach (auto iter, sl)
        {
            ActionButton *button = mapKeyToButton[iter];

            if (button)
                button->setHighlighted();
        }
    }
};

void RussianKeyboardWidget::unhighlightChar(QChar c) {
    if (mapCharToKeys.contains(c))
    {
        QStringList sl = mapCharToKeys[c];
        foreach (auto iter, sl)
        {
            ActionButton *button = mapKeyToButton[iter];

            if (button)
                button->unsetHighlighted();
        }
    }
};

bool RussianKeyboardWidget::isCharIn(QChar c) {
    return mapCharToKeys.contains(c);
}
