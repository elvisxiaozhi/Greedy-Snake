#include "widget.h"
#include "ui_widget.h"

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
    int row = rand() % rows;
    int col = rand() % cols;

    boardLblVec[row][col]->setStyleSheet("QLabel { background: yellow; }");
}
