#ifndef SCORESPAGE_H
#define SCORESPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidget>

class ScoresPage : public QWidget
{
    Q_OBJECT

public:
    explicit ScoresPage(const QSize &size, QWidget *parent = nullptr);

    // Function to update the high scores displayed on the page
    void updateHighScores(QListWidget* scoresListWidget);

signals:
    void returnToMainMenu(); // Signal to indicate a return to the main menu

private slots:
    void onReturnButtonClicked(); // Slot for handling return button click

private:
    QVBoxLayout* layout;       // Layout to organize widgets
    QLabel* titleLabel;        // Label to display the title of the page
    QPushButton* returnButton; // Button to return to the main menu
    QListWidget* scoresListWidget; // List widget to display scores
};

#endif // SCORESPAGE_H

