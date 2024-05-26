#include "NewGameDialog.h"
#include "GameLib/GameTypes.h"
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <cassert>


NewGameDialog::NewGameDialog(QWidget* parent)
    : QDialog(parent, Qt::MSWindowsFixedSizeDialogHint)
{
    setMinimumSize(180, 150);

    auto *placeholder = new QWidget(this);

    auto* gameTypeLabel  = new QLabel("Game type");
    auto* boardSizeLabel = new QLabel("Board size");
    auto* player1Label   = new QLabel("Player 1");
    auto* player2Label   = new QLabel("Player 2");

    m_gameTypeComboBox = new QComboBox();

    m_boardWidthSpinBox  = new QSpinBox();
    m_boardHeightSpinBox = new QSpinBox();
    auto* boardSizeDelimLabel = new QLabel("x");
    auto* boardSizeHLayout = new QHBoxLayout();
    boardSizeHLayout->addWidget(m_boardWidthSpinBox);
    boardSizeHLayout->addWidget(boardSizeDelimLabel);
    boardSizeHLayout->addWidget(m_boardHeightSpinBox);

    m_gameTypeComboBox->addItem("3 in a Row");
    m_gameTypeComboBox->addItem("5 in a Row");

    m_player1ComboBox = new QComboBox();
    m_player1ComboBox->addItem("Human");
    m_player1ComboBox->addItem("Computer");

    m_player2ComboBox = new QComboBox();
    m_player2ComboBox->addItem("Human");
    m_player2ComboBox->addItem("Computer");

    auto* dialogButtons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    auto* grid = new QGridLayout(placeholder);
    grid->addWidget(gameTypeLabel, 0, 0);
    grid->addWidget(boardSizeLabel, 1, 0);
    grid->addWidget(player1Label, 2, 0);
    grid->addWidget(player2Label, 3, 0);

    grid->addWidget(m_gameTypeComboBox, 0, 1);
    grid->addLayout(boardSizeHLayout, 1, 1);
    grid->addWidget(m_player1ComboBox, 2, 1);
    grid->addWidget(m_player2ComboBox, 3, 1);

    grid->addWidget(dialogButtons, 4, 0, 1, 2);

    setWindowTitle("Select game to start");

    m_boardWidthSpinBox->setRange(10, 25);
    m_boardHeightSpinBox->setRange(10, 25);
    m_boardWidthSpinBox->setEnabled(false);
    m_boardHeightSpinBox->setEnabled(false);
    m_player2ComboBox->setCurrentIndex(1);

    connect(m_gameTypeComboBox, &QComboBox::currentIndexChanged, [=](int index) {
        const bool sizeEnabled = index != 0;
        m_boardWidthSpinBox->setEnabled(sizeEnabled);
        m_boardHeightSpinBox->setEnabled(sizeEnabled);
    });

    connect(dialogButtons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(dialogButtons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

NewGameSettings NewGameDialog::GetSettings() const
{
    assert(result() == QDialog::Accepted);

    NewGameSettings s = {};
    if (m_gameTypeComboBox->currentIndex())
    {
        s.m_gameType = GameType::FiveInRow;
        s.m_boardSize[0] = m_boardWidthSpinBox->value();
        s.m_boardSize[1] = m_boardHeightSpinBox->value();
    }
    s.m_players[0] = m_player1ComboBox->currentIndex() ? PlayerType::Computer : PlayerType::Human;
    s.m_players[1] = m_player2ComboBox->currentIndex() ? PlayerType::Computer : PlayerType::Human;

    return s;
}
