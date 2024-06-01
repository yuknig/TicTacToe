#include "MainWindow.h"
#include "GameLib/GameControl.h"
#include "GameLib/GameTypes.h"
#include "NewGameDialog.h"
#include "BoardWidget.h"
#include "StatusBar.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>

MainWindow::MainWindow()
    : m_board(new BoardWidget(nullptr))
    , m_statusBar(new StatusBar(this))
{
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    setMinimumSize(250, 150);
    layout()->setSizeConstraint(QLayout::SetMinimumSize);

    auto* centralWidget = new QWidget(this);
    auto* newGameButton = new QPushButton("New Game");

    auto* vLayout = new QVBoxLayout(centralWidget);
    vLayout->addWidget(newGameButton);

    auto hBoardLayout = new QHBoxLayout(); // Use QHBoxLayout to center board horizontally
    hBoardLayout->addWidget(m_board);
    vLayout->addLayout(hBoardLayout);
    vLayout->addStretch();

    setCentralWidget(centralWidget);
    setStatusBar(m_statusBar);

    connect(newGameButton, &QPushButton::clicked, this, &MainWindow::NewGamePressed);
}

void MainWindow::SetGameControl(std::weak_ptr<GameControl> control)
{
    m_gameControl = std::move(control);
    m_board->SetGameControl(m_gameControl);
}

BoardWidget* MainWindow::GetBoardWidget() const
{
    return m_board;
}

StatusBar* MainWindow::GetStatusBar() const
{
    return m_statusBar;
}

void MainWindow::NewGamePressed()
{
    auto gameControl = m_gameControl.lock();
    if (!gameControl)
    {
        qWarning() << __func__ << " GameControl is null";
        return;
    }

    NewGameDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        const auto settings = dialog.GetSettings();

        if (settings.m_gameType == GameType::FiveInRow && (settings.m_players[0] == PlayerType::Computer || settings.m_players[1] == PlayerType::Computer))
        {
            QMessageBox msgBox(QMessageBox::Warning, "Warning", "Note: computer plays badly and slow on a big board");
            msgBox.exec();
        }

        gameControl->StartNewGame(settings);
    }
}
