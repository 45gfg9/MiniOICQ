#ifndef CHAT_VIEWMODEL_H
#define CHAT_VIEWMODEL_H

#include <QSortFilterProxyModel>

#include "chat_model.h"

namespace MINIOICQ
{

class ChatViewModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    ChatViewModel(QVariant chatId, QObject* parent = nullptr);
    ~ChatViewModel();

    int midColumn() const { return _midColumn; }
    int messageColumn() const { return _messageColumn; }
    int sendTimeColumn() const {return _sendTimeColumn; }
    int senderIdColumn() const { return _senderIdColumn; }
    int nameColumn() const { return _nameColumn; }
    int avatarColumn() const { return _avatarColumn; }
    QVariant chatId() const { return _chatId; }

    void setSourceModel(ChatModel* model);
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

public slots:
    void on_newMsg(/* QVector<MINIOICQ::Message> & messages */);
    void on_newChat(/* QVector<MINIOICQ::ChatInfo> chat */);
    void on_newUser(QVector < MINIOICQ::UserInfo> & users);

private:
// mid, message, send_time, sender_id, name, avatar
    int _midColumn = -1;      // INT
    int _messageColumn = -1;  // BLOB
    int _sendTimeColumn = -1; // TIMESTAMP
    int _senderIdColumn = -1; // INT
    int _nameColumn = -1;     // VARCHAR
    int _avatarColumn = -1;   // BLOB
    QVariant _chatId;
};

} // namespace MINIOICQ

#endif // CHAT_VIEWMODEL_H
