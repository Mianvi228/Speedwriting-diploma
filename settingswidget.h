#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QMap>
#include <QStringList>

class QListWidget;
class QStackedWidget;
class PaletteSettingsWidget;
class SoundManager;
class SoundSettingsWidget;

class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(SoundManager *soundManager, QWidget *parent = nullptr);

signals:
    void done();

private slots:
    void onSettingChanged(int row);
    void onBackToMenu();

private:
    struct SettingInfo {
        QString id;
        QString title;
    };

    QWidget *settingsPanelForId(const QString &settingId);
    void showPlaceholder();
    void showSettingPanel(const QString &settingId);
    bool hasUnsavedChangesForSetting(const QString &settingId) const;
    void saveSetting(const QString &settingId);
    void discardSetting(const QString &settingId);
    void promptSaveChangesForSetting(const QString &settingId);

    QListWidget *settingsList = nullptr;
    QStackedWidget *settingsStack = nullptr;

    QMap<QString, SettingInfo> settingInfoById;
    QMap<QString, QWidget *> settingPanels;
    PaletteSettingsWidget *paletteSettingsWidget = nullptr;
    SoundSettingsWidget *soundSettingsWidget = nullptr;
    SoundManager *soundManager = nullptr;
    QString activeSettingId;
};

#endif // SETTINGSWIDGET_H
