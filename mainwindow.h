#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include "gamecontroller.h"
#include "databaseworker.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static constexpr int number_of_snakes = 2;
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    GameController *game;
    QGraphicsView *graphicsView;
    QAction *newGameAction;
    QAction *exitAction;
    QThread *db_thread;
    DatabaseWorker *db_worker;

    void createMenu();
    void createMenuItems();
    void connectToDatabase();

public slots:
    void onGameOver();

signals:
    void startNewGame();
    void saveInitialState();
    void executeSqlQuery(const QString&);
};

#endif // MAINWINDOW_H
