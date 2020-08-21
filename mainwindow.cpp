#include "mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    game(new GameController(this, number_of_snakes)),
    graphicsView(new QGraphicsView(this))
{
    createMenu();
    connectToDatabase();
    setCentralWidget(graphicsView);
    connect(this, &MainWindow::startNewGame, game, &GameController::onNewGame);
    connect(game, &GameController::gameOver, this, &MainWindow::onGameOver);

    if (game->getMaze() != nullptr)
        graphicsView->setScene(game->getMaze());
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->show(); 
}

void MainWindow::createMenu()
{
    createMenuItems();
    QMenu *menu = menuBar()->addMenu(tr("&File"));
    menu->addAction(newGameAction);
    menu->addSeparator();
    menu->addSeparator();
    menu->addAction(exitAction);
}

void MainWindow::createMenuItems()
{
    newGameAction = new QAction(tr("&New Game"), this);
    newGameAction->setShortcuts(QKeySequence::New);
    newGameAction->setStatusTip(tr("Start a new game"));
    connect(newGameAction, &QAction::triggered, game, &GameController::onNewGame);

    exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the game"));
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);
}

void MainWindow::connectToDatabase()
{
    db_thread = new QThread();
    db_worker = new DatabaseWorker();
    connect(db_thread, &QThread::finished, db_thread, &QThread::deleteLater); //clean up after yourself
    connect(db_thread, &QThread::finished, db_worker, &QObject::deleteLater); //clean up after yourself
    connect(db_thread, &QThread::started, db_worker, &DatabaseWorker::createConnection);//create connection once the thread starts
    connect(game, &GameController::executeSqlQuery, db_worker, &DatabaseWorker::executeQuery); //send SQL queries from Game Controller to the Database Worker
    connect(this, &MainWindow::executeSqlQuery, db_worker, &DatabaseWorker::executeQuery); //send SQL queries from app to the Database Worker
    db_worker->moveToThread(db_thread);
    db_thread->start();

    QString createGameTable("CREATE TABLE IF NOT EXISTS game(created TEXT PRIMARY KEY)");
    QString createMazeTable("CREATE TABLE IF NOT EXISTS maze(name TEXT NOT NULL, width INTEGER NOT NULL, height INTEGER NOT NULL, game_id TEXT NOT NULL, "
                            "constraint game_id_fk FOREIGN KEY(game_id) REFERENCES game(created),"
                            "constraint maze_pk PRIMARY KEY(name, game_id))");
    QString createCellTable("CREATE TABLE IF NOT EXISTS cell(x INTEGER NOT NULL, y INTEGER NOT NULL, top INTEGER NOT NULL, right INTEGER NOT NULL, bottom INTEGER NOT NULL, left INTEGER NOT NULL, game_id TEXT NOT NULL, maze_id TEXT NOT NULL, "
                            "constraint game_id_fk FOREIGN KEY(game_id) REFERENCES maze(game_id),"
                            "constraint maze_id_fk FOREIGN KEY(maze_id) REFERENCES maze(name),"
                            "constraint cell_pk PRIMARY KEY(x, y, game_id, maze_id))");
    emit executeSqlQuery(createGameTable);
    emit executeSqlQuery(createMazeTable);
    emit executeSqlQuery(createCellTable);
}

void MainWindow::onGameOver()
{
    if (QMessageBox::Yes == QMessageBox::information(nullptr,"Game Over", "Launch again?",
                                                     QMessageBox::Yes | QMessageBox::No,
                                                     QMessageBox::Yes)) {
        emit startNewGame();
    } else {
        close();
    }
}

MainWindow::~MainWindow()
{

}
