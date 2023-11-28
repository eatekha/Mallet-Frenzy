#ifndef PLAYPAGE_H
#define PLAYPAGE_H

#include <QWidget>
#include "HardwareInterface.h"  // Include the HardwareInterface header

class PlayPage : public QWidget {
    Q_OBJECT

public:
    explicit PlayPage(const QSize &size, QWidget *parent = nullptr);
    ~PlayPage();  // Declare the destructor if you have specific cleanup logic

private:
    HardwareInterface* hardwareInterface;  // Declare hardwareInterface as a member variable

    // Other member variables
};

#endif // PLAYPAGE_H
