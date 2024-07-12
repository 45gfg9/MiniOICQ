#include <QMap>
#include <QPair>
#include <QVariant>

#include "chat_manager.h"
#include "chat_view.h"

namespace MINIOICQ
{

ChatManager::ChatManager() {}

ChatManager::~ChatManager() {
    closeAllChats();
}

void ChatManager::openChat(const QVariant& chatId) {
    qDebug() << "ChatManager::openChat";
    if (_chats.contains(chatId)) {
        // Chat already opened
        return;
    }

    ChatView* chatView = new ChatView();

    // TODO
}

void ChatManager::closeChat(const QVariant& chatId) {
    // TODO
}

void ChatManager::closeAllChats() {
    // TODO
}

} // namespace MINIOICQ