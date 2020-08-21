#include "cell.h"
#include <QPainter>


Cell::Cell(int x, int y, QGraphicsScene *parent) : QGraphicsScene(parent)
{
    idx = x;
    idy = y;
    x *= size;
    y *= size;
    visited = false;

    top.setLine(x, y, x+size, y);
    bottom.setLine(x, y+size, x+size, y+size);
    left.setLine(x, y, x, y+size);
    right.setLine(x+size, y, x+size, y+size);

    QPen pen;
    pen.setWidth(2);
    top.setPen(pen);
    bottom.setPen(pen);
    left.setPen(pen);
    right.setPen(pen);
    top.setZValue(2);
    bottom.setZValue(2);
    left.setZValue(2);
    right.setZValue(2);
}

Cell::~Cell()
{

}

bool Cell::isVisited() const
{
    return visited;
}

void Cell::setVisited(bool value)
{
    visited = value;
}

void Cell::getIndexes(int& x, int& y) const
{
    x = idx;
    y = idy;
}

void Cell::addToScene(QGraphicsScene *scene)
{
    scene->addItem(&top);
    scene->addItem(&bottom);
    scene->addItem(&left);
    scene->addItem(&right);
}

