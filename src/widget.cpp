#include "widget.h"
#include "ui_widget.h"
#include <QKeyEvent>
#include <QDebug>
#include <QQueue>

const int Widget::ROWS = 10;
const int Widget::COLS = 10;
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
    timer->start(100);

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
    moveSnakeHead(direction, snakeVec);

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

void Widget::moveSnakeHead(int direction, QVector<std::pair<int, int> > &snake)
{
    switch (direction) {
    case UP:
        snake.push_front(std::make_pair(snake[0].first - 1, snake[0].second));
        break;
    case DOWN:
        snake.push_front(std::make_pair(snake[0].first + 1, snake[0].second));
        break;
    case LEFT:
        snake.push_front(std::make_pair(snake[0].first, snake[0].second - 1));
        break;
    case RIGHT:
        snake.push_front(std::make_pair(snake[0].first, snake[0].second + 1));
        break;
    default:
        break;
    }
}

void Widget::moveVirtualSnake(int direction, QVector<std::pair<int, int> > &snake)
{
    moveSnakeHead(direction, snake);
    snake.pop_back();
}

bool Widget::canFindTail()
{
    return false;
}

void Widget::resetVisited()
{
    for (int i = 0; i < boardLblVec.size(); ++i) {
        for (int j = 0; j < boardLblVec[i].size(); ++j) {
            boardLblVec[i][j]->visited = false;
        }
    }

    for (int i = 0; i < snakeVec.size(); ++i) {
        boardLblVec[snakeVec[i].first][snakeVec[i].second]->visited = true;
    }
}

QVector<std::pair<int, int> > Widget::returnNbrPlaces(int row, int col)
{
    QVector<std::pair<int, int> > res;

    if (row - 1 >= 0) {
        if (boardLblVec[row - 1][col]->visited == false) {
            res.push_back(std::make_pair(row - 1, col));
        }
    }
    if (row + 1 < ROWS) {
        if (boardLblVec[row + 1][col]->visited == false) {
            res.push_back(std::make_pair(row + 1, col));
        }
    }
    if (col - 1 >= 0) {
        if (boardLblVec[row][col - 1]->visited == false) {
            res.push_back(std::make_pair(row, col - 1));
        }
    }
    if (col + 1 < COLS) {
        if (boardLblVec[row][col + 1]->visited == false) {
            res.push_back(std::make_pair(row, col + 1));
        }
    }

    return res;
}

void Widget::BFS(int row, int col)
{
    resetVisited();

//    if (root != nullptr)
//        root->deleteTree(root);

    QQueue<Node *> queue;
    root = new Node(row, col);
    queue.push_back(root);

    Node *curr = nullptr;

    while (!queue.empty()) {
        row = queue.front()->row;
        col = queue.front()->col;
        curr = queue.front();
        queue.pop_front();

        boardLblVec[row][col]->visited = true;

        QVector<std::pair<int, int> > neighbors = returnNbrPlaces(row, col);

        for (int i = 0; i < neighbors.size(); ++i) {
            boardLblVec[neighbors[i].first][neighbors[i].second]->visited = true;
            queue.push_back(root->makeChild(curr, neighbors[i].first, neighbors[i].second));
            if (neighbors[i].first == foodRow && neighbors[i].second == foodCol) {
                return;
            }
        }
    }

    delete curr;
}

QVector<std::pair<int, int> > Widget::returnFindFoodPath()
{
    QVector<std::pair<int, int> > path;
    path.clear();

    root->rootToLeaf(root, path, res);

    for (int i = 0; i < res.size(); ++i) {
        if (canFindObject(foodRow, foodCol, res[i])) {
            path.swap(res[i]);
            return path;
        }
    }

    return path;
}

bool Widget::canFindObject(int row, int col, QVector<std::pair<int, int> > mVec)
{
    auto it = std::make_pair(row, col);

    if (std::find(mVec.begin(), mVec.end(), it) != mVec.end())
        return true;

    return false;
}

void Widget::getSnakeMoveDirection()
{
    BFS(snakeVec[0].first, snakeVec[0].second);
    QVector<std::pair<int, int> > path;
    res.clear();
    root->rootToLeaf(root, path, res);

    path = returnFindFoodPath();

    qDebug() << path;

    if (!path.empty()) {
        int row = path[1].first;
        int col = path[1].second;

        if (row > snakeVec[0].first)
            snakeMoveDirection = DOWN;
        if (row < snakeVec[0].first)
            snakeMoveDirection = UP;
        if (col > snakeVec[0].second)
            snakeMoveDirection = RIGHT;
        if (col < snakeVec[0].second)
            snakeMoveDirection = LEFT;
    }

    qDebug() << snakeMoveDirection;
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

    }
//    getSnakeMoveDirection();
//    moveSnake(snakeMoveDirection);
}

void Widget::whenTimeOut()
{

    getSnakeMoveDirection();

    moveSnake(snakeMoveDirection);
    QVector<std::pair<int, int> > snakeVecCopy(snakeVec);
    moveVirtualSnake(snakeMoveDirection, snakeVecCopy);
}
