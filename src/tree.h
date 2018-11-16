#ifndef TREE_H
#define TREE_H

#include <QVector>
#include <QQueue>
#include <QDebug>

struct Node
{
    int row, col;
    bool visited;
    QVector<Node *> child;
    Node *childNode;
    Node(int x, int y) : row(x), col(y), childNode(nullptr) { visited = false; }

    void rootToLeaf(Node *node, QVector<std::pair<int, int> > path, QVector<std::pair<int, int> > &res) {
        if (node == nullptr)
            return;

        path.push_back(std::make_pair(node->row, node->col));

        if (node->child.empty()) {
            path.push_back(std::make_pair(INT_MAX, INT_MAX));
            int i;
            int s = path.size();
            for (i = 0; i < s; ++i) {
                res.push_back(std::make_pair(path[i].first, path[i].second));
            }
        }
        else {
            for (int i = 0; i < node->child.size(); ++i) {
                rootToLeaf(node->child[i], path, res);
            }
        }
    }

    Node *makeChild(Node *root, int row, int col) {
        root->child.push_back(new Node(row, col));
        Node *temp = root->child[root->child.size() - 1];
        return temp;
    }


    void deleteTree(Node *node) {
        if (node == nullptr)
            return;

        for (int i = 0; i < node->child.size(); ++i) {
            deleteTree(node->child[i]);
        }

        delete node;

        for (int i = 0; i < node->child.size(); ++i) {
            node->child[i] = nullptr;
        }

        node = nullptr;
    }
};

#endif // TREE_H
