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

    // List
    // MINIOICQ::ListView listView;
    // MINIOICQ::ListViewModel listViewModel;
    // MINIOICQ::ListModel listModel;
    // listViewModel.setSourceModel(&listModel);
    // listViewModel.setWsConnector(&wsConnector);
    // bindListView(&listView, &listViewModel);
    /*
        // Main Logic
        if (loginView->exec() == QDialog::Accepted)
        {
            qDebug() << "Login success";
            QSqlDatabase localChatDB;
            initDB(loginViewModel.loggedUserId(), localChatDB);
            listModel.setDatabase(localChatDB);
            listView.show();
        }
        else
        {
            qDebug() << "Login failed";
        }
    */
    qDebug() << "test";
    MINIOICQ::TextMessage text("me", "hello world");
    QDirIterator it(":", QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        qDebug() << it.next();
    }
    text.setAvatar(QImage(":/testImage.jpg"));
    MINIOICQ::ChatViewItem chatViewItem(text);
    chatViewItem.show();

    return a.exec();
}
