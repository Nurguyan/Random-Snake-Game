#ifndef ISNAKEBUILDER_H
#define ISNAKEBUILDER_H
#include <QPointF>
#include <QColor>

class ISnakeBuilder
{
public:
    ISnakeBuilder(void);
    virtual ~ISnakeBuilder(void);
    virtual void setHead(QPointF) = 0;
    virtual void setTail(QPointF) = 0;
    virtual void setBody(QList<QPointF>) = 0;
    virtual void appendBody(QPointF) = 0;
    virtual void setHeadColor(QColor) = 0;
    virtual void setBodyColor(QColor) = 0;
    virtual void setBodyStyle(Qt::BrushStyle) = 0;
};


#endif // ISNAKEBUILDER_H
