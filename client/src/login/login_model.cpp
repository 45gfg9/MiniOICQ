#include "login_model.h"

namespace MINIOICQ
{

LoginModel::LoginModel(QObject* parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{
    // check if "localUser" table exist
    // if not, create table
    auto tables = db.tables();
    if (!tables.contains("localUser"))
    {
        QSqlQuery createTable(db);
        createTable.prepare("CREATE TABLE localUser ("
                            "userId INT PRIMARY KEY NOT NULL, "
                            "userName VARCHAR NOT NULL, "
                            "password VARCHAR NOT NULL, "
                            "avatar BLOB"
                            ")");
        if (!createTable.exec())
        {
            qDebug() << "Create table failed: " << createTable.lastError();
            throw std::runtime_error("Create table failed");
        }
    }
    this->setTable("localUser");
    this->select();
    this->setEditStrategy(QSqlTableModel::OnFieldChange);
}

LoginModel::~LoginModel() {}

} // namespace MINIOICQ