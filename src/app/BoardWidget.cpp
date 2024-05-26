#include "BoardWidget.h"
#include "Svgs.h"

#include "GameLib/Board.h"
#include "GameLib/GameControl.h"
#include "GameLib/GameModel.h"
#include "GameLib/Util.h"

#include <QPainter>
#include <QPaintEvent>

constexpr int CellGridLineWidth = 2;
constexpr QSize CellSize(30, 30);
constexpr QSize CellStride = CellSize + QSize(CellGridLineWidth, CellGridLineWidth);


BoardWidget::BoardWidget(QWidget* parent)
    : QWidget(parent)
    , m_xPixmap(GetIconX(CellSize))
    , m_oPixmap(GetIconO(CellSize))
{
}

void BoardWidget::SetGameModel(std::weak_ptr<GameModel> gameModel)
{
    m_model = std::move(gameModel);
    setFixedSize(GetBoardSize());
    update();
}

void BoardWidget::SetGameControl(std::weak_ptr<GameControl> control)
{
    m_control = std::move(control);
}

void BoardWidget::UpdateView()
{
    update();
}

void BoardWidget::paintEvent(QPaintEvent* event)
{
    auto board = GetBoard();
    if (!board)
        return;

    const uint8_t Cols = board->Columns();
    const uint8_t Rows = board->Rows();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.fillRect(event->rect(), Qt::GlobalColor::black);

    // Draw cells
    for (uint8_t row = 0; row < Rows; ++row) {
        for (uint8_t col = 0; col < Cols; ++col) {

            const auto cellRect = GetCellRect(col, row);

            switch (auto cellState = board->GetCell({ col, row }))
            {
                case BoardCellState::Blank:
                    painter.fillRect(cellRect, Qt::GlobalColor::white);
                    break;
                case BoardCellState::X:
                    painter.drawPixmap(cellRect, m_xPixmap);
                    break;
                case BoardCellState::O:
                    painter.drawPixmap(cellRect, m_oPixmap);
                    break;
            }
        }
    }

    // Cross out winning sequence
    if (auto model = m_model.lock())
    {
        if (model->IsWonState())
        {
            const GameState state = model->GetState();
            const BoardCellState winnerCellState = state == GameState::WonX ? BoardCellState::X : BoardCellState::O;
            const int lengthToWin = GetLineLengthToWin(model->GetGameType());
            if (auto line = board->FindLine(winnerCellState, lengthToWin))
            {
                QPen pen(Qt::red, 4, Qt::SolidLine, Qt::PenCapStyle::MPenCapStyle);
                painter.setPen(pen);

                const Point cellBegin = line->Start();
                const Point cellEnd   = line->End();

                painter.drawLine(GetCellRect(cellBegin.x, cellBegin.y).center(), GetCellRect(cellEnd.x, cellEnd.y).center());
            }
        }
    }
}

void BoardWidget::mousePressEvent(QMouseEvent* event)
{
    auto control = m_control.lock();
    if (!control)
        return;

    auto board = GetBoard();
    if (!board)
        return;

    const auto eventPos = event->position();
    const uint32_t col = static_cast<uint32_t>(eventPos.x() / CellStride.width());
    const uint32_t row = static_cast<uint32_t>(eventPos.y() / CellStride.height());
    if (col >= board->Columns() || row >= board->Rows())
        return;

    const QRect rect = GetCellRect(col, row);
    if (eventPos.x() >= rect.left() && eventPos.x() <= rect.right() && // Ignore if clicked on the grid itself
        eventPos.y() >= rect.top()  && eventPos.y() <= rect.bottom())
    {
        control->OnBoardCellPressed({ static_cast<uint8_t>(col), static_cast<uint8_t>(row) });
    }
}

std::shared_ptr<Board> BoardWidget::GetBoard() const
{
    if (auto gameModel = m_model.lock())
        return gameModel->GetBoard();

    return nullptr;
}

QSize BoardWidget::GetBoardSize() const
{
    if (auto board = GetBoard()) {
        const int Cols = static_cast<int>(board->Columns());
        const int Rows = static_cast<int>(board->Rows());
        return { Cols * CellSize.width()  + (Cols + 1) * CellGridLineWidth,
                 Rows * CellSize.height() + (Rows + 1) * CellGridLineWidth };
    }

    return { 0, 0 };
}

QRect BoardWidget::GetCellRect(uint8_t col, uint8_t row) const
{
    const QPoint leftTop(col * CellStride.width() + CellGridLineWidth,
                         row * CellStride.height() + CellGridLineWidth);
    return QRect(leftTop, CellSize);
}
