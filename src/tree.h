#ifndef TREE_H
#define TREE_H

#include <QVector>
#include <QQueue>
#include <QDebug>
#include <memory>

using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::pair;
using std::make_pair;

struct Node
{
    int row, col, cost;
    bool visited;
    vector<shared_ptr<Node> > childVec;
    shared_ptr<Node> childNode;

    Node(int x, int y) : row(x), col(y) { visited = false; cost = INT_MAX; }

    void rootToLeaf(shared_ptr<Node> node, QVector<pair<int, int> > path, QVector<QVector<pair<int, int> > > &res) {
        if (node == nullptr)
            return;

        path.push_back(make_pair(node->row, node->col));
        if (node->childVec.empty())
            res.push_back(path);
        else {
            for (unsigned int i = 0; i < node->childVec.size(); ++i)
                rootToLeaf(node->childVec[i], path, res);
        }
    }

    shared_ptr<Node> createChild(shared_ptr<Node> node, int row, int col) {
        shared_ptr<Node> tempChild = make_shared<Node>(row, col);
        node->childVec.push_back(tempChild);
        return tempChild;
    }

    shared_ptr<Node> createChild(shared_ptr<Node> node, int row, int col, int cost) {
        shared_ptr<Node> tempChild = make_shared<Node>(row, col);
        tempChild->cost = cost;
        node->childVec.push_back(tempChild);
        return tempChild;
    }
};

#endif // TREE_H
