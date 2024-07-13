#include <QDebug>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include "list_model.h"

namespace MINIOICQ
{

void ListModel::setDatabase(QSqlDatabase db)
{
    // Execute MINIOICQ/sql/client.sql to create the tables
    _db = db;
    auto tables = db.tables();
    if (!tables.contains("users"))
    {
        qDebug() << "Table users not found, creating tables";
        QSqlQuery createTable(db);
        createTable.exec(".read MINIOICQ/sql/client.sql");
    }
    refresh();
}

void ListModel::on_message_received()
{
    refresh();
    emit message_received();
}

void ListModel::refresh()
{
    setQuery("SELECT * FROM chat_list_view", _db);
    setHeaderData(0, Qt::Horizontal, "cid");
    setHeaderData(1, Qt::Horizontal, "name");
    setHeaderData(2, Qt::Horizontal, "avatar");
    setHeaderData(3, Qt::Horizontal, "last_mtype");
    setHeaderData(4, Qt::Horizontal, "last_message");
    setHeaderData(5, Qt::Horizontal, "last_send_time");
    setHeaderData(6, Qt::Horizontal, "un_read_count");
}

} // namespace MINIOICQ
