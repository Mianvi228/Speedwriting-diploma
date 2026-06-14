#include "englishkeyboardwidget.h"

#include "keyboardpalette.h"

EnglishKeyboardWidget::EnglishKeyboardWidget(QWidget *parent) {
    ActionButton *button = nullptr;

    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    verticalLayout->setObjectName("keyboardLayout");

    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName("keyboardLine1");

    std::array<int, 14> stretch;

    mapKeyToButton.insert("`", new ActionButton("`", new Qt::Key(Qt::Key_QuoteLeft), parent, "~", new Qt::Key(Qt::Key_AsciiTilde)));
    mapKeyToButton.insert("~", mapKeyToButton["`"]);
    horizontalLayout->addWidget(mapKeyToButton["`"]);
    mapKeyToButton.insert("1", new ActionButton("1", new Qt::Key(Qt::Key_1), parent, "!", new Qt::Key(Qt::Key_Exclam)));
    mapKeyToButton.insert("!", mapKeyToButton["1"]);
    horizontalLayout->addWidget(mapKeyToButton["1"]);
    mapKeyToButton.insert("2", new ActionButton("2", new Qt::Key(Qt::Key_2), parent, "@", new Qt::Key(Qt::Key_At)));
    mapKeyToButton.insert("@", mapKeyToButton["2"]);
    horizontalLayout->addWidget(mapKeyToButton["2"]);
    mapKeyToButton.insert("3", new ActionButton("3", new Qt::Key(Qt::Key_3), parent, "#", new Qt::Key(Qt::Key_NumberSign)));
    mapKeyToButton.insert("#", mapKeyToButton["3"]);
    horizontalLayout->addWidget(mapKeyToButton["3"]);
    mapKeyToButton.insert("4", new ActionButton("4", new Qt::Key(Qt::Key_4), parent, "$", new Qt::Key(Qt::Key_Dollar)));
    mapKeyToButton.insert("$", mapKeyToButton["4"]);
    horizontalLayout->addWidget(mapKeyToButton["4"]);
    mapKeyToButton.insert("5", new ActionButton("5", new Qt::Key(Qt::Key_5), parent, "%", new Qt::Key(Qt::Key_Percent)));
    mapKeyToButton.insert("%", mapKeyToButton["5"]);
    horizontalLayout->addWidget(mapKeyToButton["5"]);
    mapKeyToButton.insert("6", new ActionButton("6", new Qt::Key(Qt::Key_6), parent, "^", new Qt::Key(Qt::Key_AsciiCircum)));
    mapKeyToButton.insert("^", mapKeyToButton["6"]);
    horizontalLayout->addWidget(mapKeyToButton["6"]);
    mapKeyToButton.insert("7", new ActionButton("7", new Qt::Key(Qt::Key_7), parent, "&", new Qt::Key(Qt::Key_Ampersand)));
    mapKeyToButton.insert("&", mapKeyToButton["7"]);
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
    mapKeyToButton.insert("q", new ActionButton("Q", new Qt::Key(Qt::Key_Q), parent, "q"));
    mapKeyToButton.insert("Q", mapKeyToButton["q"]);
    horizontalLayout->addWidget(mapKeyToButton["q"]);
    mapKeyToButton.insert("w", new ActionButton("W", new Qt::Key(Qt::Key_W), parent, "w"));
    mapKeyToButton.insert("W", mapKeyToButton["w"]);
    horizontalLayout->addWidget(mapKeyToButton["w"]);
    mapKeyToButton.insert("e", new ActionButton("E", new Qt::Key(Qt::Key_E), parent, "e"));
    mapKeyToButton.insert("E", mapKeyToButton["e"]);
    horizontalLayout->addWidget(mapKeyToButton["e"]);
    mapKeyToButton.insert("r", new ActionButton("R", new Qt::Key(Qt::Key_R), parent, "r"));
    mapKeyToButton.insert("R", mapKeyToButton["r"]);
    horizontalLayout->addWidget(mapKeyToButton["r"]);
    mapKeyToButton.insert("t", new ActionButton("T", new Qt::Key(Qt::Key_T), parent, "t"));
    mapKeyToButton.insert("T", mapKeyToButton["t"]);
    horizontalLayout->addWidget(mapKeyToButton["t"]);
    mapKeyToButton.insert("y", new ActionButton("Y", new Qt::Key(Qt::Key_Y), parent, "y"));
    mapKeyToButton.insert("Y", mapKeyToButton["y"]);
    horizontalLayout->addWidget(mapKeyToButton["y"]);
    mapKeyToButton.insert("u", new ActionButton("U", new Qt::Key(Qt::Key_U), parent, "u"));
    mapKeyToButton.insert("U", mapKeyToButton["u"]);
    horizontalLayout->addWidget(mapKeyToButton["u"]);
    mapKeyToButton.insert("i", new ActionButton("I", new Qt::Key(Qt::Key_I), parent, "i"));
    mapKeyToButton.insert("I", mapKeyToButton["i"]);
    horizontalLayout->addWidget(mapKeyToButton["i"]);
    mapKeyToButton.insert("o", new ActionButton("O", new Qt::Key(Qt::Key_O), parent, "o"));
    mapKeyToButton.insert("O", mapKeyToButton["o"]);
    horizontalLayout->addWidget(mapKeyToButton["o"]);
    mapKeyToButton.insert("p", new ActionButton("P", new Qt::Key(Qt::Key_P), parent, "p"));
    mapKeyToButton.insert("P", mapKeyToButton["p"]);
    horizontalLayout->addWidget(mapKeyToButton["p"]);
    mapKeyToButton.insert("[", new ActionButton("[", new Qt::Key(Qt::Key_BracketLeft), parent, "{", new Qt::Key(Qt::Key_BraceLeft)));
    mapKeyToButton.insert("{", mapKeyToButton["["]);
    horizontalLayout->addWidget(mapKeyToButton["["]);
    mapKeyToButton.insert("]", new ActionButton("]", new Qt::Key(Qt::Key_BracketRight), parent, "}", new Qt::Key(Qt::Key_BraceRight)));
    mapKeyToButton.insert("}", mapKeyToButton["]"]);
    horizontalLayout->addWidget(mapKeyToButton["]"]);
    mapKeyToButton.insert("\\", new ActionButton("\\", new Qt::Key(Qt::Key_Backslash), parent, "|", new Qt::Key(Qt::Key_Bar)));
    mapKeyToButton.insert("|", mapKeyToButton["\\"]);
    horizontalLayout->addWidget(mapKeyToButton["\\"]);

    stretch = {6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6};
    for (int i = 0; i < horizontalLayout->count(); i++)
        horizontalLayout->setStretch(i, stretch[i]);

    verticalLayout->addLayout(horizontalLayout);
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName("keyboardLine3");

    mapKeyToButton.insert("CapsLock", new ActionButton("CapsLock", new Qt::Key(Qt::Key_CapsLock), parent, ""));
    horizontalLayout->addWidget(mapKeyToButton["CapsLock"]);
    mapKeyToButton.insert("a", new ActionButton("A", new Qt::Key(Qt::Key_A), parent, "a"));
    mapKeyToButton.insert("A", mapKeyToButton["a"]);
    horizontalLayout->addWidget(mapKeyToButton["a"]);
    mapKeyToButton.insert("s", new ActionButton("S", new Qt::Key(Qt::Key_S), parent, "s"));
    mapKeyToButton.insert("S", mapKeyToButton["s"]);
    horizontalLayout->addWidget(mapKeyToButton["s"]);
    mapKeyToButton.insert("d", new ActionButton("D", new Qt::Key(Qt::Key_D), parent, "d"));
    mapKeyToButton.insert("D", mapKeyToButton["d"]);
    horizontalLayout->addWidget(mapKeyToButton["d"]);
    mapKeyToButton.insert("f", new ActionButton("F", new Qt::Key(Qt::Key_F), parent, "f"));
    mapKeyToButton.insert("F", mapKeyToButton["f"]);
    horizontalLayout->addWidget(mapKeyToButton["f"]);
    mapKeyToButton.insert("g", new ActionButton("G", new Qt::Key(Qt::Key_G), parent, "g"));
    mapKeyToButton.insert("G", mapKeyToButton["g"]);
    horizontalLayout->addWidget(mapKeyToButton["g"]);
    mapKeyToButton.insert("h", new ActionButton("H", new Qt::Key(Qt::Key_H), parent, "h"));
    mapKeyToButton.insert("H", mapKeyToButton["h"]);
    horizontalLayout->addWidget(mapKeyToButton["h"]);
    mapKeyToButton.insert("j", new ActionButton("J", new Qt::Key(Qt::Key_J), parent, "j"));
    mapKeyToButton.insert("J", mapKeyToButton["j"]);
    horizontalLayout->addWidget(mapKeyToButton["j"]);
    mapKeyToButton.insert("k", new ActionButton("K", new Qt::Key(Qt::Key_K), parent, "k"));
    mapKeyToButton.insert("K", mapKeyToButton["k"]);
    horizontalLayout->addWidget(mapKeyToButton["k"]);
    mapKeyToButton.insert("l", new ActionButton("L", new Qt::Key(Qt::Key_L), parent, "l"));
    mapKeyToButton.insert("L", mapKeyToButton["l"]);
    horizontalLayout->addWidget(mapKeyToButton["l"]);
    mapKeyToButton.insert(";", new ActionButton(";", new Qt::Key(Qt::Key_Semicolon), parent, ":", new Qt::Key(Qt::Key_Colon)));
    mapKeyToButton.insert(":", mapKeyToButton[";"]);
    horizontalLayout->addWidget(mapKeyToButton[";"]);
    mapKeyToButton.insert("'", new ActionButton("'", new Qt::Key(Qt::Key_Apostrophe), parent, "\"", new Qt::Key(Qt::Key_QuoteDbl)));
    mapKeyToButton.insert("\"", mapKeyToButton["'"]);
    horizontalLayout->addWidget(mapKeyToButton["'"]);
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
    mapKeyToButton.insert("z", new ActionButton("Z", new Qt::Key(Qt::Key_Z), parent, "z"));
    mapKeyToButton.insert("Z", mapKeyToButton["z"]);
    horizontalLayout->addWidget(mapKeyToButton["z"]);
    mapKeyToButton.insert("x", new ActionButton("X", new Qt::Key(Qt::Key_X), parent, "x"));
    mapKeyToButton.insert("X", mapKeyToButton["x"]);
    horizontalLayout->addWidget(mapKeyToButton["x"]);
    mapKeyToButton.insert("c", new ActionButton("C", new Qt::Key(Qt::Key_C), parent, "c"));
    mapKeyToButton.insert("C", mapKeyToButton["c"]);
    horizontalLayout->addWidget(mapKeyToButton["c"]);
    mapKeyToButton.insert("v", new ActionButton("V", new Qt::Key(Qt::Key_V), parent, "v"));
    mapKeyToButton.insert("V", mapKeyToButton["v"]);
    horizontalLayout->addWidget(mapKeyToButton["v"]);
    mapKeyToButton.insert("b", new ActionButton("B", new Qt::Key(Qt::Key_B), parent, "b"));
    mapKeyToButton.insert("B", mapKeyToButton["b"]);
    horizontalLayout->addWidget(mapKeyToButton["b"]);
    mapKeyToButton.insert("n", new ActionButton("N", new Qt::Key(Qt::Key_N), parent, "n"));
    mapKeyToButton.insert("N", mapKeyToButton["n"]);
    horizontalLayout->addWidget(mapKeyToButton["n"]);
    mapKeyToButton.insert("m", new ActionButton("M", new Qt::Key(Qt::Key_M), parent, "m"));
    mapKeyToButton.insert("M", mapKeyToButton["m"]);
    horizontalLayout->addWidget(mapKeyToButton["m"]);
    mapKeyToButton.insert(",", new ActionButton(",", new Qt::Key(Qt::Key_Comma), parent, "<", new Qt::Key(Qt::Key_Less)));
    mapKeyToButton.insert("<", mapKeyToButton[","]);
    horizontalLayout->addWidget(mapKeyToButton[","]);
    mapKeyToButton.insert(".", new ActionButton(".", new Qt::Key(Qt::Key_Period), parent, ">", new Qt::Key(Qt::Key_Greater)));
    mapKeyToButton.insert(">", mapKeyToButton["."]);
    horizontalLayout->addWidget(mapKeyToButton["."]);
    mapKeyToButton.insert("/", new ActionButton("/", new Qt::Key(Qt::Key_Slash), parent, "?", new Qt::Key(Qt::Key_Question)));
    mapKeyToButton.insert("?", mapKeyToButton["/"]);
    horizontalLayout->addWidget(mapKeyToButton["/"]);
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

void EnglishKeyboardWidget::applyColumnColors()
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

    applyGroup("`1qaz", colors[0], pressedFromNormal(colors[0]));
    applyGroup("2wsx", colors[1], pressedFromNormal(colors[1]));
    applyGroup("3edc", colors[2], pressedFromNormal(colors[2]));
    applyGroup("4rfv5tgb", colors[3], pressedFromNormal(colors[3]));
    applyGroup("6yhn7ujm", colors[4], pressedFromNormal(colors[4]));
    applyGroup("8ik,", colors[5], pressedFromNormal(colors[5]));
    applyGroup("9ol.", colors[6], pressedFromNormal(colors[6]));
    applyGroup("0p;/-['=]\\", colors[7], pressedFromNormal(colors[7]));
}

const QMap<QChar, QStringList> EnglishKeyboardWidget::mapCharToKeys = []() {
    QMap<QChar, QStringList> map;

    map.insert('a', QStringList{"a"});
    map.insert('b', QStringList{"b"});
    map.insert('c', QStringList{"c"});
    map.insert('d', QStringList{"d"});
    map.insert('e', QStringList{"e"});
    map.insert('f', QStringList{"f"});
    map.insert('g', QStringList{"g"});
    map.insert('h', QStringList{"h"});
    map.insert('i', QStringList{"i"});
    map.insert('j', QStringList{"j"});
    map.insert('k', QStringList{"k"});
    map.insert('l', QStringList{"l"});
    map.insert('m', QStringList{"m"});
    map.insert('n', QStringList{"n"});
    map.insert('o', QStringList{"o"});
    map.insert('p', QStringList{"p"});
    map.insert('q', QStringList{"q"});
    map.insert('r', QStringList{"r"});
    map.insert('s', QStringList{"s"});
    map.insert('t', QStringList{"t"});
    map.insert('u', QStringList{"u"});
    map.insert('v', QStringList{"v"});
    map.insert('w', QStringList{"w"});
    map.insert('x', QStringList{"x"});
    map.insert('y', QStringList{"y"});
    map.insert('z', QStringList{"z"});

    map.insert('Q', QStringList{"RShift", "q"});
    map.insert('W', QStringList{"RShift", "w"});
    map.insert('E', QStringList{"RShift", "e"});
    map.insert('R', QStringList{"RShift", "r"});
    map.insert('T', QStringList{"RShift", "t"});
    map.insert('A', QStringList{"RShift", "a"});
    map.insert('S', QStringList{"RShift", "s"});
    map.insert('D', QStringList{"RShift", "d"});
    map.insert('F', QStringList{"RShift", "f"});
    map.insert('G', QStringList{"RShift", "g"});
    map.insert('Z', QStringList{"RShift", "z"});
    map.insert('X', QStringList{"RShift", "x"});
    map.insert('C', QStringList{"RShift", "c"});
    map.insert('V', QStringList{"RShift", "v"});
    map.insert('B', QStringList{"RShift", "b"});

    map.insert('Y', QStringList{"LShift", "y"});
    map.insert('U', QStringList{"LShift", "u"});
    map.insert('I', QStringList{"LShift", "i"});
    map.insert('O', QStringList{"LShift", "o"});
    map.insert('P', QStringList{"LShift", "p"});
    map.insert('H', QStringList{"LShift", "h"});
    map.insert('J', QStringList{"LShift", "j"});
    map.insert('K', QStringList{"LShift", "k"});
    map.insert('L', QStringList{"LShift", "l"});
    map.insert('M', QStringList{"LShift", "m"});
    map.insert('N', QStringList{"LShift", "n"});

    map.insert('`', QStringList{"`"});
    map.insert('~', QStringList{"RShift", "`"});
    map.insert('1', QStringList{"1"});
    map.insert('!', QStringList{"RShift", "1"});
    map.insert('2', QStringList{"2"});
    map.insert('@', QStringList{"RShift", "2"});
    map.insert('3', QStringList{"3"});
    map.insert('#', QStringList{"RShift", "3"});
    map.insert('4', QStringList{"4"});
    map.insert('$', QStringList{"RShift", "4"});
    map.insert('5', QStringList{"5"});
    map.insert('%', QStringList{"RShift", "5"});
    map.insert('6', QStringList{"6"});
    map.insert('^', QStringList{"LShift", "6"});
    map.insert('7', QStringList{"7"});
    map.insert('&', QStringList{"LShift", "7"});
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

    map.insert('[', QStringList{"["});
    map.insert('{', QStringList{"LShift", "["});
    map.insert(']', QStringList{"]"});
    map.insert('}', QStringList{"LShift", "]"});
    map.insert('\\', QStringList{"\\"});
    map.insert('|', QStringList{"LShift", "\\"});

    map.insert(';', QStringList{";"});
    map.insert(':', QStringList{"LShift", ";"});
    map.insert('\'', QStringList{"'"});
    map.insert('"', QStringList{"LShift", "'"});

    map.insert(',', QStringList{","});
    map.insert('<', QStringList{"LShift", ","});
    map.insert('.', QStringList{"."});
    map.insert('>', QStringList{"LShift", "."});
    map.insert('/', QStringList{"/"});
    map.insert('?', QStringList{"LShift", "/"});

    map.insert(QChar(0x00B6), QStringList{"Enter"});
    map.insert('\n', map[QChar(0x0086)]);
    map.insert('\r', map[QChar(0x0086)]);
    map.insert(' ', QStringList{"Space"});
    map.insert('\t', QStringList{"Tab"});

    return map;
}();

bool EnglishKeyboardWidget::eventFilter(QObject* obj, QEvent* event)
{
    return QWidget::eventFilter(obj, event);
}

void EnglishKeyboardWidget::highlightChar(QChar c) {
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

void EnglishKeyboardWidget::unhighlightChar(QChar c) {
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

bool EnglishKeyboardWidget::isCharIn(QChar c) {
    return mapCharToKeys.contains(c);
}
