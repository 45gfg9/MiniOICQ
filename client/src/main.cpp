#include "list/list_model.h"
#include "list/list_view.h"
#include "list/list_viewmodel.h"
#include "login/login_model.h"
#include "login/login_view.h"
#include "login/login_viewmodel.h"
#include "websocket/websocket.h"
#include <QDebug>
#include <QSortFilterProxyModel>
#include <QSqlTableModel>
#include <QtWidgets/QApplication>
#include <qtmaterialstyle.h>

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

void initDB(const QString& dbName, QSqlDatabase& db)
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
    if (!db.open())
    {
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
    Q_INIT_RESOURCE(resources);
    QFont font("Roboto");

    // app style
    // QApplication::setStyle(&QtMaterialStyle::instance());
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);
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

    MINIOICQ::LoginViewModel loginViewModel;
    loginViewModel.setSourceModel(&loginModel);

    MINIOICQ::LoginView* loginView = new MINIOICQ::LoginView;
    MINIOICQ::bindLoginView(loginView, &loginViewModel);

    WebSocketConnector wsConnector;
    wsConnector.connectSocket("ws://localhost:58765");
    loginViewModel.connect(&loginViewModel, &MINIOICQ::LoginViewModel::login,
                           &wsConnector, &WebSocketConnector::on_login);
    loginViewModel.connect(&loginViewModel, &MINIOICQ::LoginViewModel::reg,
                           &wsConnector, &WebSocketConnector::on_reg);
    wsConnector.connect(&wsConnector, &WebSocketConnector::loginSuccess,
                        &loginViewModel,
                        &MINIOICQ::LoginViewModel::on_loginSuccess);
    wsConnector.connect(&wsConnector, &WebSocketConnector::loginFailed,
                        &loginViewModel,
                        &MINIOICQ::LoginViewModel::on_loginFailed);
    wsConnector.connect(&wsConnector, &WebSocketConnector::regSuccess,
                        &loginViewModel,
                        &MINIOICQ::LoginViewModel::on_regSuccess);

    MINIOICQ::ListView listView;
    if (loginView->exec() == QDialog::Accepted)
    {
        qDebug() << "Login success";
        // listView->setUserId(loginViewModel.loggedUserId());
        listView.show();
    }
    else
    {
        qDebug() << "Login failed";
    }

    return a.exec();
}
