#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "boardlabel.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;

    static const int rows;
    static const int cols;
    int foodRow, foodCol;
    QVector<QVector<BoardLabel *> > boardLblVec;
    QVector<std::pair<int, int> > snakeVec;

    void setBoardLayout();
    void createSnake();
    void showSnake();
    void generateFood();
    void removeOldSnake();
    void gameOver();
    bool hasLost();
    bool hasFoodEaten();

protected:
    void keyPressEvent(QKeyEvent *);
};

#endif // WIDGET_H
