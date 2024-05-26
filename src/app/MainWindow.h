#pragma once
#include <QMainWindow>
#include <memory>

class BoardWidget;
class GameControl;
class StatusBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    void SetGameControl(std::weak_ptr<GameControl> control);
    BoardWidget* GetBoardWidget() const;
    StatusBar* GetStatusBar() const;

private:
    void NewGamePressed();

private:
    std::weak_ptr<GameControl> m_gameControl;
    BoardWidget* m_board;
    StatusBar* m_statusBar;
};
