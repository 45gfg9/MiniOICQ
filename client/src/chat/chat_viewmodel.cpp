#include "chat_viewmodel.h"
#include <QSqlRecord>

namespace MINIOICQ
{

ChatViewModel::ChatViewModel(QVariant chatId, QObject* parent)
    : QSortFilterProxyModel(parent)
    , _chatId(chatId) {}

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
        return QSortFilterProxyModel::data(index, role);
    }
    return QVariant();
}

} // namespace MINIOICQ
