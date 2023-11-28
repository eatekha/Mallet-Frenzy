#include "HardwareInterface.h"
#include <QTimer>
#include <QThread>

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
    emit gameStarted();

    // QTimer should be created in the thread it's going to be used in
    QTimer *countdownTimer = new QTimer(this);
    countdownTimer->moveToThread(QThread::currentThread()); // Ensure timer is in the correct thread
    connect(countdownTimer, &QTimer::timeout, this, [this, countdownTimer]() {
        if (!gameController.timer.isTimeUp()) {
            emit countdownUpdated(gameController.timer.getTimeLeft());
        } else {
            countdownTimer->stop();
            countdownTimer->deleteLater();
            stopGame();
        }
    });
    countdownTimer->start(1000);
}

void HardwareInterface::stopGame() {
    gameController.endGame(player);
    //highScore.add(player.getScore(), player.getName());
    emit gameEnded();
}

void HardwareInterface::handleGame(const QString& playerName) {
    // Creating a thread for game logic
    QThread *gameThread = new QThread();
    moveToThread(gameThread); // Move this object to the new thread

    // Connecting signals and slots for thread management
    connect(gameThread, &QThread::started, this, [this, playerName]() {
        startGame(playerName);
        // Add game logic here
        // This can include interaction with LEDMatrix and GameController
        gameController.inGame(player, highScore);
        stopGame();
    });
    connect(this, &HardwareInterface::gameEnded, gameThread, &QThread::quit);
    connect(gameThread, &QThread::finished, gameThread, &QThread::deleteLater);

    // Start the thread
    gameThread->start();
}
