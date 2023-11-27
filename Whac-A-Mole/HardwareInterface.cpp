#include "HardwareInterface.h"
#include <iostream>

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

    // Example of how to use a QTimer to periodically update the score
    // QTimer *timer = new QTimer(this);
    // connect(timer, &QTimer::timeout, this, [this]() {
    //     emit scoreUpdated(player.getScore());
    // });
    // timer->start(1000); // Adjust the interval as needed
}

void HardwareInterface::stopGame() {
    gameController.endGame(player);
    highScore.add(player.getScore(), player.getName());
    emit gameEnded();
}
