#include "chat_viewmodel.h"
#include <QDebug>
#include <QImage>
#include <QSqlRecord>

namespace MINIOICQ
{

ChatViewModel::ChatViewModel(QVariant userId, QVariant chatId, QObject* parent)
    : QSortFilterProxyModel(parent), _chatId(chatId), _userId(userId)
{
    qDebug() << "ChatViewModel::ChatViewModel";
    qDebug() << "_chatId: " << _chatId << ", _userId: " << _userId;
}

ChatViewModel::~ChatViewModel() {}

void ChatViewModel::setSourceModel(ChatModel* model)
{
    QSortFilterProxyModel::setSourceModel(model);
    connect(model, &ChatModel::newMsg, this, &ChatViewModel::on_newMsg);

    _midColumn = model->record().indexOf("mid");
    _mtypeColumn = model->record().indexOf("mtype");
    _messageColumn = model->record().indexOf("message");
    _sendTimeColumn = model->record().indexOf("send_time");
    _senderIdColumn = model->record().indexOf("sender_id");
    _nameColumn = model->record().indexOf("name");
    _avatarColumn = model->record().indexOf("avatar");
    emit dataChanged(index(0, 0), index(rowCount(), columnCount()));
}

QVariant ChatViewModel::data(const QModelIndex& index, int /* role */) const
{
    auto ret = QSortFilterProxyModel::data(index, Qt::DisplayRole);
    qDebug() << "ChatViewModel::data: " << index.row() << ", " << index.column();
    if (index.column() == avatarColumn())
    {
        // convert QByteArray to QImage
        qDebug() << "ChatViewModel::data: avatarColumn";
        // QImage will read file header to determine the format
        return QImage::fromData(ret.toByteArray());
    }
    return ret;
}

void ChatViewModel::on_newMsg()
{
    qDebug() << "ChatViewModel::on_newMsg" << _chatId;
    emit dataChanged(index(0, 0), index(rowCount(), columnCount()));
}

void ChatViewModel::setWsConnector(WebSocketConnector* wsConnector)
{
    qDebug() << "ChatViewModel::setWsConnector" << wsConnector;
    connect(this, &ChatViewModel::send, wsConnector,
            &WebSocketConnector::on_send);
}

void ChatViewModel::on_send(Message msg)
{
    qDebug() << "ChatViewModel::on_send";
    qDebug() << "_chatId: " << _chatId << ", _userId: " << _userId;
    msg.setChatId(_chatId.toString());
    msg.setSender(_userId.toString());
    emit send(msg);
}

void ChatViewModel::on_closeChat()
{
    qDebug() << "ChatViewModel::on_closeChat";
    // to ChatManager
    emit closeChat(_chatId.toInt());
}

} // namespace MINIOICQ
