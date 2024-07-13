#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDateTime>
#include <QFile>
#include <QImage>
#include <QJsonObject>
#include <QTimeZone>
#include <QtMultimedia>

namespace MINIOICQ
{

class Message
{
public:
    // used for ws to construct a full-feature message
    Message(QString msgId, QString chatId, QString sender, QString type,
            QByteArray content, QDateTime time)
        : _msgId(msgId), _chatId(chatId), _sender(sender), _type(type),
          _content(content), _time(time)
    {
    }

    QJsonObject pack() const
    {
        QJsonObject obj;
        obj["chat_id"] = _chatId;
        obj["type"] = _type;
        obj["content"] = QString(_content.toBase64());
        obj["time"] = _time.toString(Qt::ISODate);
        return obj;
    }
    QString msgId() const { return _msgId; }
    QString chatId() const { return _chatId; }
    QString sender() const { return _sender; }
    QString type() const { return _type; }
    QDateTime time() const { return _time; }
    QByteArray content() const { return _content; }
    QImage avatar() const { return _avatar; }

    void setSender(QString sender) { _sender = sender; }
    void setChatId(QString chatId) { _chatId = chatId; }
    void setAvatar(QImage avatar) { _avatar = avatar; }

private:

    QString _msgId;
    QString _chatId;
    QString _sender;
    /**
     * @brief MIME type of the content
     *
     * currently we support:
     *
     * - text/plain
     * - text/html
     * - image/png
     * - image/jpeg
     *
     */
    QString _type;
    QByteArray _content;
    QDateTime _time;

    // only for view
    QImage _avatar;
};

} // namespace MINIOICQ

#endif
