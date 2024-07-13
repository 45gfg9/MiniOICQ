#include "list_viewmodel.h"
#include <QDebug>
#include <QSortFilterProxyModel>
#include <QSqlError>
#include <QSqlRecord>
#include <QtAlgorithms>

namespace MINIOICQ
{

void ListViewModel::on_debug() { qDebug() << "ListViewModel::on_debug"; }

void ListViewModel::invalidate()
{
    qDebug() << "ListViewModel::invalidate";
    QSortFilterProxyModel::invalidate();
    qDebug() << "ListViewModel Content: ";
    for (int i = 0; i < rowCount(); ++i)
    {
        for (int j = 0; j < columnCount(); ++j)
        {
            qDebug() << data(index(i, j));
        }
    }
    ListModel* model = qobject_cast<ListModel*>(sourceModel());
    _chatIdColumn = model->record().indexOf("cid");
    _chatNameColumn = model->record().indexOf("name");
    _chatAvatarColumn = model->record().indexOf("avatar");
    _chatLastMessageTypeColumn = model->record().indexOf("last_mtype");
    _chatLastMessageColumn = model->record().indexOf("last_message");
    _chatLastMessageTimeColumn = model->record().indexOf("last_send_time");
    _chatUnreadMessageCountColumn = model->record().indexOf("un_read_count");

    qDebug() << "ListViewModel::setSourceModel: chatIdColumn=" << _chatIdColumn
             << ", chatNameColumn=" << _chatNameColumn
             << ", chatAvatarColumn=" << _chatAvatarColumn
             << ", chatLastMessageTypeColumn=" << _chatLastMessageTypeColumn
             << ", chatLastMessageColumn=" << _chatLastMessageColumn
             << ", chatLastMessageTimeColumn=" << _chatLastMessageTimeColumn
             << ", chatUnreadMessageCountColumn="
             << _chatUnreadMessageCountColumn;

    emit dataChanged(index(0, 0), index(rowCount(), columnCount()));
}

void ListViewModel::setSourceModel(ListModel* model)
{
    // connect with model
    QSortFilterProxyModel::setSourceModel(model);
    connect(model, &QAbstractItemModel::dataChanged, this,
            &ListViewModel::on_debug);
    connect(model, &QAbstractItemModel::dataChanged, this,
            &ListViewModel::invalidate);
    connect(this, &ListViewModel::newMsg, model, &ListModel::on_newMsg);
    // notify the view to update
    invalidate();
}

QVariant ListViewModel::data(const QModelIndex& index, int /* role */) const
{
    auto ret = QSortFilterProxyModel::data(index, Qt::DisplayRole);
    if (index.column() == chatLastMessageTimeColumn())
    {
        auto datetime =
            QDateTime::fromString(ret.toString(), "yyyy-MM-dd hh:mm:ss");
        return datetime.toString("MM-dd hh:mm");
    }
    return ret;
}

void ListViewModel::setWsConnector(WebSocketConnector* wsConnector)
{
    _wsConnector = wsConnector;
    connect(wsConnector, &WebSocketConnector::syncMsg, this,
            &ListViewModel::on_newMsg);
    connect(wsConnector, &WebSocketConnector::syncChat, this,
            &ListViewModel::on_newChat);
    connect(wsConnector, &WebSocketConnector::syncUser, this,
            &ListViewModel::on_newUser);
    connect(this, &ListViewModel::sync, wsConnector,
            &WebSocketConnector::on_sync);
    emit sync();

    // connect with chatManager
    auto model = qobject_cast<ListModel*>(sourceModel());
    _chatManager = new ChatManager(_userId.toString(), model->database(),
                                   _wsConnector, this);
    connect(this, &ListViewModel::openChat, _chatManager,
            &ChatManager::on_openChat);
    connect(this, &ListViewModel::newMsg, _chatManager,
            &ChatManager::on_newMsg);
}

QVector<MINIOICQ::UserInfo> ListViewModel::selectUser()
{
    ListModel* model = qobject_cast<ListModel*>(sourceModel());
    QSqlDatabase db = model->database();
    QSqlTableModel userModel(nullptr, db);
    userModel.setTable("users");
    userModel.select();
    QVector<UserInfo> users;
    for (int i = 0; i < userModel.rowCount(); ++i)
    {
        QSqlRecord user = userModel.record(i);
        UserInfo userInfo;
        userInfo.setUserId(user.value("uid").toString());
        userInfo.setUsername(user.value("name").toString());
        userInfo.setAvatar(user.value("avatar").value<QImage>());
        users.push_back(userInfo);
    }
    return users;
}

void ListViewModel::on_openChat(int cid)
{
    int chatId = data(index(cid, _chatIdColumn)).toInt();
    emit openChat(chatId);
}

void ListViewModel::on_closeAll()
{
    qDebug() << "ListViewModel::on_closeButton_clicked";
    _chatManager->closeAll();
    // _wsConnector->close();
}

void ListViewModel::on_invite(const QVector<UserInfo>& users)
{
    qDebug() << "ListViewModel::on_invite";
    emit createChat(users);
}

void ListViewModel::on_newMsg(QVector<MINIOICQ::Message>& messages)
{
    qDebug() << "ListViewModel::on_newMsg";
    ListModel* model = qobject_cast<ListModel*>(sourceModel());
    QSqlDatabase db = model->database();
    QSqlTableModel msgModel(nullptr, db);
    msgModel.setTable("messages");
    for (auto& msg : messages)
    {
        QSqlRecord newMsg = msgModel.record();
        newMsg.setValue("mid", msg.msgId());
        newMsg.setValue("cid", msg.chatId());
        newMsg.setValue("uid", msg.sender());
        newMsg.setValue("mtype", msg.type());
        newMsg.setValue("message", msg.content());
        newMsg.setValue("send_time", msg.time());
        qDebug() << "newMsg: " << newMsg;
        if (!msgModel.insertRecord(-1, newMsg))
        {
            qDebug() << "insert message failed: " << msgModel.lastError();
        }
    }

    invalidate();
    qDebug() << "ListViewModel::on_newMsg: emit newMsg" << rowCount();

    // notify corresponding chat to update
    for (auto& msg : messages)
    {
        emit newMsg(msg.chatId().toInt());
    }
}

void ListViewModel::on_newChat(QVector<MINIOICQ::ChatInfo>& chats)
{
    qDebug() << "ListViewModel::on_newChat";
    std::sort(chats.begin(), chats.end(),
              [](const ChatInfo& a, const ChatInfo& b)
              { return a.chatId() < b.chatId(); });
    ListModel* model = qobject_cast<ListModel*>(sourceModel());
    QSqlDatabase db = model->database();
    QSqlTableModel chatModel(nullptr, db);
    chatModel.setTable("chats");
    chatModel.select();
    QSqlTableModel joinModel(nullptr, db);
    joinModel.setTable("joins");
    joinModel.select();
    auto it = chats.begin();
    for (int i = 0; i < chatModel.rowCount(); ++i)
    {
        QSqlRecord nowChat = chatModel.record(i);
        QString cid = nowChat.value("cid").toString();
        qDebug() << "cid: " << cid;
        while (it != chats.end() && it->chatId() < cid)
        {
            // insert
            QSqlRecord newChat = chatModel.record();
            newChat.setValue("cid", it->chatId());
            newChat.setValue("name", it->name());
            newChat.setValue("avatar", it->avatar());
            newChat.setValue("oid", it->ownerId());
            newChat.setValue("last_view", it->lastViewTime());
            newChat.setValue("start_time", it->startTime());
            qDebug() << "newChat: " << newChat;
            for (auto& memberId : it->members())
            {
                QSqlRecord newJoin = joinModel.record();
                newJoin.setValue("cid", it->chatId());
                newJoin.setValue("uid", memberId.toInt());
                qDebug() << "newJoin: " << newJoin;
                joinModel.insertRecord(-1, newJoin);
            }
            if (!chatModel.insertRecord(-1, newChat))
            {
                qDebug() << "insert chat failed: " << chatModel.lastError();
            }
            ++it;
        }
        if (it == chats.end() || it->chatId() != cid)
        {
            continue;
        }
        else
        {
            // update chat info
            nowChat.setValue("name", it->name());
            nowChat.setValue("avatar", it->avatar());
            nowChat.setValue("oid", it->ownerId());
            nowChat.setValue("last_view", it->lastViewTime());
            nowChat.setValue("start_time", it->startTime());
            for (auto& memberId : it->members())
            {
                QSqlRecord newJoin = joinModel.record();
                newJoin.setValue("cid", it->chatId());
                newJoin.setValue("uid", memberId.toInt());
                joinModel.insertRecord(-1, newJoin);
            }
            if (!chatModel.setRecord(i, nowChat))
            {
                qDebug() << "update chat failed: " << chatModel.lastError();
            }
            ++it;
        }
    }
    while (it != chats.end())
    {
        // insert
        QSqlRecord newChat = chatModel.record();
        newChat.setValue("cid", it->chatId());
        newChat.setValue("name", it->name());
        newChat.setValue("avatar", it->avatar());
        newChat.setValue("oid", it->ownerId());
        newChat.setValue("last_view", it->lastViewTime());
        newChat.setValue("start_time", it->startTime());
        for (auto& memberId : it->members())
        {
            QSqlRecord newJoin = joinModel.record();
            newJoin.setValue("cid", it->chatId());
            newJoin.setValue("uid", memberId.toInt());
            qDebug() << "memberId: " << memberId;
            qDebug() << "newJoin: " << newJoin;
            joinModel.insertRecord(-1, newJoin);
        }
        // qDebug() << "newChat: " << newChat;
        if (!chatModel.insertRecord(-1, newChat))
        {
            qDebug() << "insert chat failed: " << chatModel.lastError();
        }
        ++it;
    }
}

void ListViewModel::on_newUser(QVector<MINIOICQ::UserInfo>& users)
{
    qDebug() << "ListViewModel::on_newUser";
    std::sort(users.begin(), users.end(),
              [](const UserInfo& a, const UserInfo& b)
              { return a.userId() < b.userId(); });
    ListModel* model = qobject_cast<ListModel*>(sourceModel());
    QSqlDatabase db = model->database();
    QSqlTableModel userModel(nullptr, db);
    userModel.setTable("users");
    userModel.select();
    auto it = users.begin();
    for (int i = 0; i < userModel.rowCount(); ++i)
    {
        QSqlRecord nowUser = userModel.record(i);
        QString uid = nowUser.value("uid").toString();
        qDebug() << "uid: " << uid;
        while (it != users.end() && it->userId() < uid)
        {
            // insert
            QSqlRecord newUser = userModel.record();
            newUser.setValue("uid", it->userId());
            newUser.setValue("name", it->username());
            newUser.setValue("avatar", it->avatar());
            if (!userModel.insertRecord(-1, newUser))
            {
                qDebug() << "insert user failed: " << userModel.lastError();
            }
            ++it;
        }
        if (it == users.end() || it->userId() != uid)
        {
            continue;
        }
        else
        {
            // update user info
            nowUser.setValue("name", it->username());
            nowUser.setValue("avatar", it->avatar());
            if (!userModel.setRecord(i, nowUser))
            {
                qDebug() << "update user failed: " << userModel.lastError();
            }
            ++it;
        }
    }
    while (it != users.end())
    {
        // insert
        QSqlRecord newUser = userModel.record();
        newUser.setValue("uid", it->userId());
        newUser.setValue("name", it->username());
        newUser.setValue("avatar", it->avatar());
        if (!userModel.insertRecord(-1, newUser))
        {
            qDebug() << "insert user failed: " << userModel.lastError();
        }
        ++it;
    }
    // userModel.submitAll();
}

} // namespace MINIOICQ
