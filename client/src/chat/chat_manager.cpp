#include <QDebug>
#include <QVariant>
#include <algorithm>

#include "chat_manager.h"

namespace MINIOICQ
{

ChatManager::ChatManager(const QString userId, const QSqlDatabase& db, WebSocketConnector* ws,
                         QObject* parent)
    : QObject(parent), _db(db), _userId(userId), _ws(ws)
{
}

ChatManager::~ChatManager() { closeAll(); }

void ChatManager::on_newMsg(int cid)
{
    qDebug() << "ChatManager::on_newMsg" << cid;
    emit newMsg(cid);
}

void ChatManager::on_openChat(int cid)
{
    QVariant chatId = QVariant::fromValue(cid);
    qDebug() << "ChatManager::openChat" << chatId;
    auto chatIt =
        std::find_if(_chats.begin(), _chats.end(), [chatId](const Chat& chat)
                     { return chat.chatId == chatId; });

    if (chatIt != _chats.end())
    {
        return;
    }

    ChatView* view = new ChatView();
    ChatViewModel* viewModel = new ChatViewModel(_userId, chatId);
    ChatModel* model = new ChatModel(_db, chatId);
    qDebug() << "ChatManager::openChat: chatId" << chatId;
    qDebug() << "ChatModel rows" << model->rowCount();
    viewModel->setSourceModel(model);
    viewModel->setWsConnector(_ws);
    bindChatView(view, viewModel);
    _chats.push_back({chatId, view, viewModel, model});

    connect(this, &ChatManager::newMsg, model, &ChatModel::on_newMsg);
    connect(viewModel, &ChatViewModel::closeChat, this, &ChatManager::on_closeChat);
    view->show();
}

void ChatManager::closeAll()
{
    for (auto& chat : _chats)
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
    auto chatIt =
        std::find_if(_chats.begin(), _chats.end(), [chatId](const Chat& chat)
                     { return chat.chatId == chatId; });
    if (chatIt == _chats.end())
    {
        qDebug() << "ChatManager::on_closeChat: chatId not found";
        return;
    }

    delete chatIt->view;
    delete chatIt->viewModel;
    delete chatIt->model;
    _chats.erase(chatIt);
}

} // namespace MINIOICQ
