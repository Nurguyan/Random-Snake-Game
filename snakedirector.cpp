#include "snakedirector.h"

void SnakeDirector::setBuilder(SnakeBuilder *builder)
{
    this->builder = builder;
}

void SnakeDirector::buildDarkGreenSnake()
{
    setRandomHeadAndTail();
    builder->setHeadColor(Qt::darkRed);
    builder->setBodyColor(Qt::darkGreen);
    builder->setBodyStyle(Qt::BrushStyle::Dense1Pattern);
}

void SnakeDirector::buildDarkYellowSnake()
{
    setRandomHeadAndTail();
    builder->setHeadColor(Qt::darkBlue);
    builder->setBodyColor(Qt::darkYellow);
    builder->setBodyStyle(Qt::BrushStyle::Dense2Pattern);
}

void SnakeDirector::setRandomHeadAndTail()
{
    //set seed
    //qsrand(uint(QTime::currentTime().msec()));

    //set random (x,y) of head's position
    QPointF head;
    head.setX(qrand()%Maze::width * Cell::size);
    head.setY(qrand()%Maze::height * Cell::size);

    //set random (x,y) of tail's position
    QPointF tail;
    tail.setX(qrand()%Maze::width * Cell::size);
    tail.setY(qrand()%Maze::height * Cell::size);

    builder->setHead(head);
    builder->setTail(tail);
}
