#include <QApplication>
#include <QDebug>
#include <QSortFilterProxyModel>
#include <QSqlTableModel>

#include "qtmaterialstyle.h"

#include "chat/chat_model.h"
#include "chat/chat_view.h"
#include "chat/chat_viewmodel.h"
#include "common/misc.h"
#include "list/list_model.h"
#include "list/list_view.h"
#include "list/list_viewmodel.h"
#include "login/login_model.h"
#include "login/login_view.h"
#include "login/login_viewmodel.h"
#include "websocket/websocket.h"

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
        localUserFile.open(QIODevice::WriteOnly);
        if (!localUserFile.isOpen())
        {
            qDebug() << "Open file failed: " << localUserFile.fileName();
            throw localUserFile.errorString();
        }
        localUserFile.close();
    }
    // init database file
    db = QSqlDatabase::addDatabase("QSQLITE");
    qDebug() << db.driver()->hasFeature(QSqlDriver::BLOB);
    db.setDatabaseName(localDBPath + dbName);
    if (!db.open())
    {
        qDebug() << "Open database failed: " << db.lastError();
        throw db.lastError();
    }
}

int main(int argc, char* argv[])
{
    Q_INIT_RESOURCE(resources);
    QFont font("Roboto");
    MINIOICQ::initTheme();

    // app style
    // QApplication::setStyle(&QtMaterialStyle::instance());
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);
    a.setFont(font);

    // DB directory
    initDBPath(localDBPath);


    WebSocketConnector wsConnector;
    wsConnector.connectSocket("ws://localhost:58765");

    // Login
    QSqlDatabase localUserDB;
    initDB(localUserFileName, localUserDB);
    MINIOICQ::LoginModel loginModel(nullptr, localUserDB);
    MINIOICQ::LoginViewModel loginViewModel;
    MINIOICQ::LoginView* loginView = new MINIOICQ::LoginView;
    loginViewModel.setSourceModel(&loginModel);
    loginViewModel.setWsConnector(&wsConnector);
    MINIOICQ::bindLoginView(loginView, &loginViewModel);

    // Debug: print the table
    // qDebug() << "Table: " << loginModel.tableName();
    // qDebug() << "Columns: " << loginModel.columnCount();
    // for (int i = 0; i < loginModel.columnCount(); i++)
    // {
    //     qDebug() << "Column " << i << ": "
    //              << loginModel.headerData(i, Qt::Horizontal, Qt::DisplayRole);
    // }
    // qDebug() << "Rows: " << loginModel.rowCount();
    // for (int i = 0; i < loginModel.rowCount(); i++)
    // {
    //     for (int j = 0; j < loginModel.columnCount(); j++)
    //     {
    //         qDebug() << "Row " << i << " Column " << j << ": "
    //                  << loginModel.data(loginModel.index(i, j),
    //                                     Qt::DisplayRole);
    //     }
    // }

    MINIOICQ::ListView listView;
    MINIOICQ::ListViewModel listViewModel;
    MINIOICQ::ListModel listModel;
    bindListView(&listView, &listViewModel);

    // Main Logic
    if (loginView->exec() == QDialog::Accepted)
    {
        QSqlDatabase localChatDB;
        initDB("client_db_" + loginViewModel.loggedUserId() + ".db", localChatDB);
        listModel.setDatabase(localChatDB);
        listViewModel.setSourceModel(&listModel);
        listViewModel.setUserId(loginViewModel.loggedUserId());
        listViewModel.setWsConnector(&wsConnector);
        listView.show();
    }
    else
    {
        exit(EXIT_SUCCESS);
    }

    return a.exec();
}
