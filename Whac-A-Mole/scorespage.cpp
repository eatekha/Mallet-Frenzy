#include "scorespage.h"
#include <QVBoxLayout>
#include <QLabel>

ScoresPage::ScoresPage(const QSize &size, QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(size); // Set the size of the ScoresPage

    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *titleLabel = new QLabel("High Scores", this);
    // Add more widgets for displaying scores

    layout->addWidget(titleLabel);
    // Add more widgets to layout
}
