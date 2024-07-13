#ifndef LIST_VIEWMODEL_H
#define LIST_VIEWMODEL_H

#include <QSortFilterProxyModel>

#include "list_model.h"
#include "websocket/websocket.h"
#include "chat/chat_manager.h"
#include "common/chat_info.h"
#include "common/user_info.h"
#include "common/message.h"

namespace MINIOICQ
{

class ListViewModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    int chatIdColumn() const { return _chatIdColumn; }
    int chatNameColumn() const { return _chatNameColumn; }
    int chatAvatarColumn() const { return _chatAvatarColumn; }
    int chatLastMessageColumn() const { return _chatLastMessageColumn; }
    int chatLastMessageTimeColumn() const { return _chatLastMessageTimeColumn; }
    int chatUnreadMessageCountColumn() const
    {
        return _chatUnreadMessageCountColumn;
    }

    void setSourceModel(ListModel* model);
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override;

    void setWsConnector(WebSocketConnector* wsConnector);

    QVector<MINIOICQ::UserInfo> selectUser();

Q_SIGNALS:
    // send to websocket
    void createChat(QVector<UserInfo> users);

public slots:

    // from ListView
    void on_itemList_clicked(const QVariant& chatId);
    void on_closeButton_clicked();
    void on_invite(const QVector<UserInfo> &users);

    // from WebSocketConnector
    void on_newMsg(QVector<MINIOICQ::AbstractMessage> & messages);
    void on_newChat(QVector<MINIOICQ::ChatInfo> chat);
    void on_newUser(QVector < MINIOICQ::UserInfo> & users);

private:
    int _chatIdColumn = -1;
    int _chatNameColumn = -1;
    int _chatAvatarColumn = -1;
    int _chatLastMessageColumn = -1;
    int _chatLastMessageTimeColumn = -1;
    int _chatUnreadMessageCountColumn = -1;

    ChatManager* _chatManager = nullptr;
};

} // namespace MINIOICQ

#endif // LIST_VIEWMODEL_H
