#pragma once
#include <QApplication>
#include "MainWindow.h"

class TicTacApplication : public QApplication
{
public:
    TicTacApplication(int argc, char* argv[]);

    int Run();

private:
    MainWindow m_wnd;
};
