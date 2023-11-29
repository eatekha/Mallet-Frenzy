#ifndef PLAYPAGE_H
#define PLAYPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include "HardwareInterface.h" 

class PlayPage : public QWidget {
    Q_OBJECT

public:
    explicit PlayPage(const QSize &size, QWidget *parent = nullptr);
    ~PlayPage();  // Destructor

signals:
    void returnToMainWindowRequested(); // Signal to return to the main window

private:
    HardwareInterface* hardwareInterface;  // Hardware interface

    // UI elements as member variables
    QLineEdit* usernameInput;      // Input field for username
    QLabel* welcomeLabel;          // Label to display welcome message
    QPushButton* startButton;      // Button to start the game
    QLabel* countdownLabel;        // Label to display countdown
    QLabel* scoreLabel;            // Label to display score
    QLabel* gameStatusLabel;       // Label to display game status

    QPushButton* returnToMainWindowButton; // New member for the return button

    // You can add more member variables as needed
};

#endif // PLAYPAGE_H

