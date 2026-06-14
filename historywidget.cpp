#include "historywidget.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QScrollBar>
#include <QStandardPaths>
#include <QMouseEvent>
#include <QtMath>
#include <algorithm>
#include <limits>
#include "utils.h"

HistoryWidget::HistoryWidget(QWidget *parent, const QString &exerciseIdFilter)
    : QWidget(parent)
    , exerciseIdFilter(exerciseIdFilter)
{
    setMouseTracking(true);
    sessionsScrollBar = new QScrollBar(Qt::Horizontal, this);
    connect(sessionsScrollBar, &QScrollBar::valueChanged, this, [this](int value) {
        startSessionIndex = value;
        update();
    });
    loadSessions();
    updateScrollBounds();
}

namespace {
    const QString kFreeTypingEnId = QStringLiteral("Free typing en");
}

void HistoryWidget::loadSessions()
{
    kpmSeries.clear();
    accuracySeries.clear();

    const QString historyDirPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (historyDirPath.isEmpty())
        return;

    QFile historyFile(historyDirPath + "/completed_sessions.json");
    if (!historyFile.open(QIODevice::ReadOnly))
        return;

    const QJsonDocument doc = QJsonDocument::fromJson(historyFile.readAll());
    historyFile.close();

    if (!doc.isArray())
        return;

    const QJsonArray sessions = doc.array();
    for (const QJsonValue &value : sessions) {
        if (!value.isObject())
            continue;
        const QJsonObject obj = value.toObject();

        QString exerciseId = obj.value("exerciseId").toString();
        /*if (exerciseId.isEmpty())
            exerciseId = kFreeTypingEnId;
        */

        if (!exerciseIdFilter.isEmpty() && exerciseId != exerciseIdFilter)
            continue;

        const double kpm = obj.value("kpm").toDouble();
        const double accuracy = obj.value("accuracy").toDouble(0.0);
        QString dateTime = obj.value("completedAt").toString();
        const int durationSeconds = obj.value("durationSeconds").toInt();

        kpmSeries.append(kpm);
        accuracySeries.append(qBound(0.0, accuracy, 100.0));
        exerciseIdSeries.append(exerciseId);
        dateTimeSeries.append(dateTime);
        durationSeries.append(durationSeconds);
    }
}

void HistoryWidget::reloadSessions()
{
    loadSessions();
    updateScrollBounds();
    update();
}

void HistoryWidget::updateScrollBounds()
{
    const int count = kpmSeries.size();
    const int maxStart = qMax(0, count - visibleSessionsCount);
    startSessionIndex = maxStart;
    sessionsScrollBar->setRange(0, maxStart);
    sessionsScrollBar->setPageStep(1);
    sessionsScrollBar->setSingleStep(1);
    sessionsScrollBar->setValue(startSessionIndex);
    sessionsScrollBar->setVisible(maxStart > 0);
}

void HistoryWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.fillRect(rect(), QColor("#11151c"));

    const int scrollBarHeight = 28;
    sessionsScrollBar->setGeometry(60, height() - scrollBarHeight, qMax(1, width() - 90), 20);

    const QRect plotRect(60, 35, width() - 90, height() - 90 - scrollBarHeight);
    if (plotRect.width() <= 0 || plotRect.height() <= 0)
        return;

    painter.setPen(QPen(QColor("#d0d7e2"), 1));
    painter.drawRect(plotRect);
    const QString title = exerciseIdFilter.isEmpty()
        ? tr("Завершенные сессии: КВМ и точность %")
        : tr("Упражнение %1: КВМ и точность%").arg(exerciseIdFilter);
    painter.drawText(QRect(0, 8, width(), 20), Qt::AlignCenter, title);

    if (kpmSeries.isEmpty()) {
        painter.setPen(QColor("#c0c8d3"));
        painter.drawText(plotRect, Qt::AlignCenter, tr("Нету завершенных сессий"));
        return;
    }

    const int count = kpmSeries.size();
    const int endSessionIndex = qMin(count, startSessionIndex + visibleSessionsCount);
    const int visibleCount = endSessionIndex - startSessionIndex;

    const auto visibleBegin = kpmSeries.constBegin() + startSessionIndex;
    const auto visibleEnd = kpmSeries.constBegin() + endSessionIndex;
    const double maxKpm = *std::max_element(visibleBegin, visibleEnd);
    const double maxY = qMax(100.0, maxKpm + 5.0);

    auto pointAt = [&](int index, double value) -> QPointF {
        const double xRatio = visibleCount == 1 ? 0.5 : static_cast<double>(index) / static_cast<double>(visibleCount - 1);
        const double yRatio = value / maxY;
        const double x = plotRect.left() + xRatio * plotRect.width();
        const double y = plotRect.bottom() - yRatio * plotRect.height();
        return QPointF(x, y);
    };

    QPen gridPen(QColor("#3a4150"), 1, Qt::DashLine);
    painter.setPen(gridPen);
    for (int i = 1; i <= 4; ++i) {
        const int y = plotRect.top() + (i * plotRect.height()) / 5;
        painter.drawLine(plotRect.left(), y, plotRect.right(), y);
    }
    painter.setPen(QColor("#c0c8d3"));
    for (int i = 0; i <= 5; ++i) {
        const double value = maxY * static_cast<double>(i) / 5.0;
        const int y = plotRect.bottom() - (i * plotRect.height()) / 5;
        painter.drawText(8, y + 4, QString::number(static_cast<int>(qRound(value))));
    }

    QPolygonF kpmLine;
    QPolygonF accuracyLine;
    kpmPoints.clear();
    accuracyPoints.clear();
    for (int i = 0; i < visibleCount; ++i) {
        const int sessionIndex = startSessionIndex + i;
        const QPointF kpmPoint = pointAt(i, kpmSeries[sessionIndex]);
        const QPointF accuracyPoint = pointAt(i, accuracySeries[sessionIndex]);

        kpmLine << kpmPoint;
        accuracyLine << accuracyPoint;

        const QString exerciseId = exerciseIdSeries[sessionIndex];
        const QString dateTime = dateTimeSeries[sessionIndex];
        const int durationSeconds = durationSeries[sessionIndex];
        kpmPoints.append({kpmPoint, QStringLiteral("KPM"), kpmSeries[sessionIndex], exerciseId, dateTime, durationSeconds});
        accuracyPoints.append({accuracyPoint, QStringLiteral("Accuracy"), accuracySeries[sessionIndex], exerciseId, dateTime, durationSeconds});
    }

    painter.setPen(QPen(QColor("#52c7ea"), 2.2));
    painter.drawPolyline(kpmLine);

    painter.setPen(QPen(QColor("#7ee787"), 2.2));
    painter.drawPolyline(accuracyLine);

    painter.setBrush(QColor("#52c7ea"));
    painter.setPen(Qt::NoPen);
    for (const QPointF &p : kpmLine)
        painter.drawEllipse(p, 3, 3);

    painter.setBrush(QColor("#7ee787"));
    for (const QPointF &p : accuracyLine)
        painter.drawEllipse(p, 3, 3);

    painter.setPen(QColor("#52c7ea"));
    painter.drawText(plotRect.left(), plotRect.bottom() + 22, tr("КВМ"));
    painter.setPen(QColor("#7ee787"));
    painter.drawText(plotRect.left() + 90, plotRect.bottom() + 22, tr("Точность %"));
    painter.setPen(QColor("#c0c8d3"));
    painter.drawText(plotRect.right() - 150, plotRect.bottom() + 22, tr("Сессии %1-%2")
                     .arg(startSessionIndex + 1)
                     .arg(endSessionIndex));

    if (hasHoveredPoint) {
         const QString metricText = hoveredPoint.label == "KPM"
            ? tr("КВМ: %1").arg(static_cast<int>(qRound(hoveredPoint.value)))
            : tr("Точность: %1%").arg(QString::number(hoveredPoint.value, 'f', 1));
        const QString valueText = hoveredPoint.exerciseId + QLatin1Char('\n')
                                   + hoveredPoint.dateTime + QLatin1Char('\n')
                                   + timeFormat(hoveredPoint.durationSeconds) + QLatin1Char('\n')
                                   + metricText;

        QFontMetrics metrics = painter.fontMetrics();

        const QRect textBounds = metrics.boundingRect(QRect(0, 0, 240, 0), Qt::TextWordWrap, valueText);
        QRect tooltipRect(hoveredPoint.pos.x() + 10, hoveredPoint.pos.y() - textBounds.height() - 12,
                          textBounds.width() + 14, textBounds.height() + 8);

        if (tooltipRect.right() > width())
            tooltipRect.moveRight(width() - 6);
        if (tooltipRect.top() < 0)
            tooltipRect.moveTop(6);

        painter.setPen(QPen(QColor("#0d1117"), 1));
        painter.setBrush(QColor("#f0f6fc"));
        painter.drawRoundedRect(tooltipRect, 5, 5);
        painter.setPen(QColor("#0d1117"));
        painter.drawText(tooltipRect.adjusted(7, 4, -7, -4), Qt::AlignLeft | Qt::AlignVCenter, valueText);
    }
}

void HistoryWidget::mouseMoveEvent(QMouseEvent *event)
{
    updateHoveredPoint(event->position());
    QWidget::mouseMoveEvent(event);
}

void HistoryWidget::leaveEvent(QEvent *event)
{
    hasHoveredPoint = false;
    update();
    QWidget::leaveEvent(event);
}

void HistoryWidget::updateHoveredPoint(const QPointF &cursorPos)
{
    const double hoverRadius = 8.0;
    const double hoverRadiusSq = hoverRadius * hoverRadius;

    bool found = false;
    PlotPoint nearestPoint;
    double nearestDistSq = std::numeric_limits<double>::max();

    auto processPoints = [&](const QVector<PlotPoint> &points) {
        for (const PlotPoint &point : points) {
            const double dx = point.pos.x() - cursorPos.x();
            const double dy = point.pos.y() - cursorPos.y();
            const double distSq = dx * dx + dy * dy;
            if (distSq <= hoverRadiusSq && distSq < nearestDistSq) {
                nearestDistSq = distSq;
                nearestPoint = point;
                found = true;
            }
        }
    };

    processPoints(kpmPoints);
    processPoints(accuracyPoints);

    if (found) {
        hasHoveredPoint = true;
        hoveredPoint = nearestPoint;
    } else {
        hasHoveredPoint = false;
    }
    update();
}
