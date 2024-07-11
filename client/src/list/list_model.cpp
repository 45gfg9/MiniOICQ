#include <QSqlDatabase>
#include <QSqlQuery>

#include "list_model.h"

namespace MINIOICQ
{

ListModel::ListModel(QObject* parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{
    // Execute MINIOICQ/sql/client.sql to create the tables
    auto tables = db.tables();
    if (!tables.contains("users")) {
        qDebug() << "Table users not found, creating tables";
        QString sql_path = "MINIOICQ/sql/client.sql";
        QSqlQuery createTable(db);
        createTable.prepare(".read " + sql_path);
        if (!createTable.exec()) {
            qDebug() << "Create table failed: " << createTable.lastError();
            throw std::runtime_error("Create table failed");
        }
    }
    setTable("chat_list_view");
    select();
}

ListModel::~ListModel() {}

} // namespace MINIOICQ