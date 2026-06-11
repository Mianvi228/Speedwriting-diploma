#include "soundmanager.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QtMath>

SoundManager::SoundManager()
{
    clickSound.setSource(QUrl(QStringLiteral("qrc:/sound/key_click.wav")));
    loadSettings();
    applyVolume();
}

void SoundManager::playClick()
{
    if (!m_clickSoundsEnabled)
        return;

    clickSound.play();
}

void SoundManager::setClickSoundsEnabled(bool enabled)
{
    m_clickSoundsEnabled = enabled;
}

void SoundManager::setVolume(qreal volume)
{
    m_volume = qBound(0.0, volume, 1.0);
    applyVolume();
}

void SoundManager::applyVolume()
{
    clickSound.setVolume(static_cast<float>(m_volume));
}

void SoundManager::loadSettings()
{
    const QString path = configPath();
    if (path.isEmpty())
        return;

    QFile file(path);
    if (!file.exists() || !file.open(QIODevice::ReadOnly))
        return;

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (!doc.isObject())
        return;

    const QJsonObject obj = doc.object();
    m_clickSoundsEnabled = obj.value(QStringLiteral("clickSoundsEnabled")).toBool(true);
    m_volume = qBound(0.0, obj.value(QStringLiteral("volume")).toDouble(1.0), 1.0);
}

void SoundManager::saveSettings() const
{
    const QString path = configPath();
    if (path.isEmpty())
        return;

    QDir().mkpath(QFileInfo(path).absolutePath());

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;

    QJsonObject obj;
    obj[QStringLiteral("clickSoundsEnabled")] = m_clickSoundsEnabled;
    obj[QStringLiteral("volume")] = m_volume;
    obj[QStringLiteral("version")] = 1;

    file.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
    file.close();
}

void SoundManager::resetToDefaults()
{
    m_clickSoundsEnabled = true;
    m_volume = 1.0;
    applyVolume();
}

QString SoundManager::configPath()
{
    const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (dir.isEmpty())
        return QString();
    return dir + QStringLiteral("/sound_settings.json");
}
