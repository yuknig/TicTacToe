#pragma once
#include <QComboBox>
#include <QDialog>
#include <QSpinBox>

struct NewGameSettings;

class NewGameDialog : public QDialog
{
    Q_OBJECT

public:
    NewGameDialog(QWidget* parent = nullptr);

    NewGameSettings GetSettings() const;

private:
    QComboBox* m_gameTypeComboBox;
    QComboBox* m_player1ComboBox;
    QComboBox* m_player2ComboBox;
    QSpinBox* m_boardWidthSpinBox;
    QSpinBox* m_boardHeightSpinBox;
};
