#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H
#include <QObject>
#include <QSharedPointer>
#include <QTimer>
#include "snake.h"
#include "snakebuilder.h"
#include "snakedirector.h"
#include "maze.h"


class GameController : public QObject
{
    Q_OBJECT

public:
    GameController(QObject *parent, int number_of_snakes);
    ~GameController();
    static QPoint pointToCell(const QPointF& point);
    static QPointF cellToPoint(const QPoint& cell);
    bool generateSnakes();
    Maze* getMaze();
    QList<QSharedPointer<Snake>>* getSnakes();

private:
    int num_snakes;
    SnakeBuilder builder;
    SnakeDirector director;
    Maze *maze;
    QList<QSharedPointer<Snake>> *snakes;
    QTimer timer;

    QList<QPoint> getShortestPath(const QPoint& root, const QPoint& node);
    bool snakeHitAnother();
    bool snakesCollide();
    QList<Direction> getAvailableForwardDirections(const Snake *snake) const;
    QList<Direction> getAvailableBackwardDirections(const Snake *snake) const;

public slots:
    void setDirections();
    void onNewGame();
    void onGameOver();
    void saveCurrentState();

signals:
    void move();
    void gameOver();
    void executeSqlQuery(const QString& query);
};

#endif // GAMECONTROLLER_H
