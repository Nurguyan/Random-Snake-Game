#include "snakebuilder.h"

SnakeBuilder::SnakeBuilder()
{
    this->reset();
}

SnakeBuilder::~SnakeBuilder()
{

}

void SnakeBuilder::setHead(QPointF head)
{
    this->snake->rhead() = head;
}

void SnakeBuilder::setTail(QPointF tail)
{
    this->snake->rtail() = tail;
}

void SnakeBuilder::appendBody(QPointF point)
{
    this->snake->body.push_back(point);
}

void SnakeBuilder::setBody(QList<QPointF> body)
{
    this->snake->body = body;
}

void SnakeBuilder::setHeadColor(QColor color)
{
    this->snake->head_color = color;
}

void SnakeBuilder::setBodyColor(QColor color)
{
    this->snake->body_color = color;
}

void SnakeBuilder::setBodyStyle(Qt::BrushStyle style)
{
    this->snake->body_style = style;
}

void SnakeBuilder::reset()
{
    this->snake = QSharedPointer<Snake>(new Snake());
}

QSharedPointer<Snake> SnakeBuilder::getSnake() const
{
    return snake;
}
