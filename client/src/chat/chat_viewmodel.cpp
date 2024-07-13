#include "chat_viewmodel.h"
#include <QDebug>
#include <QImage>
#include <QSqlRecord>

namespace MINIOICQ
{

ChatViewModel::ChatViewModel(QVariant chatId, QObject* parent)
    : QSortFilterProxyModel(parent), _chatId(chatId)
{
}

ChatViewModel::~ChatViewModel() {}

void ChatViewModel::setSourceModel(ChatModel* model)
{
    QSortFilterProxyModel::setSourceModel(model);

    _midColumn = model->record().indexOf("mid");
    _mtypeColumn = model->record().indexOf("mtype");
    _messageColumn = model->record().indexOf("message");
    _sendTimeColumn = model->record().indexOf("send_time");
    _senderIdColumn = model->record().indexOf("sender_id");
    _nameColumn = model->record().indexOf("name");
    _avatarColumn = model->record().indexOf("avatar");
}

QVariant ChatViewModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        auto ret = QSortFilterProxyModel::data(index, role);
        if (index.column() == avatarColumn())
        {
            // convert QByteArray to QImage
            qDebug() << "ChatViewModel::data: avatarColumn";
            // QImage will read file header to determine the format
            return QImage::fromData(ret.toByteArray());
        }
        return ret;
    }
    else
    {
        qDebug() << "ChatViewModel::data: role not implemented";
    }
    return QVariant();
}

void ChatViewModel::update() { qDebug() << "ChatViewModel::update"; }

void ChatViewModel::setWsConnector(WebSocketConnector* wsConnector)
{
    connect(this, &ChatViewModel::send, wsConnector,
            &WebSocketConnector::on_send);
}

void ChatViewModel::on_send(const Message& msg)
{
    qDebug() << "ChatViewModel::on_send";
    emit send(msg);
}

} // namespace MINIOICQ
