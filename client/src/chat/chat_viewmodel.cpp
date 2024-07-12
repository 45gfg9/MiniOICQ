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

    _midColumn = model->fieldIndex("mid");
    _messageColumn = model->fieldIndex("message");
    _sendTimeColumn = model->fieldIndex("send_time");
    _senderIdColumn = model->fieldIndex("sender_id");
    _nameColumn = model->fieldIndex("name");
    _avatarColumn = model->fieldIndex("avatar");
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
