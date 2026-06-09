#ifndef UTILS_H
#define UTILS_H

#include <QString>

class QListWidget;

QString timeFormat(unsigned long long timeInSec);
void configureSideListWidget(QListWidget *list);

#endif // UTILS_H
