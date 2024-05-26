#include "Application.h"
#include "BoardWidget.h"
#include "StatusBar.h"
#include "GameLib/GameControl.h"
#include <QTimer>

TicTacApplication::TicTacApplication(int argc, char* argv[])
    : QApplication(argc, argv)
{}

int TicTacApplication::Run()
{
    auto control = std::make_shared<GameControl>(m_wnd.GetBoardWidget(), m_wnd.GetStatusBar());
    m_wnd.SetGameControl(control);
    control->StartNewGame({});

    // Periodically call GameControl.Update() to let computer player(s) make moves
    QTimer updateTimer;
    updateTimer.callOnTimeout([=] {
        control->Update();
    });

    m_wnd.show();

    updateTimer.start(std::chrono::milliseconds(300));

    return QApplication::exec();
}
