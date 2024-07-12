#include <QSqlDatabase>
#include <QSqlQuery>

#include "chat_model.h"

namespace MINIOICQ
{

ChatModel::ChatModel(QObject* parent, QSqlDatabase db, QVariant chatId)
    : QSqlTableModel(parent, db), _chatId(chatId)
{
    qDebug() << "ChatModel::ChatModel()";
    Q_ASSERT(chatId.type() == QVariant::Type::Int);
    auto tables = db.tables();
    QString viewName = "chat_messages_view_" + chatId.toString();
    if (!tables.contains(viewName))
    {
        qDebug() << "View " + viewName + " not found, creating new view";
        QSqlQuery createView(db);
        createView.prepare("CREATE VIEW " + viewName + " AS "
                           "SELECT mid, message, send_time, sender_id, name, avatar"
                           "FROM messages JOIN users ON messages.sender_id = users.userId"
                           "WHERE chatId = :chatId"
                           "ORDER BY send_time ASC");
        createView.bindValue(":chatId", chatId.toString());
        if (!createView.exec())
        {
            qDebug() << "Create view failed: " << createView.lastError();
            throw std::runtime_error("Create view failed");
        }
    }
    setTable(viewName);
    select();
}

ChatModel::~ChatModel() {
    qDebug() << "ChatModel::~ChatModel()";
    qDebug() << "Drop view " + tableName();
    QSqlQuery dropView(database());
    dropView.prepare("DROP VIEW " + tableName());
    if (!dropView.exec())
    {
        qDebug() << "Drop view failed: " << dropView.lastError();
        throw std::runtime_error("Drop view failed");
    }
}

} // namespace MINIOICQ