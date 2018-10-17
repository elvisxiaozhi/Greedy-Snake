#include "boardlabel.h"

BoardLabel::BoardLabel(QWidget *parent) : QLabel(parent)
{
    setStyleSheet("QLabel { border: 1px solid grey; }");
    setFixedSize(25, 25);
}
