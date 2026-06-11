#ifndef SOUNDSETTINGSWIDGET_H
#define SOUNDSETTINGSWIDGET_H

#include <QWidget>

class QCheckBox;
class QLabel;
class QSlider;
class SoundManager;

class SoundSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SoundSettingsWidget(SoundManager *soundManager, QWidget *parent = nullptr);

    bool hasUnsavedChanges() const;
    void applyChanges();
    void discardChanges();

private slots:
    void onClickSoundsToggled(bool enabled);
    void onVolumeChanged(int value);
    void onSaveClicked();
    void onResetClicked();

private:
    void refreshControls();

    SoundManager *soundManager = nullptr;
    QCheckBox *clickSoundsCheckBox = nullptr;
    QSlider *volumeSlider = nullptr;
    QLabel *volumeValueLabel = nullptr;
};

#endif // SOUNDSETTINGSWIDGET_H
