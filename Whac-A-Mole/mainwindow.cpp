#include "mainwindow.h"
#include <QVBoxLayout>
#include <QApplication>
#include <QDebug>
#include <QLabel>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include "gamepage.h"
#include "scorespage.h"
#include <QMovie>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Set window title and fixed window size
    setWindowTitle("Whac-A-Mole Game");
    setFixedSize(900, 758);

    // Set background image
    QPixmap background("C:/Users/rexgu/Documents/Whac-A-Mole/Whac-A-Mole/mainbgimage.png");
    QLabel *backgroundLabel = new QLabel(this);
    backgroundLabel->setPixmap(background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    backgroundLabel->setScaledContents(true);
    backgroundLabel->setGeometry(this->rect());

    // Create a QLabel for the GIF
    QLabel *gifLabel = new QLabel(this);
    QMovie *movie = new QMovie("C:/Users/rexgu/Documents/Whac-A-Mole/Whac-A-Mole/molegif.gif");
    if (movie->isValid()) {
        gifLabel->setMovie(movie);
        movie->start();
    } else {
        qDebug() << "Failed to load the GIF.";
    }

    gifLabel->setScaledContents(true); // Ensure the GIF scales with the label size
    int gifWidth = 170; // Set desired width for the GIF
    int gifHeight = 170; // Set desired height for the GIF
    gifLabel->setFixedSize(gifWidth, gifHeight);
    gifLabel->setAlignment(Qt::AlignCenter);


    // Create a central widget and layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // Add stretch before the GIF label to push it down
    layout->addStretch(15);

    // Add the GIF label to the layout
    layout->addWidget(gifLabel, 0, Qt::AlignCenter);

    // Initialize buttons with images
    startButton = setupButtonWithImage("C:/Users/rexgu/Documents/Whac-A-Mole/Whac-A-Mole/pressstart.png", QSize(200, 80));
    gamescoresButton = setupButtonWithImage("C:/Users/rexgu/Documents/Whac-A-Mole/Whac-A-Mole/gamescores.png", QSize(200, 80));
    exitButton = setupButtonWithImage("C:/Users/rexgu/Documents/Whac-A-Mole/Whac-A-Mole/exit.png", QSize(200, 80));

    // Add buttons to layout
    layout->addStretch(1);
    layout->addWidget(startButton, 0, Qt::AlignCenter);
    layout->addWidget(gamescoresButton, 0, Qt::AlignCenter);
    layout->addWidget(exitButton, 0, Qt::AlignCenter);
    layout->addStretch(1);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // Connect buttons to slots
    connect(startButton, &QPushButton::clicked, this, &MainWindow::on_startButton_clicked);
    connect(gamescoresButton, &QPushButton::clicked, this, &MainWindow::on_gamescoresButton_clicked);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::on_exitButton_clicked);
}

QPushButton* MainWindow::setupButtonWithImage(const QString &imagePath, const QSize &size) {
    QPushButton *button = new QPushButton(this);
    QPixmap pixmap(imagePath);
    pixmap = pixmap.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    button->setIcon(QIcon(pixmap));
    button->setIconSize(size);
    button->setFixedSize(size);
    button->setStyleSheet(
        "QPushButton { border: none; background-color: transparent; }"
        "QPushButton:pressed { background-color: rgba(255, 255, 255, 100); }"
        );

    connect(button, &QPushButton::pressed, this, [this, button]() { animateButton(button); });

    return button;
}


void MainWindow::animateButton(QPushButton* button) {
    QPropertyAnimation* animation = new QPropertyAnimation(button, "geometry");
    animation->setDuration(100);
    animation->setStartValue(button->geometry());
    QRect endGeometry = button->geometry();
    endGeometry.setWidth(endGeometry.width() - 10);
    endGeometry.setHeight(endGeometry.height() - 10);
    endGeometry.moveCenter(button->geometry().center());
    animation->setEndValue(endGeometry);

    QPropertyAnimation* revertAnimation = new QPropertyAnimation(button, "geometry");
    revertAnimation->setDuration(100);
    revertAnimation->setStartValue(endGeometry);
    revertAnimation->setEndValue(button->geometry());

    QSequentialAnimationGroup* group = new QSequentialAnimationGroup;
    group->addAnimation(animation);
    group->addAnimation(revertAnimation);
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::on_startButton_clicked() {
    QSize currentSize = this->size(); // Get the current size of MainWindow
    this->hide(); // Hide the main window

    GamePage *gamePage = new GamePage(currentSize); // Pass the size to GamePage
    gamePage->show(); // Show the game page
}

#include "scorespage.h"
// ...

void MainWindow::on_gamescoresButton_clicked() {
    QSize currentSize = this->size(); // Get the current size of MainWindow
    this->hide(); // Hide the main window

    ScoresPage *scoresPage = new ScoresPage(currentSize); // Pass the size to ScoresPage
    scoresPage->show(); // Show the scores page
}

void MainWindow::on_exitButton_clicked() {
    QApplication::quit();
}
