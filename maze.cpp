#include "maze.h"
#include <QDebug>
#include <QtAlgorithms>
Maze::Maze(QObject *parent) : QGraphicsScene(parent), name("snake maze")
{
    setSceneRect(0, 0, width * Cell::size, height * Cell::size);
    setBackgroundBrush(QBrush(QColor(161, 171, 186), Qt::SolidPattern));
}

Maze::~Maze()
{

}

bool Maze::generate()
{
    try {
        //setup grid and prepare first cell
        for(int i = 0; i < width; i++)
        {
            for(int j = 0; j < height; j++)
            {
                grid[i][j] = new Cell(i, j, this);
                grid[i][j]->addToScene(this);
            }
        }

        current_cell = grid[0][0];
        current_cell->setVisited(true);

        history.push(current_cell);

        while (!history.isEmpty()){
            generateCell();
        }
    }
    catch (...) {
        qDebug() << "Could not generate Maze";
        return false;
    }
    return true;
}

void Maze::clearMaze()
{
    for(int i = 0; i < width; ++i)
    {
        for(int j = 0; j < height; ++j)
        {
            auto& cell = grid[i][j];
            if (!cell.isNull())
            {
                removeItem(&cell->top);
                removeItem(&cell->bottom);
                removeItem(&cell->right);
                removeItem(&cell->left);
            }
            cell.clear();
        }
    }

}

void Maze::generateCell()
{
    next = getFreeNeighbour(current_cell);
    if(next != nullptr)
    {
        history.push(next);
        next->setVisited(true);
        removeWalls(current_cell, next);
        current_cell = next;
    }
    else if(!history.isEmpty())
    {
        //backtracing
        current_cell = history.pop();
    }
}

void Maze::removeWalls(Cell *c, Cell *n)
{
    int cx, cy, nx, ny;
    c->getIndexes(cx, cy);
    n->getIndexes(nx, ny);

    switch(cx-nx)
    {
    case 1:
        c->left.hide();
        n->right.hide();
        removeItem(&n->right);
        removeItem(&c->left);
        break;
    case -1:
        c->right.hide();
        n->left.hide();
        /*remove*/
        removeItem(&c->right);
        removeItem(&n->left);
        break;
    }

    switch(cy-ny)
    {
    case 1:
        c->top.hide();
        n->bottom.hide();
        removeItem(&c->top);
        removeItem(&n->bottom);
        break;
    case -1:
        c->bottom.hide();
        n->top.hide();
        removeItem(&c->bottom);
        removeItem(&n->top);
        break;
    }
}

QString Maze::getName() const
{
    return name;
}

void Maze::setName(const QString &value)
{
    name = value;
}

Cell * Maze::getFreeNeighbour(Cell * curr)
{
    int curX, curY;
    QVector<Cell *> available;
    curr->getIndexes(curX, curY);

    if(curX > 0 && !grid[curX-1][curY]->isVisited())        //left
    {
        available.push_back(grid[curX-1][curY]);
    }
    if(curX < width-1 && !grid[curX+1][curY]->isVisited())  // right
    {
        available.push_back(grid[curX+1][curY]);
    }
    if(curY > 0 && !grid[curX][curY-1]->isVisited())        //up
    {
        available.push_back(grid[curX][curY-1]);
    }
    if(curY < height-1 && !grid[curX][curY+1]->isVisited()) //down
    {
        available.push_back(grid[curX][curY+1]);
    }

    int len = available.size();
    if(len)
    {
        return available[qrand() % len];
    }
    return nullptr;
}
