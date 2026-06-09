#include "keyboardpalette.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>

KeyboardPalette KeyboardPalette::loadOrCreateDefault()
{
    const QString path = configPath();
    QFile f(path);
    if (f.exists() && f.open(QIODevice::ReadOnly)) {
        const QByteArray data = f.readAll();
        f.close();

        const QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isObject()) {
            const QJsonObject obj = doc.object();
            const QJsonArray colors = obj.value("normalColors").toArray();

            KeyboardPalette palette;
            palette.m_normalColors.clear();
            for (const QJsonValue &v : colors) {
                const QString hex = v.toString();
                QColor c(hex);
                if (c.isValid())
                    palette.m_normalColors.push_back(c);
            }

            const QString defaultHex = obj.value("defaultColor").toString("#3a6ea5");
            QColor def(defaultHex);
            palette.m_defaultColor = def.isValid() ? def : QColor("#3a6ea5");
            palette.m_keyboardVisibleDuringExercise = obj.value("keyboardVisibleDuringExercise").toBool(true);

            if (palette.m_normalColors.size() == GroupCount)
                return palette;
        }
    }

    KeyboardPalette def = defaults();
    def.save();
    return def;
}

KeyboardPalette KeyboardPalette::defaultPalette()
{
    return defaults();
}

void KeyboardPalette::save() const
{
    const QString path = configPath();
    QDir().mkpath(QFileInfo(path).absolutePath());

    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;

    QJsonObject obj;
    QJsonArray colors;
    for (const QColor &c : m_normalColors) {
        colors.push_back(c.name(QColor::HexRgb));
    }
    obj["normalColors"] = colors;
    obj["defaultColor"] = m_defaultColor.isValid() ? m_defaultColor.name(QColor::HexRgb) : QString("#3a6ea5");
    obj["keyboardVisibleDuringExercise"] = m_keyboardVisibleDuringExercise;
    obj["version"] = 1;

    f.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
    f.close();
}

void KeyboardPalette::setNormalColor(int groupIndex, const QColor &color)
{
    if (groupIndex < 0 || groupIndex >= GroupCount)
        return;
    if (m_normalColors.size() != GroupCount)
        m_normalColors = defaults().m_normalColors;
    m_normalColors[groupIndex] = color;
}

QString KeyboardPalette::configPath()
{
    const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (dir.isEmpty())
        return QString();
    return dir + "/keyboard_palette.json";
}

KeyboardPalette KeyboardPalette::defaults()
{
    KeyboardPalette palette;
    palette.m_normalColors = {
        QColor("#3b82f6"), // 1qaz
        QColor("#8b5cf6"), // 2wsx
        QColor("#06b6d4"), // 3edc
        QColor("#14b8a6"), // 4rfv5tgb
        QColor("#22c55e"), // 6yhn7ujm
        QColor("#eab308"), // 8ik,
        QColor("#f97316"), // 9ol.
        QColor("#ef4444")  // 0p;/-['=] and backslash key
    };
    palette.m_defaultColor = QColor("#3a6ea5");
    palette.m_keyboardVisibleDuringExercise = true;
    return palette;
}

