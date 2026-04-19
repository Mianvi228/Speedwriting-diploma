#include "utils.h"

QString timeFormat(unsigned long long timeInSec)
{
    unsigned int hours = timeInSec / 3600;
    unsigned int minutes = (timeInSec % 3600) / 60;
    unsigned int seconds = timeInSec % 60;

    QString s = QString("%1:%2:%3")
                    .arg(hours, 2, 10, QChar('0'))
                    .arg(minutes, 2, 10, QChar('0'))
                    .arg(seconds, 2, 10, QChar('0'));
    return s;
}
