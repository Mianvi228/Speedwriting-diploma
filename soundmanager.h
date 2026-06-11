#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QSoundEffect>

class SoundManager
{
public:
    SoundManager();

    void playClick();

    bool clickSoundsEnabled() const { return m_clickSoundsEnabled; }
    qreal volume() const { return m_volume; }

    void setClickSoundsEnabled(bool enabled);
    void setVolume(qreal volume);

    void loadSettings();
    void saveSettings() const;
    void resetToDefaults();

private:
    void applyVolume();

    QSoundEffect clickSound;
    bool m_clickSoundsEnabled = true;
    qreal m_volume = 1.0;

    static QString configPath();
};

#endif // SOUNDMANAGER_H
