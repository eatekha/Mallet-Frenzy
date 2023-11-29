#include "playpage.h"
#include "mainwindow.h"
#include "HardwareInterface.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QThread>

/**
 * @class PlayPage
 * @brief The PlayPage class for displaying the game interface.
 *
 * This class is responsible for setting up and displaying the game play interface.
 * It includes player name input, start button, welcome label, countdown, and score display.
 * @author Yangxiuye Gu
 */
PlayPage::PlayPage(const QSize &size, QWidget *parent)
        : QWidget(parent), hardwareInterface(new HardwareInterface(this)) {
    setFixedSize(size);
    setStyleSheet("QWidget { background-color: #857be6; }"
                  "QLabel { font-size: 20px; color: #FFD700; text-align: center; }"
                  "QLineEdit { font-size: 18px; color: black; background-color: white; border-radius: 5px; padding: 10px; margin-bottom: 15px; text-align: center; border: 2px solid #FFD700; }");

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLineEdit *usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Enter your name");
    layout->addWidget(usernameInput, 0, Qt::AlignCenter);

    QPushButton *startButton = new QPushButton(this);
    QPixmap startButtonPixmap("start.png");
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

    connect(startButton, &QPushButton::clicked, this, [this, usernameInput, welcomeLabel, startButton]() {
        QString playerName = usernameInput->text();
        if (!playerName.isEmpty()) {
            welcomeLabel->setText("Get Ready, " + playerName + "!");
            hardwareInterface->handleGame(playerName);
            startButton->setDisabled(true);
            usernameInput->setDisabled(true);
        } else {
            welcomeLabel->setText("Please enter your name!");
            welcomeLabel->setStyleSheet("color: red;");
        }
    });

    connect(hardwareInterface, &HardwareInterface::scoreUpdated, this, [scoreLabel](int newScore) {
        scoreLabel->setText(QString("Score: %1").arg(newScore));
    });

    connect(hardwareInterface, &HardwareInterface::countdownUpdated, this, [countdownLabel](int timeLeft) {
        countdownLabel->setText("Time left: " + QString::number(timeLeft));
    });

    connect(hardwareInterface, &HardwareInterface::gameEnded, this, [startButton, usernameInput]() {
        playAudio("over.wav");
        startButton->setEnabled(true);
        usernameInput->setEnabled(true);
    });

    setLayout(layout);
}

/**
 * @brief Destructor for PlayPage.
 *
 * Cleans up resources used by the PlayPage instance.
 */
PlayPage::~PlayPage() {
    // Cleanup if needed
}
