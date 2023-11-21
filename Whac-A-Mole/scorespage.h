#ifndef SCORESPAGE_H
#define SCORESPAGE_H

#include <QWidget>

class ScoresPage : public QWidget
{
    Q_OBJECT

public:
    explicit ScoresPage(const QSize &size, QWidget *parent = nullptr);

    // Additional functions and members for displaying scores
};

#endif // SCORESPAGE_H
