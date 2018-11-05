#include "widget.h"
#include "ui_widget.h"
#include <QKeyEvent>
#include <QDebug>

const int Widget::ROWS = 30;
const int Widget::COLS = 30;
const int Widget::UP = 0;
const int Widget::DOWN = 1;
const int Widget::LEFT = 2;
const int Widget::RIGHT = 3;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setBoardLayout();
    createSnake();
    generateFood();

    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &Widget::whenTimeOut);

//    timer->start(500);
//    qDebug() << canFindTail();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setBoardLayout()
{
    for(int i = 0; i < ROWS; ++i) {
        QVector<BoardLabel *> colVec;
        for(int j = 0; j < COLS; ++j) {
            BoardLabel *boardLabel = new BoardLabel(this);
            ui->gridLayout->addWidget(boardLabel, i, j);
            colVec.push_back(boardLabel);
        }
        boardLblVec.push_back(colVec);
    }
}

void Widget::createSnake()
{
    snakeVec.push_back(std::make_pair(ROWS / 2, COLS / 2));
    snakeVec.push_back(std::make_pair(ROWS / 2, COLS / 2 - 1));
    snakeVec.push_back(std::make_pair(ROWS / 2, COLS / 2 - 2));

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (i != ROWS / 2 && j != COLS / 2 && j != COLS / 2 - 1 && j != COLS / 2 - 2) {
                availPlaces.push_back(std::make_pair(i, j));
            }
        }
    }

    snakeMoveDirection = RIGHT;

    showSnake();
}

void Widget::showSnake()
{
    for(int i = 0; i < snakeVec.size(); ++i) {
        if(i == 0) {
            boardLblVec[snakeVec[i].first][snakeVec[i].second]->setStyleSheet("QLabel { background: red; }");
        }
        else if (i == snakeVec.size() - 1) {
            boardLblVec[snakeVec[i].first][snakeVec[i].second]->setStyleSheet("QLabel { background: blue; }");
        }
        else {
            boardLblVec[snakeVec[i].first][snakeVec[i].second]->setStyleSheet("QLabel { background: black; }");
        }
    }
}

void Widget::generateFood()
{
    if (availPlaces.size() > 0) {
        int i = rand() % availPlaces.size();
        foodRow = availPlaces[i].first;
        foodCol = availPlaces[i].second;
    }

    boardLblVec[foodRow][foodCol]->setStyleSheet("QLabel { background: yellow; }");
}

void Widget::removeOldSnake()
{
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (i == foodRow && j == foodCol) {
                continue;
            }
            else {
                boardLblVec[i][j]->setText("");
                boardLblVec[i][j]->setStyleSheet("QLabel { border: 1px solid grey; }");
            }
        }
    }
}

void Widget::gameOver()
{
    qDebug() << "Lost";
}

bool Widget::hasLost()
{
    if (snakeVec[0].first < 0 || snakeVec[0].first >= ROWS || snakeVec[0].second < 0 || snakeVec[0].second >= COLS) {
        return true;
    }

    auto p = std::make_pair(snakeVec[0].first, snakeVec[0].second);

    if (std::find(snakeVec.begin() + 1, snakeVec.end(), p) != snakeVec.end()) {
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

void Widget::moveSnake(int direction)
{
    removeOldSnake();

    //move snake head
    switch (direction) {
    case UP:
        snakeVec.push_front(std::make_pair(snakeVec[0].first - 1, snakeVec[0].second));
        break;
    case DOWN:
        snakeVec.push_front(std::make_pair(snakeVec[0].first + 1, snakeVec[0].second));
        break;
    case LEFT:
        snakeVec.push_front(std::make_pair(snakeVec[0].first, snakeVec[0].second - 1));
        break;
    case RIGHT:
        snakeVec.push_front(std::make_pair(snakeVec[0].first, snakeVec[0].second + 1));
        break;
    default:
        break;
    }

    //move snake body
    if (hasFoodEaten()) {
        auto p = std::make_pair(snakeVec[0].first, snakeVec[0].second);
        availPlaces.erase(std::remove(availPlaces.begin(), availPlaces.end(), p), availPlaces.end());

        generateFood();
    }
    else {
        availPlaces.push_back(std::make_pair(snakeVec[snakeVec.size() - 1].first, snakeVec[snakeVec.size() - 1].second));
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

bool Widget::canFindTail()
{
    auto it = std::make_pair(foodRow, foodCol);
    if (std::find(queue.begin(), queue.end(), it) != queue.end()) {
        return true;
    }

    return false;
}

bool Widget::canMoveThere(int row, int col)
{
    if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
        auto it = std::make_pair(row, col);
        if (std::find(searchOutput.begin(), searchOutput.end(), it) != searchOutput.end()) {
            return false;
        }
        else {
//            if (std::find(snakeVec.begin() + 1, snakeVec.end() - 1))
            return true;
        }
    }

    return false;
}

void Widget::BFS(int row, int col)
{
    if (canFindTail()) {
        return;
    }

    searchOutput.push_back(std::make_pair(row, col));

    if (queue.empty()) {
        if (canMoveThere(row - 1, col)) {
            queue.push_back(std::make_pair(row - 1, col));
        }
        if (canMoveThere(row + 1, col)) {
            queue.push_back(std::make_pair(row + 1, col));
        }
        if (canMoveThere(row, col - 1)) {
            queue.push_back(std::make_pair(row, col - 1));
        }
        if (canMoveThere(row, col + 1)) {
            queue.push_back(std::make_pair(row, col + 1));
        }
    }
    else {
        while (!queue.empty()) {
//            BFS (queue.first().first)
        }
    }
}

//void Widget::mapScores(int row, int col)
//{
//    for (int i = 0; i < rows; ++i) {
//        for (int j = 0; j < cols; ++j) {
//            auto p = std::make_pair(i, j);

//            if (std::find(snakeVec.begin(), snakeVec.end(), p) != snakeVec.end()) {
//                continue;
//            }
//            else if (i == foodRow && j == foodCol) {
//                continue;
//            }
//            else {
//                int score = 0;
//                score = std::abs(i - row) + std::abs(j - col);
//                boardLblVec[i][j]->setText(QString::number(score));
//            }
//        }
//    }
//}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down || event->key() == Qt::Key_Left || event->key() == Qt::Key_Right) {
        if (event->key() == Qt::Key_Up) {
            snakeMoveDirection = UP;
        }
        else if (event->key() == Qt::Key_Down) {
            snakeMoveDirection = DOWN;
        }
        else if (event->key() == Qt::Key_Left) {
            snakeMoveDirection = LEFT;
        }
        else if (event->key() == Qt::Key_Right) {
            snakeMoveDirection = RIGHT;
        }

        moveSnake(snakeMoveDirection);
    }
}

void Widget::whenTimeOut()
{
    moveSnake(snakeMoveDirection);
}
