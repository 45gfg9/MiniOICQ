#include <QSortFilterProxyModel>
#include "list_proxymodel.h"

namespace MINIOICQ {

void ListProxyModel::setSourceModel(ListModel* model)
{
    QSortFilterProxyModel::setSourceModel(model);
    _chatIdColumn = model->record().indexOf("cid");
    _chatNameColumn = model->record().indexOf("name");
    _chatAvatarColumn = model->record().indexOf("avatar");
    _chatLastMessageColumn = model->record().indexOf("last_message");
    _chatLastMessageTimeColumn = model->record().indexOf("last_send_time");
    _chatUnreadMessageCountColumn = model->record().indexOf("un_read_count");
}

QVariant ListProxyModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole) {
        return QSortFilterProxyModel::data(index, role);
    }
    return QVariant();
}

void ListProxyModel::on_click_chat(const QVariant &chatId)
{
    qDebug() << "ListProxyModel::on_click_chat";
    // Open chat window
    emit open_chat(chatId);
}

} // namespace MINIOICQ