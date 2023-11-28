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

    void handleGame(const QString& playerName); // New method for game logic

public slots:
    void startGame(const QString& playerName);
    void stopGame();

signals:
    void scoreUpdated(int newScore);
    void gameStarted();
    void gameEnded();
    void countdownUpdated(int timeLeft);

private:
    GameController gameController;
    Player player;
    HighScore highScore;
};

#endif // HARDWAREINTERFACE_H
