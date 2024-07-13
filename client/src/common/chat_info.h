#ifndef CHAT_INFO_H
#define CHAT_INFO_H

#include <QDateTime>
#include <QImage>
#include <QString>

namespace MINIOICQ
{

class ChatInfo
{
    QString _cid;
    QString _name;
    QImage _avatar;
    QString _owner_id;
    QDateTime _creation_time;
    QDateTime _last_view;

public:
    ChatInfo() = default;
    ChatInfo(QString cid, QString name, QImage avatar, QString owner_id,
             QDateTime creation_time, QDateTime last_view)
        : _cid(cid), _name(name), _avatar(avatar), _owner_id(owner_id),
          _creation_time(creation_time), _last_view(last_view)
    {
    }

    // data
    QString chatId() const { return _cid; }
    QString name() const { return _name; }
    QImage avatar() const { return _avatar; }
    QString ownerId() const { return _owner_id; }
    QDateTime startTime() const { return _creation_time; }
    QDateTime lastViewTime() const { return _last_view; }

    // set data
    void setChatId(QString cid) { _cid = cid; }
    void setName(QString name) { _name = name; }
    void setAvatar(QImage avatar) { _avatar = avatar; }
    void setOwnerId(QString owner_id) { _owner_id = owner_id; }
    void setCreationTime(QDateTime time) { _creation_time = time; }
    void setLastView(QDateTime last_view) { _last_view = last_view; }
};

} // namespace MINIOICQ
#endif
