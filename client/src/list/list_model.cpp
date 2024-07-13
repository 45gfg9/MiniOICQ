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
        QSqlQuery initdb(db);
        initdb.exec("CREATE TABLE IF NOT EXISTS users ("
                        "uid INT PRIMARY KEY,"
                        "name VARCHAR NOT NULL,"
                        "avatar BLOB"
                        ");");
        initdb.exec("CREATE TABLE IF NOT EXISTS chats ("
                        "cid INT PRIMARY KEY,"
                        "name VARCHAR NOT NULL,"
                        "avatar BLOB,"
                        "oid INT NOT NULL REFERENCES users(uid),"
                        "last_view TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
                        "start_time TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP"
                        ");");
        initdb.exec("CREATE TABLE IF NOT EXISTS messages ("
                        "cid INT NOT NULL,"
                        "mid INT NOT NULL,"
                        "mtype VARCHAR NOT NULL,"
                        "message BLOB NOT NULL,"
                        "send_time TIMESTAMP NOT NULL,"
                        "uid INT NOT NULL,"
                        "PRIMARY KEY (cid, mid)"
                        ");");
        initdb.exec("CREATE TABLE IF NOT EXISTS joins ("
                        "cid INT NOT NULL,"
                        "uid INT NOT NULL,"
                        "join_time TIMESTAMP NOT NULL,"
                        "PRIMARY KEY (cid, uid)"
                        ");");
        initdb.exec("CREATE VIEW IF NOT EXISTS chat_list_view AS "
                        "WITH latest_messages AS ("
                        "SELECT c.cid, c.name, c.avatar, m.mtype, m.message, m.send_time,"
                        "RANK() OVER (PARTITION BY c.cid ORDER BY m.send_time DESC) AS rank "
                        "FROM chats c LEFT JOIN messages m ON c.cid = m.cid),"
                        "messages_after_last_view AS ("
                        "SELECT c.cid, COUNT(m.mid) AS msg_count_after_last_view "
                        "FROM chats c LEFT JOIN messages m ON c.cid = m.cid "
                        "WHERE m.send_time > c.last_view "
                        "GROUP BY c.cid)"
                        "SELECT "
                        "lm.cid AS cid,"
                        "lm.name AS name,"
                        "lm.avatar AS avatar,"
                        "lm.mtype AS last_mtype,"
                        "lm.message AS last_message,"
                        "lm.send_time AS last_send_time,"
                        "COALESCE(mlv.msg_count_after_last_view, 0) AS un_read_count "
                        "FROM latest_messages lm LEFT JOIN messages_after_last_view mlv ON lm.cid = mlv.cid "
                        "WHERE lm.rank = 1;");
        if (initdb.lastError().isValid())
        {
            qDebug() << "Create tables failed: " << initdb.lastError();
            throw std::runtime_error("Create tables failed");
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
