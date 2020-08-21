#ifndef SNAKEBUILDER_H
#define SNAKEBUILDER_H
#include <QSharedPointer>
#include "isnakebuilder.h"
#include "snake.h"

class SnakeBuilder : public ISnakeBuilder
{
private:
    QSharedPointer<Snake> snake;

public:
    SnakeBuilder();
    ~SnakeBuilder() override;
    void setHead(QPointF) override;
    void setTail(QPointF) override;
    void appendBody(QPointF) override;
    void setBody(QList<QPointF>) override;
    void setHeadColor(QColor) override;
    void setBodyColor(QColor) override;
    void setBodyStyle(Qt::BrushStyle) override;
    void reset();
    QSharedPointer<Snake> getSnake() const;
};

#endif // SNAKEBUILDER_H
