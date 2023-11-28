#include "HardwareInterface.h"
#include <QTimer>

HardwareInterface::HardwareInterface(QObject *parent)
    : QObject(parent), gameController(), player(), highScore() {
    // Initialization, if needed
}

HardwareInterface::~HardwareInterface() {
    // Clean up, if needed
}

void HardwareInterface::startGame(const QString& playerName) {
    player.setName(playerName.toStdString());
    gameController.setup();
    gameController.startGame();
    emit gameStarted(); // Signal game start

    // Start the timer for 30 seconds
    gameController.timer.start(); 

    // Update the UI every second with the remaining time
    QTimer *countdownTimer = new QTimer(this);
    connect(countdownTimer, &QTimer::timeout, this, [this, countdownTimer]() {
        if (!gameController.timer.isTimeUp()) {
            emit countdownUpdated(gameController.timer.getTimeLeft());
        } else {
            countdownTimer->stop();
            countdownTimer->deleteLater(); // Clean up the timer
            this->stopGame(); // Stop the game when time is up
        }
    });
    countdownTimer->start(1000); // Update every second
}


void HardwareInterface::stopGame() {
    gameController.endGame(player);
    highScore.add(player.getScore(), player.getName());
    emit gameEnded(); // Emit a signal indicating the game has ended
}

