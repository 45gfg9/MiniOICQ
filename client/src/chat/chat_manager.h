#ifndef CHAT_MANAGER_H
#define CHAT_MANAGER_H

#include <QVariant>
#include <QVector>

#include "chat_view.h"
#include "chat_viewmodel.h"

namespace MINIOICQ
{

struct Chat
{
    QVariant chatId;
    ChatView* view;
    ChatViewModel* viewModel;
    ChatModel* model;
};

class ChatManager : public QObject
{
    Q_OBJECT

public:
    ChatManager(const QString userId, const QSqlDatabase& db, WebSocketConnector* ws,
                QObject* parent = nullptr);
    ~ChatManager();

    QVector<Chat> getChats() const { return _chats; };

    // from ListView
    void closeAll();

public slots:
    void on_newMsg(int cid);
    void on_openChat(int cid);
    void on_closeChat(const QVariant& chatId);

Q_SIGNALS:
    void newMsg(int cid);

private:
    QSqlDatabase _db;
    QString _userId;
    WebSocketConnector* _ws;
    QVector<Chat> _chats;
};

} // namespace MINIOICQ

#endif // CHAT_MANAGER_H
