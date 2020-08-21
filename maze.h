#ifndef MAZE_H
#define MAZE_H
#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStack>
#include <QVector2D>
#include <QPointer>
#include <QTime>
#include "cell.h"

class Maze : public QGraphicsScene
{
    Q_OBJECT

public:

    static constexpr int width = 20;
    static constexpr int height = 20;

    QPointer<Cell> grid[width][height];

    Maze(QObject *parent);
    ~Maze();
    bool generate();
    void clearMaze();

    QString getName() const;
    void setName(const QString &value);

private:
    void generateCell();
    Cell* getFreeNeighbour(Cell *curr);
    void removeWalls(Cell *c, Cell *n);

    QStack<Cell*> history;
    Cell *current_cell = nullptr;
    Cell *next = nullptr;
    QString name;
};

#endif
