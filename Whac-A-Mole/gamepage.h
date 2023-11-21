#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QWidget>
#include <QPushButton>

class GamePage : public QWidget
{
    Q_OBJECT

public:
    explicit GamePage(const QSize &size, QWidget *parent = nullptr);

public slots:
    void startGame();  // Declare the slot for starting the game

    // ... other members and functions ...s
};

#endif // GAMEPAGE_H

