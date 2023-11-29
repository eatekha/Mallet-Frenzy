#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include "gamepage.h"
#include "scorespage.h"
#include <string>

bool playAudio(const std::string &audioPath);
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void on_startButton_clicked();
    void on_gamescoresButton_clicked();
    void on_exitButton_clicked();
    void returnToMainMenu(); // Slot to handle the return from ScoresPage

private:
    QPushButton* setupButtonWithImage(const QString &imagePath, const QSize &size);
    void animateButton(QPushButton* button);

    QPushButton *startButton;
    QPushButton *gamescoresButton;
    QPushButton *exitButton;

    ScoresPage *scoresPage; // Pointer to the ScoresPage
};

#endif // MAINWINDOW_H
