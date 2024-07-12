#include "chat_viewmodel.h"

namespace MINIOICQ
{

ChatViewModel::ChatViewModel(QVariant chatId, QObject* parent)
    : QSortFilterProxyModel(parent)
    , _chatId(chatId) {}

ChatViewModel::~ChatViewModel() {}

void ChatViewModel::setSourceModel(ChatModel* model)
{
    QSortFilterProxyModel::setSourceModel(model);

    for(int i = 0; i < model->columnCount(); i++)
    {
        QString columnName = model->headerData(i, Qt::Horizontal).toString();
        if (columnName == "mid")
        {
            _midColumn = i;
        }
        else if (columnName == "message")
        {
            _messageColumn = i;
        }
        else if (columnName == "send_time")
        {
            _sendTimeColumn = i;
        }
        else if (columnName == "sender_id")
        {
            _senderIdColumn = i;
        }
        else if (columnName == "name")
        {
            _nameColumn = i;
        }
        else if (columnName == "avatar")
        {
            _avatarColumn = i;
        }
    }
}

QVariant ChatViewModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return QSortFilterProxyModel::data(index, role);
    }
    return QVariant();
}

} // namespace MINIOICQ
