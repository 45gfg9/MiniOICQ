#include <QVariant>
#include <QDebug>
#include <algorithm>

#include "chat_manager.h"

namespace MINIOICQ
{

ChatManager::ChatManager() {}

ChatManager::~ChatManager() { closeAll(); }

void ChatManager::openChat(const QVariant& chatId)
{
    qDebug() << "ChatManager::openChat";
    auto chatIt = std::find_if(_chats.begin(), _chats.end(),
                               [chatId](const Chat& chat) {
                                   return chat.chatId == chatId;
                               });

    if(chatIt != _chats.end())
    {
        return;
    }

    ChatView* view = new ChatView();
    ChatViewModel* viewModel = new ChatViewModel();
    ChatModel* model = new ChatModel(nullptr, _db, chatId);
    viewModel->setSourceModel(model);
    bindChatView(view, viewModel);
    _chats.push_back({chatId, view, viewModel, model});
}

void ChatManager::closeAll()
{
    for(auto& chat : _chats)
    {
        delete chat.view;
        delete chat.viewModel;
        delete chat.model;
    }
}

void ChatManager::on_closeChat(const QVariant& chatId)
{
    qDebug() << "ChatManager::on_closeChat";
    // check if chatId exists
    auto chatIt = std::find_if(_chats.begin(), _chats.end(),
                               [chatId](const Chat& chat) {
                                   return chat.chatId == chatId;
                               });
    if(chatIt == _chats.end())
    {
        return;
    }

    delete chatIt->view;
    delete chatIt->viewModel;
    delete chatIt->model;
    _chats.erase(chatIt);
}

} // namespace MINIOICQ
