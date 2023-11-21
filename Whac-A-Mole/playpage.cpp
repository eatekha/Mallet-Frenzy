#include "playpage.h"
#include <QVBoxLayout>
#include <QLabel>

PlayPage::PlayPage(const QSize &size, QWidget *parent) : QWidget(parent) {
    setFixedSize(size);
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Create and configure the label
    QLabel *label = new QLabel("This is the Play Page", this);
    layout->addWidget(label);

    setLayout(layout);
}
