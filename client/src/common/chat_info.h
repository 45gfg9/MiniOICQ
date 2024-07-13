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
    QDateTime _created;
    QDateTime _last_view;
    QStringList _members;

public:
    ChatInfo() = default;
    ChatInfo(QString cid, QString name, QImage avatar, QString owner_id,
             QDateTime created, QDateTime last_view, QStringList members)
        : _cid(cid), _name(name), _avatar(avatar), _owner_id(owner_id),
          _created(created), _last_view(last_view), _members(members)
    {
    }

    // data
    QString chatId() const { return _cid; }
    QString name() const { return _name; }
    QImage avatar() const { return _avatar; }
    QString ownerId() const { return _owner_id; }
    QDateTime startTime() const { return _created; }
    QDateTime lastViewTime() const { return _last_view; }
    QStringList members() const { return _members; }

    // set data
    void setChatId(QString cid) { _cid = cid; }
    void setName(QString name) { _name = name; }
    void setAvatar(QImage avatar) { _avatar = avatar; }
    void setOwnerId(QString owner_id) { _owner_id = owner_id; }
    void setCreationTime(QDateTime time) { _created = time; }
    void setLastView(QDateTime last_view) { _last_view = last_view; }
    void setMembers(QStringList members) { _members = members; }
};

} // namespace MINIOICQ
#endif
