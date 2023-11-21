#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QPushButton>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void on_startButton_clicked();
    void on_gamescoresButton_clicked();
    void on_exitButton_clicked();

private:
    QPushButton *setupButtonWithImage(const QString &imagePath, const QSize &size); // Include QSize parameter

    QPushButton *startButton;

    QPushButton *gamescoresButton;
    QPushButton *exitButton;

    void animateButton(QPushButton* button);
};

#endif // MAINWINDOW_H
