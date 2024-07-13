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

enum class MessageType
{
    Text,
    Image,
    File,
    Audio,
    Video,
};

static inline QString MessageTypeToQString(MessageType type)
{
    switch (type)
    {
        case MessageType::Text:
            return "Text";
        case MessageType::Image:
            return "Image";
        case MessageType::File:
            return "File";
        case MessageType::Audio:
            return "Audio";
        case MessageType::Video:
            return "Video";
        default:
            return "Unknown";
    }
}

class AbstractMessage
{
public:
    // used for ws to construct a full-feature message
    AbstractMessage(QString msgId, QString chatId, QString sender,
                    MessageType type, QString content, QDateTime time)
        : _msgId(msgId), _chatId(chatId), _sender(sender), _type(type),
          _time(time), _content(content)
    {
    }

    virtual ~AbstractMessage() = default;

    QJsonObject pack() const
    {
        QJsonObject obj;
        obj["chat_id"] = _chatId;
        obj["type"] = MessageTypeToQString(_type);
        obj["content"] = _content;
        return obj;
    }
    QString msgId() const { return _msgId; }
    QString chatId() const { return _chatId; }
    QString sender() const { return _sender; }
    MessageType type() const { return _type; }
    QDateTime time() const { return _time; }
    QString content() const { return _content; }
    QImage avatar() const { return _avatar; }

    void setAvatar(QImage avatar) { _avatar = avatar; }

protected:
    AbstractMessage(QString sender, MessageType type)
        : AbstractMessage("-1", "-1", sender, type, "",
                          QDateTime::currentDateTime())
    {
    }

    QString _msgId;
    QString _chatId;
    QString _sender;
    MessageType _type;
    QDateTime _time;
    QImage _avatar;
    QString _content;
};

class TextMessage : public AbstractMessage
{
public:
    explicit TextMessage(QString sender, const QString& text)
        : AbstractMessage(sender, MessageType::Text)
    {
        _content = text;
    }
};

class ImageMessage : public AbstractMessage
{
public:
    explicit ImageMessage(QString sender, const QImage& image)
        : AbstractMessage(sender, MessageType::Image)
    {
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG");
        _content = QString::fromUtf8(byteArray.toBase64());
    }
};

class FileMessage : public AbstractMessage
{
    QByteArray _file;
    QString _fileName;

public:
    explicit FileMessage(QString sender, const QByteArray& file,
                         const QString& fileName)
        : AbstractMessage(sender, MessageType::File), _file(file),
          _fileName(fileName)
    {
    }

    QByteArray fileData() const { return _file; }
    QString fileName() const { return _fileName; }
};

class AudioMessage : public AbstractMessage
{
    QAudioBuffer _audio;

public:
    explicit AudioMessage(QString sender, const QAudioBuffer& audio)
        : AbstractMessage(sender, MessageType::Audio), _audio(audio)
    {
    }

    QAudioBuffer audio() const { return _audio; }
};

class VideoMessage : public AbstractMessage
{
    QVideoFrame _video;

public:
    explicit VideoMessage(QString sender, const QVideoFrame& video)
        : AbstractMessage(sender, MessageType::Video), _video(video)
    {
    }

    QVideoFrame video() const { return _video; }
};

} // namespace MINIOICQ

#endif
