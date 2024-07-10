#include "login/login_model.h"
#include "login/login_proxymodel.h"
#include "login/login_view.h"
#include <QDebug>
#include <QSortFilterProxyModel>
#include <QSqlTableModel>
#include <QtWidgets/QApplication>

int main (int argc, char* argv[]) {
    QApplication a (argc, argv);
    a.setQuitOnLastWindowClosed (true);
    Q_INIT_RESOURCE (resources);

    // Login
    QSqlDatabase localUsersDB;
    localUsersDB = QSqlDatabase::addDatabase ("QSQLITE", "users_connection");
    localUsersDB.setDatabaseName (QDir::currentPath () + "/MINIOICQ/databases/localUser.db");
    if (!localUsersDB.open ()) {
        qDebug () << "Cannot open database: " << localUsersDB.databaseName () << "\n";
        // create the database
        throw localUsersDB.lastError ();
    } else {
        qDebug () << "Database: connection ok";
    }
    MINIOICQ::LoginModel loginModel (nullptr, localUsersDB);

    // Debug: print the table
    qDebug () << "Table: " << loginModel.tableName ();
    qDebug () << "Columns: " << loginModel.columnCount ();
    for (int i = 0; i < loginModel.columnCount (); i++) {
        qDebug () << "Column " << i << ": "
                  << loginModel.headerData (i, Qt::Horizontal, Qt::DisplayRole);
    }
    qDebug () << "Rows: " << loginModel.rowCount ();
    for (int i = 0; i < loginModel.rowCount (); i++) {
        for (int j = 0; j < loginModel.columnCount () - 1; j++) {
            qDebug () << "Row " << i << " Column " << j << ": "
                      << loginModel.data (loginModel.index (i, j), Qt::DisplayRole);
        }
    }


    MINIOICQ::LoginProxyModel loginProxyModel;
    loginProxyModel.setSourceModel (&loginModel);

    // Debug: print the table
    qDebug () << "LoginProxyModel";
    qDebug () << "Columns: " << loginProxyModel.columnCount ();
    for (int i = 0; i < loginProxyModel.columnCount (); i++) {
        qDebug () << "Column " << i << ": "
                  << loginProxyModel.headerData (i, Qt::Horizontal, Qt::DisplayRole);
    }
    qDebug () << "Rows: " << loginProxyModel.rowCount ();
    for (int i = 0; i < loginProxyModel.rowCount (); i++) {
        for (int j = 0; j < loginProxyModel.columnCount () - 1; j++) {
            qDebug () << "Row " << i << " Column " << j << ": "
                      << loginProxyModel.data (loginProxyModel.index (i, j), Qt::DisplayRole);
        }
    }


    MINIOICQ::LoginView loginView;
    loginView.setModel (&loginProxyModel);
    if (loginView.exec () == QDialog::Accepted) {
        qDebug () << "Login success";
    } else {
        qDebug () << "Login failed";
    }

    return 0;
}
