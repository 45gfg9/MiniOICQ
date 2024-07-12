#ifndef CHAT_MANAGER_H
#define CHAT_MANAGER_H

#include <QMap>
#include <QPair>
#include <QVariant>

#include "chat_view.h"
#include "chat_viewmodel.h"

namespace MINIOICQ
{

class ChatManager
{
public:
    ChatManager();
    ~ChatManager();

    void openChat(const QVariant& chatId);
    void closeChat(const QVariant& chatId);
    void closeAllChats();

private:
    QMap<QVariant, QPair<ChatView *, ChatViewModel *> > _chats;
};

} // namespace MINIOICQ

#endif // CHAT_MANAGER_H