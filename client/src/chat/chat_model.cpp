#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

#include "chat_model.h"

namespace MINIOICQ
{

ChatModel::ChatModel(QSqlDatabase db, QVariant chatId, QObject* parent)
    : QSqlQueryModel(parent), _chatId(chatId), _db(db), _viewName("chat_messages_view_" + chatId.toString())
{
    qDebug() << "ChatModel::ChatModel()";
    Q_ASSERT(chatId.type() == QVariant::Type::Int);
    qDebug() << "Create view " + _viewName;
    QSqlQuery createView(_db);
    createView.prepare("CREATE VIEW " + _viewName + " AS "
                        "SELECT mid, mtype, message, send_time, sender_id, name, avatar"
                        "FROM messages JOIN users ON messages.sender_id = users.userId"
                        "WHERE cid = :chatId"
                        "ORDER BY send_time ASC");
    createView.bindValue(":chatId", chatId.toString());
    if (!createView.exec())
    {
        qDebug() << "Create view failed: " << createView.lastError();
        throw std::runtime_error("Create view failed");
    }
    refresh();
}

ChatModel::~ChatModel() {
    qDebug() << "ChatModel::~ChatModel()";
    qDebug() << "Drop view " + _viewName;
    QSqlQuery dropView(_db);
    dropView.prepare("DROP VIEW " + _viewName);
    if (!dropView.exec())
    {
        qDebug() << "Drop view failed: " << dropView.lastError();
        throw std::runtime_error("Drop view failed");
    }
}

void ChatModel::on_message_received()
{
    refresh();
    emit message_received();
}

void ChatModel::refresh()
{
    setQuery("SELECT * FROM " + _viewName, _db);
    setHeaderData(0, Qt::Horizontal, "mid");
    setHeaderData(1, Qt::Horizontal, "mtype");
    setHeaderData(2, Qt::Horizontal, "message");
    setHeaderData(3, Qt::Horizontal, "send_time");
    setHeaderData(4, Qt::Horizontal, "sender_id");
    setHeaderData(5, Qt::Horizontal, "name");
    setHeaderData(6, Qt::Horizontal, "avatar");
}

} // namespace MINIOICQ
