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

    void reverseLevelOrder(Node *root) {
        QStack<Node *> stack;
        QQueue<Node *> queue;
        queue.push_back(root);

        // Do something like normal level order traversal order. Following are the
        // differences with normal level order traversal
        // 1) Instead of printing a node, we push the node to stack
        // 2) Right subtree is visited before left subtree
        while (!queue.empty()) {
            /* Dequeue node and make it root */
            root = queue.front();
            queue.pop_front();
            stack.push_back(root);

            for (int i = 0; i < root->child.size(); ++i) {
                queue.push_back(root->child[i]);
            }
        }

        // Now pop all items from stack one by one and print them
        while (!stack.empty()) {
            root = stack.top();
            qDebug() << root->row << root->col;
            stack.pop();
        }
    }
};

#endif // TREE_H
