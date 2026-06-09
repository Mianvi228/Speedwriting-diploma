#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QMap>
#include <QStringList>

class QListWidget;
class QStackedWidget;
class PaletteSettingsWidget;

class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget *parent = nullptr);

signals:
    void done();

private slots:
    void onSettingChanged();

private:
    struct SettingInfo {
        QString id;
        QString title;
    };

    QWidget *settingsPanelForId(const QString &settingId);
    void showPlaceholder();

    QListWidget *settingsList = nullptr;
    QStackedWidget *settingsStack = nullptr;

    QMap<QString, SettingInfo> settingInfoById;
    QMap<QString, QWidget *> settingPanels;
    PaletteSettingsWidget *paletteSettingsWidget = nullptr;
};

#endif // SETTINGSWIDGET_H
