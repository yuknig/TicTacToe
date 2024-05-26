#include "StatusBar.h"
#include <QString>

void StatusBar::ShowStatusMessage(const std::string& msg)
{
    QStatusBar::showMessage(QString::fromStdString(msg));
}
