#pragma once
#include "GameLib/IStatusMessage.h"
#include <QStatusBar>

class StatusBar : public QStatusBar, public IStatusMessage
{
public:
    using QStatusBar::QStatusBar;

    void ShowStatusMessage(const std::string& msg) override;
};
