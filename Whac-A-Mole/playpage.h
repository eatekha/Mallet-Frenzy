#ifndef PLAYPAGE_H
#define PLAYPAGE_H

#include <QWidget>

class PlayPage : public QWidget
{
    Q_OBJECT

public:
    explicit PlayPage(const QSize &size, QWidget *parent = nullptr);
    // Additional functions and members for play logic
};

#endif // PLAYPAGE_H
