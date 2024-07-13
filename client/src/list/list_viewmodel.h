#ifndef LIST_VIEWMODEL_H
#define LIST_VIEWMODEL_H

#include <QSortFilterProxyModel>

#include "chat/chat_manager.h"
#include "common/chat_info.h"
#include "common/message.h"
#include "common/user_info.h"
#include "list_model.h"
#include "websocket/websocket.h"

namespace MINIOICQ
{

class ListViewModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    int chatIdColumn() const { return _chatIdColumn; }
    int chatNameColumn() const { return _chatNameColumn; }
    int chatAvatarColumn() const { return _chatAvatarColumn; }
    int chatLastMessageTypeColumn() const { return _chatLastMessageTypeColumn; }
    int chatLastMessageColumn() const { return _chatLastMessageColumn; }
    int chatLastMessageTimeColumn() const { return _chatLastMessageTimeColumn; }
    int chatUnreadMessageCountColumn() const
    {
        return _chatUnreadMessageCountColumn;
    }
    QVariant userId() const { return _userId; }
    void setUserId(const QVariant& userId) { _userId = userId; }

    void setSourceModel(ListModel* model);
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override;

    void setWsConnector(WebSocketConnector* wsConnector);

    QVector<UserInfo> selectUser();

Q_SIGNALS:
    // to WebSocketConnector
    void sync();
    void createChat(QVector<UserInfo> users);

    // to ChatManager
    void openChat(int cid);

    // to ChatViewModel
    void newMsg(int cid);

public slots:

    // debug
    void on_debug();
    void invalidate();
    

    // from ListView
    void on_openChat(int cid);
    void on_closeAll();
    void on_invite(const QVector<UserInfo>& users);

    // from WebSocketConnector
    void on_newMsg(QVector<MINIOICQ::Message>& messages);
    void on_newChat(QVector<MINIOICQ::ChatInfo>& chat);
    void on_newUser(QVector<MINIOICQ::UserInfo>& users);

private:
    int _chatIdColumn = -1;
    int _chatNameColumn = -1;
    int _chatAvatarColumn = -1;
    int _chatLastMessageTypeColumn = -1;
    int _chatLastMessageColumn = -1;
    int _chatLastMessageTimeColumn = -1;
    int _chatUnreadMessageCountColumn = -1;

    QVariant _userId;
    ChatManager* _chatManager = nullptr;
    WebSocketConnector * _wsConnector = nullptr;
};

} // namespace MINIOICQ

#endif // LIST_VIEWMODEL_H
