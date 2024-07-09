#include "login/login_model.h"
#include "login/login_proxymodel.h"
#include "login/login_view.h"
#include <QDebug>
#include <QSortFilterProxyModel>
#include <QSqlTableModel>
#include <QtWidgets/QApplication>

int main (int argc, char* argv[]) {
    QApplication a (argc, argv);
    Q_INIT_RESOURCE (resources);

    // Login
    QSqlDatabase localUsersDB;
    localUsersDB = QSqlDatabase::addDatabase ("QSQLITE", "users_connection");
    localUsersDB.setDatabaseName (QDir::currentPath () + "/MINIOICQ/databases/users._db");
    if (!localUsersDB.open ()) {
        qDebug () << "Cannot open database: " << localUsersDB.lastError ().text () << "\n";
        // create the database
        throw localUsersDB.lastError ();
    } else {
        qDebug () << "Database: connection ok";
    }
    // MINIOICQ::LoginModel loginModel(nullptr, localUsersDB);
    QSqlTableModel loginModel (nullptr, localUsersDB);
    MINIOICQ::LoginProxyModel loginProxyModel;
    loginProxyModel.setSourceModel (&loginModel);
    // loginProxyModel->setFilterKeyColumn(1);
    MINIOICQ::LoginView loginView;
    loginView.setModel (&loginProxyModel);
    if (loginView.exec () == QDialog::Accepted) {
        qDebug () << "Login success";
    } else {
        qDebug () << "Login failed";
    }

    return a.exec ();
}
