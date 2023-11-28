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
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

bool playAudio(const std::string &audioPath)
{
    // Initialize SDL for audio
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    // Load the music
    Mix_Music *music = Mix_LoadMUS(audioPath.c_str());
    if (music == NULL)
    {
        std::cerr << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        Mix_CloseAudio();
        SDL_Quit();
        return false;
    }

    // Play the music once
    if (Mix_PlayMusic(music, 0) == -1)
    {
        std::cerr << "Failed to play music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        SDL_Quit();
        return false;
    }

    return true;
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    ////////////////////////////////////////////////////////////////////
    // Set window title and fixed window size
    setWindowTitle("Whac-A-Mole Game");
    setFixedSize(900, 758);

    // Set background image
    QPixmap background("mainbgimage.png");
    QLabel *backgroundLabel = new QLabel(this);
    backgroundLabel->setPixmap(background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    backgroundLabel->setScaledContents(true);
    backgroundLabel->setGeometry(this->rect());

    // Create a QLabel for the GIF
    QLabel *gifLabel = new QLabel(this);
    QMovie *movie = new QMovie("molegif.gif");
    if (movie->isValid())
    {
        gifLabel->setMovie(movie);
        movie->start();
    }
    else
    {
        qDebug() << "Failed to load the GIF.";
    }

    gifLabel->setScaledContents(true); // Ensure the GIF scales with the label size
    int gifWidth = 170;                // Set desired width for the GIF
    int gifHeight = 170;               // Set desired height for the GIF
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
    startButton = setupButtonWithImage("pressstart.png", QSize(200, 80));
    gamescoresButton = setupButtonWithImage("gamescores.png", QSize(200, 80));
    exitButton = setupButtonWithImage("exit.png", QSize(200, 80));
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

QPushButton *MainWindow::setupButtonWithImage(const QString &imagePath, const QSize &size)
{
    QPushButton *button = new QPushButton(this);
    QPixmap pixmap(imagePath);
    pixmap = pixmap.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    button->setIcon(QIcon(pixmap));
    button->setIconSize(size);
    button->setFixedSize(size);

    button->setStyleSheet(
        "QPushButton { border: none; background-color: transparent; }"
        "QPushButton:pressed { background-color: rgba(255, 255, 255, 100); }");

    connect(button, &QPushButton::pressed, this, [this, button]()
            { animateButton(button); 
            playAudio("Click.wav"); });

    return button;
}

void MainWindow::animateButton(QPushButton *button)
{
    QPropertyAnimation *animation = new QPropertyAnimation(button, "geometry");
    animation->setDuration(100);
    animation->setStartValue(button->geometry());
    QRect endGeometry = button->geometry();
    endGeometry.setWidth(endGeometry.width() - 10);
    endGeometry.setHeight(endGeometry.height() - 10);
    endGeometry.moveCenter(button->geometry().center());
    animation->setEndValue(endGeometry);

    QPropertyAnimation *revertAnimation = new QPropertyAnimation(button, "geometry");
    revertAnimation->setDuration(100);
    revertAnimation->setStartValue(endGeometry);
    revertAnimation->setEndValue(button->geometry());

    QSequentialAnimationGroup *group = new QSequentialAnimationGroup;
    group->addAnimation(animation);
    group->addAnimation(revertAnimation);
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::on_startButton_clicked()
{
    QSize currentSize = this->size(); // Get the current size of MainWindow
    this->hide();                     // Hide the main window

    GamePage *gamePage = new GamePage(currentSize); // Pass the size to GamePage
    gamePage->show();                               // Show the game page
}

#include "scorespage.h"
// ...

void MainWindow::on_gamescoresButton_clicked()
{
    QSize currentSize = this->size(); // Get the current size of MainWindow
    this->hide();                     // Hide the main window

    ScoresPage *scoresPage = new ScoresPage(currentSize); // Pass the size to ScoresPage
    scoresPage->show();                                   // Show the scores page
}

void MainWindow::on_exitButton_clicked()
{
    playAudio("Click.wav");
    QApplication::quit();
}
