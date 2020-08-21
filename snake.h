#ifndef ABSTRACT_SNAKE_H
#define ABSTRACT_SNAKE_H
#include <QGraphicsItem>
#include <QRect>
#include "cell.h"
#include "maze.h"

enum class Direction {ForwardLeft, ForwardRight, ForwardUp, ForwardDown,
                      BackwardLeft, BackwardRight, BackwardUp, BackwardDown,
                      Stop};

class Snake : public QGraphicsItem
{

public:
    static constexpr int size = Cell::size;

    QList<QPointF>  body;
    QColor          head_color;
    QColor          body_color;
    Qt::BrushStyle  body_style;

    Snake();
    ~Snake() override;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget) override;

    QPointF& rhead();
    const QPointF& head() const;
    QPointF& rtail();
    const QPointF& tail() const;
    Direction getDirection() const;
    void setDirection(const Direction &direction);
    bool isGoingForward() const;
    bool isGoingBackward() const;
    bool isNotMoving() const;

protected:
    void advance(int step) override;
    QPainterPath shape() const override;

private:
    Direction       direction;

    void forwardUp();
    void forwardDown();
    void forwardLeft();
    void forwardRight();
    void backwardUp();
    void backwardDown();
    void backwardLeft();
    void backwardRight();
};

#endif // ABSTRACT_SNAKE_H
