#pragma once
#include "GameLib/IGameView.h"
#include <QWidget>
#include <QPixmap>

class Board;
class GameControl;
class GameModel;

class BoardWidget : public QWidget, public IGameView
{
    Q_OBJECT

public:
    BoardWidget(QWidget* parent = nullptr);

    void SetGameModel(std::weak_ptr<GameModel> gameModel) override;
    void SetGameControl(std::weak_ptr<GameControl> gameControl) override;
    void UpdateView() override;

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    QSize GetBoardSize() const;
    QRect GetCellRect(uint8_t col, uint8_t row) const;
    std::shared_ptr<Board> GetBoard() const;

private:
    std::weak_ptr<GameModel> m_model;
    std::weak_ptr<GameControl> m_control;

    QPixmap m_xPixmap;
    QPixmap m_oPixmap;
};
