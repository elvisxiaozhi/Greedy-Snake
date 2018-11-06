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
    return false;
}

void Widget::markAllVisited()
{
    for (int i = 0; i < snakeVec.size(); ++i) {
        boardLblVec[snakeVec[i].first][snakeVec[i].second]->visited = true;
    }
}

void Widget::unmarkAllVisited()
{
    for (int i = 0; i < boardLblVec.size(); ++i) {
        for (int j = 0; j < boardLblVec[i].size(); ++j) {
            boardLblVec[i][j]->visited = false;
        }
    }

    markAllVisited();
}

bool Widget::canMoveThere(int row, int col)
{
    if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
        if (boardLblVec[row][col]->visited == false) {
            return true;
        }
    }

    return false;
}

void Widget::BFS(int row, int col)
{
    QVector<std::pair<int, int> > queue;

    queue.push_back(std::make_pair(row, col));

    while (!queue.empty()) {
        int queueRow = queue.front().first;
        int queueCol = queue.front().second;
        queue.pop_front();

        if (queueRow == foodRow && queueCol == foodCol) {
            break;
        }

        boardLblVec[queueRow][queueCol]->visited = true;

        if (canMoveThere(queueRow - 1, queueCol)) {
            queue.push_back(std::make_pair(queueRow - 1, queueCol));
        }
        else if (canMoveThere(queueRow + 1, queueCol)) {
            queue.push_back(std::make_pair(queueRow + 1, queueCol));
        }
        else if (canMoveThere(queueRow, queueCol - 1)) {
            queue.push_back(std::make_pair(queueRow, queueCol - 1));
        }
        else if (canMoveThere(queueRow, queueCol + 1)) {
            queue.push_back(std::make_pair(queueRow, queueCol + 1));
        }
    }
}

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
        BFS(snakeVec[0].first, snakeVec[0].second);
        unmarkAllVisited();
    }
}

void Widget::whenTimeOut()
{
    moveSnake(snakeMoveDirection);
}
