#ifndef SNAKEDIRECTOR_H
#define SNAKEDIRECTOR_H
#include "snakebuilder.h"

class SnakeDirector
{
public:
    void setBuilder(SnakeBuilder *builder);
    void buildDarkGreenSnake();
    void buildDarkYellowSnake();

private:
    SnakeBuilder* builder;
    void setRandomHeadAndTail();
};

#endif // SNAKEDIRECTOR_H
