#include "databaseworker.h"
#include <QSqlQuery>
#include <QDebug>

DatabaseWorker::DatabaseWorker(QObject *parent, QString database_name) : QObject(parent), db_name(database_name) {}

DatabaseWorker::~DatabaseWorker()
{
    if (db.isOpen())
        db.close();
}

void DatabaseWorker::createConnection()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName(db_name);

    if (db.open())
    {
        qDebug("Connected to database");
    }
    else
    {
        qDebug("Error occured in connection to database");
    }
}

void DatabaseWorker::executeQuery(const QString &queryString)
{
    QSqlQuery query(queryString, db);
    //qDebug() << "Query: " << queryString;
    query.exec();
}

