#include "playpage.h"
#include "HardwareInterface.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

PlayPage::PlayPage(const QSize &size, QWidget *parent) 
    : QWidget(parent), hardwareInterface(new HardwareInterface(this)) {
    setFixedSize(size);
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Username input
    QLineEdit *usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Enter your name");
    layout->addWidget(usernameInput);

    // Create and configure the welcome message label
    QLabel *welcomeLabel = new QLabel(this);
    layout->addWidget(welcomeLabel);

    // Create start button
    QPushButton *startButton = new QPushButton("Start Game", this);
    layout->addWidget(startButton);

    // Create and configure the label for countdown timer
    QLabel *countdownLabel = new QLabel("Time left: --", this);
    layout->addWidget(countdownLabel);

    // Create score label
    QLabel *scoreLabel = new QLabel("Score: 0", this);
    layout->addWidget(scoreLabel);

    // Create and configure the label for gameplay status
    QLabel *gameStatusLabel = new QLabel("Game not started", this);
    layout->addWidget(gameStatusLabel);

    // Connect start button signal to HardwareInterface slot
    connect(startButton, &QPushButton::clicked, this, [this, usernameInput, welcomeLabel, startButton]() {
        QString playerName = usernameInput->text();
        if (!playerName.isEmpty()) {
            welcomeLabel->setText("Welcome " + playerName + "!");
            hardwareInterface->startGame(playerName);
            //Call should be made here to inGame
            startButton->setDisabled(true); // Disable the start button when game starts
        } else {
            // Handle empty username case, e.g., show a message
        }
    });

    // Connect HardwareInterface signals to PlayPage slots for updating the UI
    connect(hardwareInterface, &HardwareInterface::scoreUpdated, this, [scoreLabel](int newScore) {
        scoreLabel->setText(QString("Score: %1").arg(newScore));
    });

    connect(hardwareInterface, &HardwareInterface::countdownUpdated, this, [countdownLabel](int timeLeft) {
        countdownLabel->setText("Time left: " + QString::number(timeLeft));
    });

    // Handle game start and end
    connect(hardwareInterface, &HardwareInterface::gameStarted, this, [gameStatusLabel]() {
        gameStatusLabel->setText("Game is running...");
    });

    connect(hardwareInterface, &HardwareInterface::gameEnded, this, [gameStatusLabel, startButton]() {
        gameStatusLabel->setText("Game over!");
        startButton->setEnabled(true); // Enable the start button when game ends
    });

    setLayout(layout);
}

PlayPage::~PlayPage() {
    // Cleanup if needed
}

