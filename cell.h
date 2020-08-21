#ifndef CELL_H
#define CELL_H
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>


class Cell : public QGraphicsScene {

    Q_OBJECT

public:

    static constexpr int size = 25;

    QGraphicsLineItem top;     //top line
    QGraphicsLineItem right;   //right  line
    QGraphicsLineItem bottom;  //bottom line
    QGraphicsLineItem left;    //left line

    Cell(int x, int y, QGraphicsScene *parent = nullptr);
    ~Cell();

    bool isVisited() const;
    void setVisited(bool value);
    void getIndexes(int&, int&) const;
    void addToScene(QGraphicsScene *scene);

private:
    bool visited;
    int idx;
    int idy;
};
#endif
