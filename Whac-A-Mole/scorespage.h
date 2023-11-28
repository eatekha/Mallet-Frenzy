#ifndef SCORESPAGE_H
#define SCORESPAGE_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QListWidget>
#include <vector>
#include <string>

class ScoresPage : public QWidget
{
    Q_OBJECT

public:
    explicit ScoresPage(const QSize &size, QWidget *parent = nullptr);

    // Function to update the high scores displayed on the page
    void updateHighScores(QListWidget* scoresListWidget);

private:
    QVBoxLayout* layout; // Layout to organize widgets
    QLabel* titleLabel;  // Label to display the title of the page
};

#endif // SCORESPAGE_H
