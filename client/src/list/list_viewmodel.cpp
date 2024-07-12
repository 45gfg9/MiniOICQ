#include "list_viewmodel.h"
#include <QDebug>
#include <QSortFilterProxyModel>
#include <QSqlRecord>

namespace MINIOICQ
{

void ListViewModel::setSourceModel(ListModel* model)
{
    QSortFilterProxyModel::setSourceModel(model);
    _chatIdColumn = model->record().indexOf("cid");
    _chatNameColumn = model->record().indexOf("name");
    _chatAvatarColumn = model->record().indexOf("avatar");
    _chatLastMessageColumn = model->record().indexOf("last_message");
    _chatLastMessageTimeColumn = model->record().indexOf("last_send_time");
    _chatUnreadMessageCountColumn = model->record().indexOf("un_read_count");
    _chatManager = new ChatManager();
}

QVariant ListViewModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return QSortFilterProxyModel::data(index, role);
    }
    else
    {
        qDebug() << "ListViewModel::data: role not supported";
    }
    return QVariant();
}

void ListViewModel::setWsConnector(WebSocketConnector* wsConnector)
{
    connect(wsConnector, &WebSocketConnector::newMsg, this,
            &ListViewModel::on_newMsg);
}

void ListViewModel::on_itemList_clicked(const QVariant& chatId)
{
    qDebug() << "ListViewModel::on_itemList_clicked: chatId=" << chatId;
    auto chatIdStr = chatId.toString();
    _chatManager->openChat(chatIdStr);
}

void ListViewModel::on_closeButton_clicked()
{
    qDebug() << "ListViewModel::on_closeButton_clicked";
}

void ListViewModel::on_newMsg(/* QVector<MINIOICQ::Message> & messages */)
{
    qDebug() << "ListViewModel::on_newMsg";
}

} // namespace MINIOICQ
