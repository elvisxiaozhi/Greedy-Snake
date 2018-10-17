#include "widget.h"
#include "ui_widget.h"
#include <QKeyEvent>
#include <QDebug>

const int Widget::rows = 30;
const int Widget::cols = 30;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setBoardLayout();
    createSnake();
    generateFood();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setBoardLayout()
{
    for(int i = 0; i < rows; ++i) {
        QVector<BoardLabel *> colVec;
        for(int j = 0; j < cols; ++j) {
            BoardLabel *boardLabel = new BoardLabel(this);
            ui->gridLayout->addWidget(boardLabel, i, j);
            colVec.push_back(boardLabel);
        }
        boardLblVec.push_back(colVec);
    }
}

void Widget::createSnake()
{
    snakeVec.push_back(std::make_pair(rows / 2, cols / 2));
    snakeVec.push_back(std::make_pair(rows / 2, cols / 2 - 1));
    snakeVec.push_back(std::make_pair(rows / 2, cols / 2 - 2));

    showSnake();
}

void Widget::showSnake()
{
    for(int i = 0; i < snakeVec.size(); ++i) {
        if(i == 0) {
            boardLblVec[snakeVec[i].first][snakeVec[i].second]->setStyleSheet("QLabel { background: red; }");
        }
        else {
            boardLblVec[snakeVec[i].first][snakeVec[i].second]->setStyleSheet("QLabel { background: black; }");
        }
    }
}

void Widget::generateFood()
{
    if (snakeVec.size() <= rows * cols) {
        foodRow = rand() % rows;
        foodCol = rand() % cols;
    }

    boardLblVec[foodRow][foodCol]->setStyleSheet("QLabel { background: yellow; }");
}

void Widget::removeOldSnake()
{
    for(int i = 0; i < snakeVec.size(); ++i) {
        boardLblVec[snakeVec[i].first][snakeVec[i].second]->setStyleSheet("QLabel { border: 1px solid grey; }");
    }
}

void Widget::gameOver()
{
    qDebug() << "Lost";
}

bool Widget::hasLost()
{
    if (snakeVec[0].first < 0 || snakeVec[0].first >= rows || snakeVec[0].second < 0 || snakeVec[0].second >= cols) {
        return true;
    }

    return false;
}

bool Widget::hasFoodEaten()
{
    if (snakeVec[0].first == foodRow && snakeVec[0].second == foodCol) {
        return true;
    }

    return false;
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down || event->key() == Qt::Key_Left || event->key() == Qt::Key_Right) {
        removeOldSnake();

        if (event->key() == Qt::Key_Up) {
            snakeVec.push_front(std::make_pair(snakeVec[0].first - 1, snakeVec[0].second));
        }
        else if (event->key() == Qt::Key_Down) {
            snakeVec.push_front(std::make_pair(snakeVec[0].first + 1, snakeVec[0].second));
        }
        else if (event->key() == Qt::Key_Left) {
            snakeVec.push_front(std::make_pair(snakeVec[0].first, snakeVec[0].second - 1));
        }
        else if (event->key() == Qt::Key_Right) {
            snakeVec.push_front(std::make_pair(snakeVec[0].first, snakeVec[0].second + 1));
        }

        if (hasFoodEaten()) {
            generateFood();
        }
        else {
            snakeVec.pop_back();
        }

        if (hasLost()) {
            snakeVec.pop_front();
            showSnake();
            boardLblVec[snakeVec[0].first][snakeVec[0].second]->setStyleSheet("QLabel { background: black; }");

            gameOver();
        }
        else {
            showSnake();
        }
    }
}
