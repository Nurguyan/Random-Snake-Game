#include "snake.h"
#include <QPainter>
#include <QDebug>

Snake::Snake() : direction(Direction::Stop)
{
    body.push_back(QPointF(0,0));         //deafult head position
    body.push_back(QPointF(size,size));   //deafult tail position
}

void Snake::advance(int step)
{
    if (!step) return;

    switch (direction) {
    case Direction::ForwardLeft:
        forwardLeft();
        break;
    case Direction::ForwardRight:
        forwardRight();
        break;
    case Direction::ForwardUp:
        forwardUp();
        break;
    case Direction::ForwardDown:
        forwardDown();
        break;
    case Direction::BackwardLeft:
        backwardLeft();
        break;
    case Direction::BackwardRight:
        backwardRight();
        break;
    case Direction::BackwardUp:
        backwardUp();
        break;
    case Direction::BackwardDown:
        backwardDown();
        break;
    case Direction::Stop:
        return;
    }

    setPos(head());
    //handleCollisions();
}

QRectF Snake::boundingRect() const
{
    qreal minX = head().x();
    qreal minY = head().y();
    qreal maxX = head().x();
    qreal maxY = head().y();

    for (const QPointF& p : body) {
        maxX = p.x() > maxX ? p.x() : maxX;
        maxY = p.y() > maxY ? p.y() : maxY;
        minX = p.x() < minX ? p.x() : minX;
        minY = p.y() < minY ? p.y() : minY;
    }

    QPointF topleft = mapFromScene(QPointF(minX, minY));
    QPointF bottomright = mapFromScene(QPointF(maxX, maxY));

    QRectF rect = QRectF(topleft.x(),                           //x
                         topleft.y(),                           //y
                         bottomright.x() - topleft.x() + size,  //width
                         bottomright.y() - topleft.y() + size); //height
    return rect;
}

QPainterPath Snake::shape() const
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    //add body
    for (const QPointF& p : body) {
        QPointF body_part_pos = mapFromScene(p);
        path.addRect(QRectF(body_part_pos.x(), body_part_pos.y(), size, size));
    }
    return path;
}

void Snake::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();
    painter->fillPath(shape(), QBrush(body_color, body_style));
    QPointF head_pos = mapFromScene(head());
    painter->fillRect(QRectF(head_pos.x(),head_pos.y(), size,  size), head_color);
    painter->restore();
}

QPointF& Snake::rhead()
{
    return body.first();
}

const QPointF& Snake::head() const
{
    return body.first();
}

QPointF& Snake::rtail()
{
    return body.last();
}

const QPointF& Snake::tail() const
{
    return body.last();
}

Direction Snake::getDirection() const
{
    return direction;
}

void Snake::setDirection(const Direction &new_direction)
{
   /* auto afterhead = body.at(1);
    auto beforetail = body.at(body.size()-2);

    switch(new_direction){
    case Direction::ForwardLeft:
        if (afterhead.x() > head().x()) return;
        break;
    case Direction::ForwardRight:
        if (afterhead.x() < head().x()) return;
        break;
    case Direction::ForwardUp:
        if (afterhead.y() > head().y()) return;
        break;
    case Direction::ForwardDown:
        if (afterhead.y() > head().y()) return;
        break;
    case Direction::BackwardLeft:
        if (beforetail.x() > tail().x()) return;
        break;
    case Direction::BackwardRight:
        if (beforetail.x() < tail().x()) return;
        break;
    case Direction::BackwardUp:
        if (beforetail.y() > tail().y()) return;
        break;
    case Direction::BackwardDown:
        if (beforetail.y() < tail().y()) return;
        break;
    case Direction::Stop:
        return;
    }
*/
    direction = new_direction;
}

bool Snake::isGoingForward() const
{
    return (direction == Direction::ForwardUp || direction == Direction::ForwardDown
            || direction == Direction::ForwardLeft || direction == Direction::ForwardRight);
}

bool Snake::isGoingBackward() const
{
    return (direction == Direction::BackwardUp || direction == Direction::BackwardDown
            || direction == Direction::BackwardLeft || direction == Direction::BackwardRight);
}

bool Snake::isNotMoving() const
{
    return (direction == Direction::Stop);
}

void Snake::forwardUp()
{
    if (!body.isEmpty())
    {
        body.removeLast();
        QPointF next = head();
        next.ry() -= size;
        body.push_front(next);
    }
}

void Snake::forwardDown()
{
    if (!body.isEmpty())
    {
        body.removeLast();
        QPointF next = head();
        next.ry() += size;
        body.push_front(next);
    }
}

void Snake::forwardLeft()
{
    if (!body.isEmpty())
    {
        body.removeLast();
        QPointF next = head();
        next.rx() -= size;
        body.push_front(next);
    }
}

void Snake::forwardRight()
{
    if (!body.isEmpty())
    {
        body.removeLast();
        QPointF next = head();
        next.rx() += size;
        body.push_front(next);
    }
}

void Snake::backwardUp()
{
    if (!body.isEmpty())
    {
        body.removeFirst();
        QPointF next = tail();
        next.ry() -= size;
        body.push_back(next);
    }
}

void Snake::backwardDown()
{
    if (!body.isEmpty())
    {
        body.removeFirst();
        QPointF next = tail();
        next.ry() += size;
        body.push_back(next);
    }
}

void Snake::backwardLeft()
{
    if (!body.isEmpty())
    {
        body.removeFirst();
        QPointF next = tail();
        next.rx() -= size;
        body.push_back(next);
    }
}

void Snake::backwardRight()
{
    if (!body.isEmpty())
    {
        body.removeFirst();
        QPointF next = tail();
        next.rx() += size;
        body.push_back(next);
    }
}

Snake::~Snake()
{

}

