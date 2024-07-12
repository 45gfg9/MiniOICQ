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
    auto tables = db.tables();
    if (!tables.contains("users"))
    {
        qDebug() << "Table users not found, creating tables";

        // Get the SQL script from file
        QString sql_path = "MINIOICQ/sql/client.sql";
        QFile file(sql_path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Open file failed: " << file.errorString();
            throw std::runtime_error("Open file failed");
        }
        QTextStream in(&file);
        QString sql = in.readAll();
        qDebug() << "SQL script: " << sql;

        // Execute the SQL script
        QSqlQuery createTable(db);
        createTable.prepare(sql);
        if (!createTable.exec())
        {
            qDebug() << "Create table failed: " << createTable.lastError();
            throw std::runtime_error("Create table failed");
        }
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
    setQuery("SELECT * FROM chat_list_view");
    setHeaderData(0, Qt::Horizontal, "cid");
    setHeaderData(1, Qt::Horizontal, "name");
    setHeaderData(2, Qt::Horizontal, "avatar");
    setHeaderData(3, Qt::Horizontal, "last_message");
    setHeaderData(4, Qt::Horizontal, "last_send_time");
    setHeaderData(5, Qt::Horizontal, "un_read_count");
}

} // namespace MINIOICQ
