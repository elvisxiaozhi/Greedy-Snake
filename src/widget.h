#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "boardlabel.h"
#include <QTimer>

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

    static const int ROWS;
    static const int COLS;
    static const int UP;
    static const int DOWN;
    static const int LEFT;
    static const int RIGHT;
    int foodRow, foodCol;
    QVector<QVector<BoardLabel *> > boardLblVec;
    QVector<std::pair<int, int> > snakeVec;
    QVector<std::pair<int, int> > availPlaces;
    QTimer *timer;
    int snakeMoveDirection;
    QVector<std::pair<int, int> > queue;
    QVector<std::pair<int, int> > searchOutput;

    void setBoardLayout();
    void createSnake();
    void showSnake();
    void generateFood();
    void removeOldSnake();
    void gameOver();
    bool hasLost();
    bool hasFoodEaten();
    void moveSnake(int);
    void mapScores(int, int);
    bool canFindTail();
    bool canMoveThere(int, int);
    void BFS(int, int);


protected:
    void keyPressEvent(QKeyEvent *);

private slots:
    void whenTimeOut();
};

#endif // WIDGET_H
