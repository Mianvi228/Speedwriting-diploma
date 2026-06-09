#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPointF>

class QScrollBar;

class HistoryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryWidget(QWidget *parent = nullptr, const QString &exerciseIdFilter = QString());

    void reloadSessions();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    struct PlotPoint {
        QPointF pos;
        QString label;
        double value;
        QString exerciseId;
        QString dateTime;
        int durationSeconds;
    };

    QString exerciseIdFilter;
    QVector<double> kpmSeries;
    QVector<double> accuracySeries;
    QVector<QString> exerciseIdSeries;
    QVector<QString> dateTimeSeries;
    QVector<int> durationSeries;
    QScrollBar *sessionsScrollBar = nullptr;
    int startSessionIndex = 0;
    const int visibleSessionsCount = 25;
    QVector<PlotPoint> kpmPoints;
    QVector<PlotPoint> accuracyPoints;
    bool hasHoveredPoint = false;
    PlotPoint hoveredPoint;

    void loadSessions();
    void updateScrollBounds();
    void updateHoveredPoint(const QPointF &cursorPos);
};

#endif // HISTORYWIDGET_H
