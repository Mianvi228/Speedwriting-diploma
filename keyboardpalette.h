#ifndef KEYBOARDPALETTE_H
#define KEYBOARDPALETTE_H

#include <QColor>
#include <QVector>
#include <QString>

class KeyboardPalette
{
public:
    static constexpr int GroupCount = 8;

    // Colors are "normal" colors. "pressed" will be derived in keyboard rendering.
    QVector<QColor> normalColors() const { return m_normalColors; }
    QColor defaultColor() const { return m_defaultColor; }
    bool keyboardVisibleDuringExercise() const { return m_keyboardVisibleDuringExercise; }

    static KeyboardPalette loadOrCreateDefault();
    static KeyboardPalette defaultPalette();
    void save() const;

    void setNormalColor(int groupIndex, const QColor &color);
    void setDefaultColor(const QColor &color) { m_defaultColor = color; }
    void setKeyboardVisibleDuringExercise(bool visible) { m_keyboardVisibleDuringExercise = visible; }

private:
    QVector<QColor> m_normalColors;
    QColor m_defaultColor = QColor("#3a6ea5");
    bool m_keyboardVisibleDuringExercise = true;

    static QString configPath();
    static KeyboardPalette defaults();
};

#endif // KEYBOARDPALETTE_H

