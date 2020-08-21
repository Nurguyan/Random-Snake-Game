#ifndef DATABASEWORKER_H
#define DATABASEWORKER_H

#include <QObject>
#include <QSqlDatabase>

class DatabaseWorker : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseWorker(QObject *parent = nullptr, QString database_name = "database.db");
    ~DatabaseWorker();

private:
    QString db_name;
    QSqlDatabase db;


public slots:
    void createConnection();
    void executeQuery(const QString&);
};

#endif // DATABASEWORKER_H
