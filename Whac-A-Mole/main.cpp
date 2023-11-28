#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    playAudio("gameSound.mp3");
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}
