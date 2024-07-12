#ifndef CHAT_MANAGER_H
#define CHAT_MANAGER_H

#include <QVector>
#include <QVariant>

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

class ChatManager: public QObject
{
    Q_OBJECT

public:
    ~ChatManager();

    // from ListView
    void openChat(const QVariant& chatId);
    void closeAll();

public slots:
    void on_closeChat(const QVariant& chatId);

private:
    QSqlDatabase _db;
    QVector<Chat> _chats;
};

} // namespace MINIOICQ

#endif // CHAT_MANAGER_H
