#include "gamecontroller.h"
#include <QDebug>
#include <QQueue>
#include <QDateTime>

GameController::GameController(QObject *parent, int number_of_snakes) : QObject(parent),
    num_snakes(number_of_snakes), maze(new Maze(this)),
    snakes(new QList<QSharedPointer<Snake>>())
{
    qsrand(uint(QTime::currentTime().msec()));
    snakes->reserve(number_of_snakes);
    director.setBuilder(&builder); 

    connect(this, &GameController::gameOver, this, &GameController::onGameOver);
}

GameController::~GameController()
{
//    delete snakes;
//    delete maze;
}

void GameController::onNewGame()
{
    onGameOver();
    snakes->clear();
    maze->clearMaze();  //clear previous cells and snakes
    if (maze->generate())
    {
        qDebug() << "Maze generated";
        if (generateSnakes())
        {
            saveCurrentState();
            timer.start(600);
            connect(&timer, &QTimer::timeout, this, &GameController::setDirections, Qt::QueuedConnection);
            connect(this, &GameController::move, maze, &Maze::advance, Qt::QueuedConnection);
        }
    }
    else {
        qDebug() << "Could not generate maze";
    }
}

void GameController::onGameOver()
{
    timer.stop();
    disconnect(&timer, &QTimer::timeout, this, &GameController::setDirections);
    disconnect(this, &GameController::move, maze, &Maze::advance);
}

void GameController::saveCurrentState()
{
    //insert game data
    QDateTime current_datetime = QDateTime::currentDateTime();
    QString game_name = current_datetime.toString("yyyy-MM-dd hh:mm:ss");
    QString insertGame = QString("INSERT INTO game(created) VALUES('%1');").arg(game_name);
    emit executeSqlQuery(insertGame);

    QString insertMaze = QString("INSERT INTO maze(name, width, height, game_id) VALUES('%1', %2, %3, '%4');").
             arg(maze->getName()).arg(maze->width).arg(maze->height).arg(game_name);

    emit executeSqlQuery(insertMaze);
    for (auto x = 0; x < maze->width; ++x) {
        for (auto y = 0; y < maze->height; ++y) {
            auto& cell = maze->grid[x][y];
            QString insertCell = QString("INSERT INTO cell(x, y, top, right, bottom, left, game_id, maze_id) VALUES(%1,%2,%3,%4,%5,%6,'%7','%8');")
                    .arg(x).arg(y)
                    .arg(cell->top.isVisible()).arg(cell->right.isVisible()).arg(cell->bottom.isVisible()).arg(cell->left.isVisible())
                    .arg(game_name)
                    .arg(maze->getName());
            emit executeSqlQuery(insertCell);
        }
    }
}

bool GameController::generateSnakes()
{
    constexpr int iter_threshold = 100;      //try N times
    int iter = 0;
    do{
        snakes->clear();

        for (auto i = 0; i < num_snakes; ++i)
        {
            if ((i+2) % 2 != 0)
            {
                director.buildDarkGreenSnake();
            }
            else
            {
                director.buildDarkYellowSnake();
            }
            auto snake = builder.getSnake();
            auto root = pointToCell(snake->head());
            auto node = pointToCell(snake->tail());
            auto shortest_path = getShortestPath(root, node);
            if (shortest_path.isEmpty())
            {
                qDebug() << "Could not generate snakes";
                return false;
            }
            QList<QPointF> body;
            std::for_each(shortest_path.cbegin(), shortest_path.cend(),[&body](const QPoint& cell){
                body.push_back(cellToPoint(cell));
            });

            builder.setBody(std::move(body));

            snakes->push_back(snake);
            maze->addItem(snake.get());
            builder.reset();
        }
        ++iter;
    }
    while((snakes->isEmpty() || snakeHitAnother() || snakesCollide()) && iter < iter_threshold);
    if (iter < iter_threshold)
    {
        qDebug() << "Snakes generated";
        return true;
    }
    else {
        qDebug() << "Could not generate snakes";
        snakes->clear();
        return false;
    }
}

//this function uses Breadth-First Search (BFS) algorithm for finding shortest path from root to node
QList<QPoint> GameController::getShortestPath(const QPoint& root, const QPoint& node)
{
    int dst[Maze::width][Maze::height];                 //distance of each vertex from the root
    QPair<int, int> prev[Maze::width][Maze::height];    //store previous vertex for a given vertex. It's needed to restore the shortest path after BFS

    for (auto i = 0; i < Maze::width; i++) {
        for (auto j = 0; j < Maze::width; j++) {
            dst[i][j] = 0;
            maze->grid[i][j]->setVisited(false);
        }
    }

    QQueue<QPair<int, int>> q;                  //queue stores coordinates (x,y) in cells x={0, Maze width}, y={0, Maze height}
    q.enqueue({root.x(), root.y()});
    maze->grid[root.x()][root.y()]->setVisited(true);
    dst[root.x()][root.y()] = 0;
    prev[root.x()][root.y()] = {-1,-1};         //head does not have previous cell

    while (!q.empty()) {
        QPair<int, int> cur = q.front();
        q.dequeue();

        int cx = cur.first, cy = cur.second;
        const auto& cell = maze->grid[cx][cy];

        QVector<QPair<int, int>> available_moves;
        if (!cell->bottom.isVisible())
            available_moves.append(QPair<int, int>(0,1));
        if (!cell->top.isVisible())
            available_moves.append(QPair<int, int>(0,-1));
        if (!cell->left.isVisible())
            available_moves.append(QPair<int, int>(-1,0));
        if (!cell->right.isVisible())
            available_moves.append(QPair<int, int>(1,0));

        for (auto move: available_moves) {
            int dx = move.first, dy = move.second;
            if (!maze->grid[cx + dx][cy + dy]->isVisited()) {
                q.enqueue({cx + dx, cy + dy});
                maze->grid[cx + dx][cy + dy]->setVisited(true);
                dst[cx + dx][cy + dy] = dst[cx][cy] + 1;
                prev[cx + dx][cy + dy] = {cx, cy};
            }
        }
    }

    QList<QPoint> path;
    if (dst[node.x()][node.y()] != 0) {
        QPair<int, int> cur = {node.x(), node.y()};                         //current vertex
        path.push_back(QPoint(cur.first, cur.second));                      //adding node

        while (prev[cur.first][cur.second] != QPair<int,int>(-1,-1)){       //until previous vertex exists
            cur = prev[cur.first][cur.second];                              //move to previous vertex
            path.push_back(QPoint(cur.first, cur.second));                  //add current vertex to the path
        }

        std::reverse(path.begin(), path.end());
    }
    return path;
}

bool GameController::snakesCollide()
{
    for (auto i = 0; i < num_snakes; ++i)
    {
        auto snake1 = snakes->at(i).get();
        for (auto j = i + 1; j < num_snakes; ++j)
        {
            auto snake2 = snakes->at(j).get();
            if (snake1->collidesWithItem(snake2)) return true;
        }
    }
    return false;
}

QList<Direction> GameController::getAvailableForwardDirections(const Snake *snake) const
{
    auto head = pointToCell(snake->head());
    auto afterhead = pointToCell(snake->body.at(1));
    const auto& cell = maze->grid[head.x()][head.y()];
    QList<Direction> freeForwardDirections;

    if (!cell->bottom.isVisible() && afterhead.y() <= head.y())
        freeForwardDirections.push_back(Direction::ForwardDown);

    if (!cell->top.isVisible() && afterhead.y() >= head.y())
        freeForwardDirections.push_back(Direction::ForwardUp);

    if (!cell->left.isVisible() && afterhead.x() >= head.x())
        freeForwardDirections.push_back(Direction::ForwardLeft);

    if (!cell->right.isVisible() && afterhead.x() <= head.x())
        freeForwardDirections.push_back(Direction::ForwardRight);
    return freeForwardDirections;
}

QList<Direction> GameController::getAvailableBackwardDirections(const Snake *snake) const
{
    auto tail = pointToCell(snake->tail());
    auto beforetail = pointToCell(snake->body.at(snake->body.size()-2));
    const auto& cell = maze->grid[tail.x()][tail.y()];
    QList<Direction> freeBackwardDirections;

    if (!cell->bottom.isVisible() && beforetail.y() <= tail.y())
        freeBackwardDirections.append(Direction::BackwardDown);

    if (!cell->top.isVisible() && beforetail.y() >= tail.y())
        freeBackwardDirections.append(Direction::BackwardUp);

    if (!cell->left.isVisible() && beforetail.x() >= tail.x())
        freeBackwardDirections.append(Direction::BackwardLeft);

    if (!cell->right.isVisible() && beforetail.x() <= tail.x())
        freeBackwardDirections.append(Direction::BackwardRight);
    return freeBackwardDirections;
}

void GameController::setDirections()
{
    if (snakes == nullptr || snakes->isEmpty())
        return;

    if (snakeHitAnother()) {
        qDebug() << "GAME OVER";
        emit gameOver();
        return;
    }

    for(auto& snake : *snakes)
    {
        auto available_fordirs {getAvailableForwardDirections(snake.get())};
        auto available_backdirs {getAvailableBackwardDirections(snake.get())};

        if (snake->isGoingForward() || snake->isNotMoving())
        {
            if (!available_fordirs.isEmpty())
            {   //if there are available forward directions, move forward to random cell
                snake->setDirection(available_fordirs.at(rand() % available_fordirs.size()));
            }
            else
            {   //if there are no available forward directions, move backward to random cell
                snake->setDirection(available_backdirs.at(rand() % available_backdirs.size()));
            }
        }
        else if (snake->isGoingBackward())
        {
            if (!available_backdirs.isEmpty() && available_fordirs.size() <= 1)
            {   //keep moving back until there are more than one available forward directions
                snake->setDirection(available_backdirs.at(rand() % available_backdirs.size()));
            }
            else
            {   //if there are more than one available forward directions, move to random one
                snake->setDirection(available_fordirs.at(rand() % available_fordirs.size()));
            }
        }
    }
    emit move();
}

bool GameController::snakeHitAnother()
{
    for (auto i = 0; i < snakes->size(); ++i)
    {
        auto snake1 = snakes->at(i).get();
        for (auto j = i + 1; j < snakes->size(); ++j)
        {
            auto snake2 = snakes->at(j).get();
            if (snake1->body.contains(snake2->head()) || snake2->body.contains(snake1->head())
                || snake1->body.contains(snake2->tail()) || snake2->body.contains(snake1->tail()))
                return true;
        }
    }
    return false;
}

//converts from pixel to cell coordinate system
QPoint GameController::pointToCell(const QPointF &point)
{
    QPoint cell(int(point.x() / Cell::size), int(point.y() / Cell::size));
    return cell;
}

//converts from cell to pixel coordinate system
QPointF GameController::cellToPoint(const QPoint &cell)
{
    QPointF point(cell.x() * Cell::size, cell.y() * Cell::size);
    return point;
}

Maze* GameController::getMaze()
{
    return maze;
}

QList<QSharedPointer<Snake>>* GameController::getSnakes()
{
    return snakes;
}


