#include "playpage.h"
#include "HardwareInterface.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QThread>

PlayPage::PlayPage(const QSize &size, QWidget *parent) 
    : QWidget(parent), hardwareInterface(new HardwareInterface(this)) {
    setFixedSize(size);
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLineEdit *usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Enter your name");
    layout->addWidget(usernameInput);

    QLabel *welcomeLabel = new QLabel(this);
    layout->addWidget(welcomeLabel);

    QPushButton *startButton = new QPushButton("Start Game", this);
    layout->addWidget(startButton);

    QLabel *countdownLabel = new QLabel("Time left: --", this);
    layout->addWidget(countdownLabel);

    QLabel *scoreLabel = new QLabel("Score: 0", this);
    layout->addWidget(scoreLabel);

    QLabel *gameStatusLabel = new QLabel("Game not started", this);
    layout->addWidget(gameStatusLabel);

    connect(startButton, &QPushButton::clicked, this, [this, usernameInput, welcomeLabel, startButton]() {
        QString playerName = usernameInput->text();
        if (!playerName.isEmpty()) {
            welcomeLabel->setText("Welcome " + playerName + "!");
            hardwareInterface->handleGame(playerName);
            startButton->setDisabled(true);
            usernameInput->setDisabled(true); // Disable the username input field
        } else {
            // Handle empty username case
        }
    });

    connect(hardwareInterface, &HardwareInterface::scoreUpdated, this, [scoreLabel](int newScore) {
        scoreLabel->setText(QString("Score: %1").arg(newScore));
    });

    connect(hardwareInterface, &HardwareInterface::countdownUpdated, this, [countdownLabel](int timeLeft) {
        countdownLabel->setText("Time left: " + QString::number(timeLeft));
    });

    connect(hardwareInterface, &HardwareInterface::gameStarted, this, [gameStatusLabel]() {
        gameStatusLabel->setText("Game is running...");
    });

    connect(hardwareInterface, &HardwareInterface::gameEnded, this, [gameStatusLabel, startButton, usernameInput]() {
        gameStatusLabel->setText("Game over!");
        startButton->setEnabled(true);
        usernameInput->setEnabled(true); // Re-enable the username input field
    });

    setLayout(layout);
}

PlayPage::~PlayPage() {
    // Cleanup if needed
}

