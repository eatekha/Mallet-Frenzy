#include "playpage.h"
#include "mainwindow.h"
#include "HardwareInterface.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QThread>

PlayPage::PlayPage(const QSize &size, QWidget *parent) 
    : QWidget(parent), hardwareInterface(new HardwareInterface(this)) {
    setFixedSize(size);
    setStyleSheet("QWidget { background-color: #857be6; }"  // Set background color
                  "QLabel { font-size: 20px; color: #FFD700; text-align: center; }" // Gold text
                  "QLineEdit { font-size: 18px; color: black; background-color: white; border-radius: 5px; padding: 10px; margin-bottom: 15px; text-align: center; border: 2px solid #FFD700; }" // Styled input field
                 );

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLineEdit *usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Enter your name");
    layout->addWidget(usernameInput, 0, Qt::AlignCenter);

    // Use an image for the start button
    QPushButton *startButton = new QPushButton(this);
    QPixmap startButtonPixmap("start.png"); // Replace with your image path
    startButtonPixmap = startButtonPixmap.scaled(QSize(190, 80), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    startButton->setIcon(QIcon(startButtonPixmap));
    startButton->setIconSize(startButtonPixmap.size());
    startButton->setFixedSize(QSize(190, 80));
    startButton->setStyleSheet("border: none; background-color: transparent;");
    layout->addWidget(startButton, 0, Qt::AlignCenter);

    QLabel *welcomeLabel = new QLabel(this);
    layout->addWidget(welcomeLabel, 0, Qt::AlignCenter);

    QLabel *countdownLabel = new QLabel("Time left: --", this);
    layout->addWidget(countdownLabel, 0, Qt::AlignCenter);

    QLabel *scoreLabel = new QLabel("Score: 0", this);
    layout->addWidget(scoreLabel, 0, Qt::AlignCenter);

    //QLabel *gameStatusLabel = new QLabel("Game not started", this);
    //layout->addWidget(gameStatusLabel, 0, Qt::AlignCenter);   
    

    connect(startButton, &QPushButton::clicked, this, [this, usernameInput, welcomeLabel, startButton]() {
        QString playerName = usernameInput->text();
        if (!playerName.isEmpty()) {
            welcomeLabel->setText("Get Ready, " + playerName + "!");
            hardwareInterface->handleGame(playerName);
            startButton->setDisabled(true);
            usernameInput->setDisabled(true); // Disable the username input field
        } else {
            welcomeLabel->setText("Please enter your name!");
            welcomeLabel->setStyleSheet("color: red;"); // Change color for feedback
        }
    });



    



    connect(hardwareInterface, &HardwareInterface::scoreUpdated, this, [scoreLabel](int newScore) {
        scoreLabel->setText(QString("Score: %1").arg(newScore));
    });

    connect(hardwareInterface, &HardwareInterface::countdownUpdated, this, [countdownLabel](int timeLeft) {
        countdownLabel->setText("Time left: " + QString::number(timeLeft));
    });

    //connect(hardwareInterface, &HardwareInterface::gameStarted, this, [gameStatusLabel]() {
        //gameStatusLabel->setText("Game is running...");
    //});

    connect(hardwareInterface, &HardwareInterface::gameEnded, this, [startButton, usernameInput]() {
        //gameStatusLabel->setText("Game over!");
        playAudio("over.wav");
        startButton->setEnabled(true);
        usernameInput->setEnabled(true); // Re-enable the username input field
    });

    setLayout(layout);
}

PlayPage::~PlayPage() {
    // Cleanup if needed
}




