#ifndef BOARDLABEL_H
#define BOARDLABEL_H

#include <QObject>
#include <QLabel>

class BoardLabel : public QLabel
{
    Q_OBJECT
public:
    explicit BoardLabel(QWidget *parent = nullptr);
    bool visited;

signals:

public slots:
};

#endif // BOARDLABEL_H
