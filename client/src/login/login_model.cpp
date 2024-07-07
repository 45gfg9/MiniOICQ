#include "login_model.h"

/**
 * @brief Constructor for LoginModel
 * 
 * @note Initializes the database connection
 */
LoginModel::LoginModel() {
    if (QSqlDatabase::contains("users_connection")) {
        db = QSqlDatabase::database("users_connection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", "users_connection");
        db.setDatabaseName(QDir::currentPath() + "/databases/users.db");
        if (!db.open()) {
            qDebug() << "Error: connection with database failed";
            throw db.lastError();
        } else {
            qDebug() << "Database: connection ok";
        }
    }
}

/**
 * @brief Destructor for LoginModel
 * 
 * @note Deletes all UserInfo objects in the users QVector
 */
LoginModel::~LoginModel() {
    db.close();
}

/**
 * @brief Get all users in the local database
 * 
 * @return const QVector<UserInfo *> & The QVector of UserInfo objects
 */
QVector<UserInfo *> *LoginModel::getUsers() {

    // Otherwise, initialize it with data from the database
    QVector<UserInfo *> *users = new QVector<UserInfo *>();
    
    QSqlQueryModel queryModel;
    QSqlQuery query(db);

    // Check if the table "users" exists
    query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='users'");
    if (!query.next()) {
        query.exec("CREATE TABLE users (username varchar(30) PRIMARY KEY, password varchar(50) NOT NULL, icon blob)");
    }

    // Get all users from the database
    queryModel.setQuery("SELECT * FROM users");

    for (int i = 0; i < queryModel.rowCount(); i++) {
        UserInfo *user = new UserInfo(queryModel.record(i).value("username").toString(),
                                      queryModel.record(i).value("password").toString(),
                                      queryModel.record(i).value("icon").toByteArray());
        users->append(user);
    }

    return users;
}


/**
 * @brief Add a user to the local database
 * 
 * @param user The UserInfo object to add
 */
void LoginModel::addUser(UserInfo *user) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO users (username, password, icon) VALUES (:username, :password, :icon)");
    query.bindValue(":username", user->getUsername());
    query.bindValue(":password", user->getPassword());
    query.bindValue(":icon", user->getIcon());
    query.exec();
}