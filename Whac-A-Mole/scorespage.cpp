#include "scorespage.h"
#include "mainwindow.h"
#include "Hardware/HighScore.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QDebug>
#include <QDir>

ScoresPage::ScoresPage(const QSize &size, QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(size);
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *titleLabel = new QLabel("High Scores", this);
    QListWidget *scoresListWidget = new QListWidget(this);
    layout->addWidget(titleLabel);
    layout->addWidget(scoresListWidget);
    updateHighScores(scoresListWidget);
}

void ScoresPage::updateHighScores(QListWidget *scoresListWidget)
{
    HighScore highScore;
    highScore.print();
    auto scores = highScore.getHighScores(); // Use the new method

    qDebug() << "Updating high scores. Found " << scores.size() << "entries.";
    qDebug() << "Current directory:" << QDir::currentPath();

    scoresListWidget->clear();

    for (const auto &score : scores)
    {
        QString scoreEntry = QString::fromStdString(score.second + " - " + std::to_string(score.first));
        qDebug() << "Adding score to list:" << scoreEntry;
        scoresListWidget->addItem(scoreEntry);
    }
}
