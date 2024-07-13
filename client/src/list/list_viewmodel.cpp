#include "list_viewmodel.h"
#include <QDebug>
#include <QSortFilterProxyModel>
#include <QSqlRecord>

namespace MINIOICQ
{

void ListViewModel::setSourceModel(ListModel* model)
{
    QSortFilterProxyModel::setSourceModel(model);
    _chatIdColumn = model->record().indexOf("cid");
    _chatNameColumn = model->record().indexOf("name");
    _chatAvatarColumn = model->record().indexOf("avatar");
    _chatLastMessageColumn = model->record().indexOf("last_message");
    _chatLastMessageTimeColumn = model->record().indexOf("last_send_time");
    _chatUnreadMessageCountColumn = model->record().indexOf("un_read_count");
    _chatManager = new ChatManager();
}

QVariant ListViewModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return QSortFilterProxyModel::data(index, role);
    }
    else
    {
        qDebug() << "ListViewModel::data: role not supported";
    }
    return QVariant();
}

void ListViewModel::setWsConnector(WebSocketConnector* wsConnector)
{
    connect(wsConnector, &WebSocketConnector::newMsg, this,
            &ListViewModel::on_newMsg);
}

void ListViewModel::on_itemList_clicked(const QVariant& chatId)
{
    qDebug() << "ListViewModel::on_itemList_clicked: chatId=" << chatId;
    auto chatIdStr = chatId.toString();
    _chatManager->openChat(chatIdStr);
}

void ListViewModel::on_closeButton_clicked()
{
    qDebug() << "ListViewModel::on_closeButton_clicked";
}

void ListViewModel::on_newMsg(QVector<MINIOICQ::Message> & messages)
{
    qDebug() << "ListViewModel::on_newMsg";
    ListModel* model = qobject_cast<ListModel*>(sourceModel());
    QSqlDatabase db = model->database();
    QSqlTableModel msgModel(nullptr, db);
    msgModel.setTable("messages");
    for (auto msg : messages)
    {
        QSqlRecord newMsg = msgModel.record();
        newMsg.setValue("mid", msg.msgId());
        newMsg.setValue("cid", msg.chatId());
        newMsg.setValue("uid", msg.userId());
        newMsg.setValue("content", msg.content());
        newMsg.setValue("send_time", msg.sendTime());
        if (!msgModel.insertRecord(-1, newMsg)) {
            qDebug() << "insert message failed: " << msgModel.lastError();
        }
    }
}

void ListViewModel::on_newChat(QVector<MINIOICQ::ChatInfo> chats)
{
    qDebug() << "ListViewModel::on_newChat";
    qSort(chats.begin(), chats.end(), [](const ChatInfo& a, const ChatInfo& b) {
        return a.chatId < b.chatId;
    });
    ListModel* model = qobject_cast<ListModel*>(sourceModel());
    QSqlDatabase db = model->database();
    QSqlTableModel chatModel(nullptr, db);
    chatModel.setTable("chats");
    chatModel.select();
    QVector::iterator it = chats.begin();
    for (int i = 0; i < chatModel.rowCount(); ++i)
    {
        QSqlRecord nowChat = chatModel.record(i);
        QString cid = nowChat.value("cid").toString();
        qDebug() << "cid: " << cid;
        while (it != chats.end() && it->chatId() < cid) {
            // insert
            QSqlRecord newChat = chatModel.record();
            newChat.setValue("cid", it->chatId());
            newChat.setValue("name", it->name());
            newChat.setValue("avatar", it->avatar());
            newChat.setValue("oid", it->ownerId());
            newChat.setValue("last_view", it->lastViewTime());
            newChat.setValue("start_time", it->startTime());
            if (!chatModel.insertRecord(-1, newChat)) {
                qDebug() << "insert chat failed: " << chatModel.lastError();
            }
            ++it;
        }
        if (it->chatId() != cid) {
            continue;
        } else {
            // update chat info
            nowChat.setValue("name", it->name());
            nowChat.setValue("avatar", it->avatar());
            nowChat.setValue("oid", it->ownerId());
            nowChat.setValue("last_view", it->lastViewTime());
            nowChat.setValue("start_time", it->startTime());
            if (!chatModel.setRecord(i, nowChat)) {
                qDebug() << "update chat failed: " << chatModel.lastError();
            }
            ++it;
        }
    }
    while (it != chats.end()) {
        // insert
        QSqlRecord newChat = chatModel.record();
        newChat.setValue("cid", it->chatId());
        newChat.setValue("name", it->name());
        newChat.setValue("avatar", it->avatar());
        newChat.setValue("oid", it->ownerId());
        newChat.setValue("last_view", it->lastViewTime());
        newChat.setValue("start_time", it->startTime());
        if (!chatModel.insertRecord(-1, newChat)) {
            qDebug() << "insert chat failed: " << chatModel.lastError();
        }
        ++it;
    }
}

void ListViewModel::on_newUser(QVector<MINIOICQ::UserInfo>& users)
{
    qDebug() << "ListViewModel::on_newUser";
    qSort(users.begin(), users.end(), [](const UserInfo& a, const UserInfo& b) {
        return a.userId < b.userId;
    });
    ListModel* model = qobject_cast<ListModel*>(sourceModel());
    QSqlDatabase db = model->database();
    QSqlTableModel userModel(nullptr, db);
    userModel.setTable("users");
    userModel.select();
    QVector::iterator it = users.begin();
    for (int i = 0; i < userModel.rowCount(); ++i)
    {
        QSqlRecord nowUser = userModel.record(i);
        QString uid = nowUser.value("uid").toString();
        qDebug() << "uid: " << uid;
        while (it != users.end() && it->userId() < uid) {
            // insert
            QSqlRecord newUser = userModel.record();
            newUser.setValue("uid", it->userId());
            newUser.setValue("name", it->name());
            newUser.setValue("avatar", it->avatar());
            if (!userModel.insertRecord(-1, newUser)) {
                qDebug() << "insert user failed: " << userModel.lastError();
            }
            ++it;
        }
        if (it->userId() != uid) {
            continue;
        } else {
            // update user info
            nowUser.setValue("name", it->name());
            nowUser.setValue("avatar", it->avatar());
            if (!userModel.setRecord(i, nowUser)) {
                qDebug() << "update user failed: " << userModel.lastError();
            }
            ++it;
        }
    }
    while (it != users.end()) {
        // insert
        QSqlRecord newUser = userModel.record();
        newUser.setValue("uid", it->userId());
        newUser.setValue("name", it->name());
        newUser.setValue("avatar", it->avatar());
        if (!userModel.insertRecord(-1, newUser)) {
            qDebug() << "insert user failed: " << userModel.lastError();
        }
        ++it;
    }
    // userModel.submitAll();
}

} // namespace MINIOICQ
