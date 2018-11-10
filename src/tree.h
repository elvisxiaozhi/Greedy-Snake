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

    void levelOrderTraversal(Node *root) {
        if (root == nullptr)
            return;

        QQueue<Node *> queue;
        queue.push_back(root);

        while (!queue.empty()) {
            int n = queue.size();

            while (n > 0) {
                Node *removed = queue.front();
                queue.pop_front();

                qDebug() << removed->row << removed->col;

                for (int i = 0; i < removed->child.size(); ++i) {
                    queue.push_back(removed->child[i]);
                }

                --n;
            }
        }
    }
};

#endif // TREE_H
