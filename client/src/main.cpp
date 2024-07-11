#include "login/login_model.h"
#include "login/login_proxymodel.h"
#include "login/login_view.h"
#include <QDebug>
#include <QSortFilterProxyModel>
#include <QSqlTableModel>
#include <QtWidgets/QApplication>

const QString localDBPath = "MINIOICQ/databases/";
const QString localUserFileName = "localUser.db";

void initDBPath(const QString& dbPath)
{
    QDir dir;
    if (!dir.exists(dbPath))
    {
        dir.mkpath(dbPath);
        if (!dir.exists(dbPath))
        {
            qDebug() << "Create db path failed: " << dbPath;
            throw std::runtime_error("Create db path failed");
        }
    }
}

void initDB(const QString& dbName, QSqlDatabase &db)
{
    // create dbfile if not exist
    QFile localUserFile(localDBPath + dbName);
    if (!localUserFile.exists())
    {
        qDebug() << "Create new db file " << localUserFile.fileName();
        localUserFile.open(QIODevice::WriteOnly);
        if (!localUserFile.isOpen())
        {
            qDebug() << "Open file failed: " << localUserFile.fileName();
            throw localUserFile.errorString();
        }
        localUserFile.close();
    }
    // init database file
    db = QSqlDatabase::addDatabase("QSQLITE", "users_connection");
    db.setDatabaseName(localDBPath + dbName);
    if (!db.open()) {
        qDebug() << "Open database failed: " << db.lastError();
        throw db.lastError();
    }
    else 
    {
        qDebug() << "Open database success: " << db.databaseName();
    }
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);
    Q_INIT_RESOURCE(resources);
    QFont font("Roboto", 12, QFont::Medium);
    a.setFont(font);

    // DB directory
    initDBPath(localDBPath);
    QSqlDatabase localUserDB;
    initDB(localUserFileName, localUserDB);
    MINIOICQ::LoginModel loginModel(nullptr, localUserDB);

    // Debug: print the table
    qDebug() << "Table: " << loginModel.tableName();
    qDebug() << "Columns: " << loginModel.columnCount();
    for (int i = 0; i < loginModel.columnCount(); i++)
    {
        qDebug() << "Column " << i << ": "
                 << loginModel.headerData(i, Qt::Horizontal, Qt::DisplayRole);
    }
    qDebug() << "Rows: " << loginModel.rowCount();
    for (int i = 0; i < loginModel.rowCount(); i++)
    {
        for (int j = 0; j < loginModel.columnCount() - 1; j++)
        {
            qDebug() << "Row " << i << " Column " << j << ": "
                     << loginModel.data(loginModel.index(i, j),
                                        Qt::DisplayRole);
        }
    }

    MINIOICQ::LoginProxyModel loginProxyModel;
    loginProxyModel.setSourceModel(&loginModel);

    // Debug: print the table
    qDebug() << "LoginProxyModel";
    qDebug() << "Columns: " << loginProxyModel.columnCount();
    for (int i = 0; i < loginProxyModel.columnCount(); i++)
    {
        qDebug() << "Column " << i << ": "
                 << loginProxyModel.headerData(i, Qt::Horizontal,
                                               Qt::DisplayRole);
    }
    qDebug() << "Rows: " << loginProxyModel.rowCount();
    for (int i = 0; i < loginProxyModel.rowCount(); i++)
    {
        for (int j = 0; j < loginProxyModel.columnCount() - 1; j++)
        {
            qDebug() << "Row " << i << " Column " << j << ": "
                     << loginProxyModel.data(loginProxyModel.index(i, j),
                                             Qt::DisplayRole);
        }
    }

    MINIOICQ::LoginView loginView;
    loginView.setModel(&loginProxyModel);

    //MINIOICQ::ListView listView;

    // connect the signals
    // connect();



    if (loginView.exec() == QDialog::Accepted)
    {
        qDebug() << "Login success";
        // listView->setUserId(loginProxyModel.loggedUserId());
        // listView.show();
    }
    else
    {
        qDebug() << "Login failed";
    }

    return 0;
}
