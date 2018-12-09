#ifndef TREE_H
#define TREE_H

#include <QVector>
#include <QQueue>
#include <QDebug>

struct Node
{
    int row, col, cost;
    bool visited;
    QVector<Node *> child;
    Node *childNode;
    Node(int x, int y) : row(x), col(y), childNode(nullptr) { visited = false; cost = INT_MAX; }

    void rootToLeaf(Node *node, QVector<std::pair<int, int> > path, QVector<QVector<std::pair<int, int> > > &res) {
        if (node == nullptr)
            return;

        path.push_back(std::make_pair(node->row, node->col));

        if (node->child.empty()) {
            res.push_back(path);
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

    Node *returnMinCostOfUnvisitedNode(Node *root) {
        if (root == nullptr)
            return root;

        Node *res = nullptr;
        int minCost = INT_MAX;

        QQueue<Node *> queue;
        queue.push_back(root);

        Node *curr = nullptr;

        while (queue.empty() == false) {
            int n = queue.size();

            while (n > 0) {
                curr = queue.front();
                queue.pop_front();

                if (curr->visited == false) {
                    if (minCost > curr->cost) {
                        res = curr;
                        minCost = curr->cost;
                    }
                }

                for (int i = 0; i < curr->child.size(); ++i) {
                    if (curr->child[i] != nullptr) {
                        queue.push_back(curr->child[i]);
                    }
                }

                --n;
            }
        }

        delete curr;

        return res;
    }

    int returnTreeSize(Node* node) {
        if (node == nullptr)
            return 0;
        else {
            int s = 0;
            for (int i = 0; i < node->child.size(); ++i) {
                s += returnTreeSize(node->child[i]);
            }

            return s + 1;
        }
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
