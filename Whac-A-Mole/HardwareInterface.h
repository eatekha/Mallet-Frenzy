#ifndef HARDWAREINTERFACE_H
#define HARDWAREINTERFACE_H

#include <QObject>
#include "Hardware/GameController.h"
#include "Hardware/Player.h"
#include "Hardware/HighScore.h"

class HardwareInterface : public QObject {
    Q_OBJECT

public:
    explicit HardwareInterface(QObject *parent = nullptr);
    ~HardwareInterface();

public slots:
    void startGame(const QString& playerName);
    void stopGame();

signals:
    void scoreUpdated(int newScore);
    void gameStarted();  // Signal to indicate the game has started
    void gameEnded();    // Signal to indicate the game has ended
    void countdownUpdated(int timeLeft); // New signal for countdown updates

private:
    GameController gameController;
    Player player;
    HighScore highScore;
};

#endif // HARDWAREINTERFACE_H
