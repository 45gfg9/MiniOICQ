#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDateTime>
#include <QFile>
#include <QImage>
#include <QTimeZone>
#include <QtMultimedia>
#include <msgpack.hpp>

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

class AbstractMessage
{
public:
    // sender is id or name
    AbstractMessage(QString sender, MessageType type,
                    QDateTime time = QDateTime::currentDateTime())
        : _sender(sender), _type(type), _time(time)
    {
    }
    virtual ~AbstractMessage() = default;

    virtual void pack(uint64_t cid,
                      msgpack::packer<msgpack::sbuffer>& pk) const = 0;
    MessageType type() const { return _type; }
    QString sender() const { return _sender; }
    QDateTime time() const { return _time; }

protected:
    MessageType _type;

private:
    QString _sender;
    QDateTime _time;
};

class TextMessage : public AbstractMessage
{
    QString _text;

public:
    explicit TextMessage(QString sender, const QString& text)
        : AbstractMessage(sender, MessageType::Text), _text(text)
    {
    }

    QString text() const { return _text; }

    void pack(uint64_t cid,
              msgpack::packer<msgpack::sbuffer>& pk) const override
    {
        pk.pack_map(3);
        pk.pack("type");
        pk.pack("text");

        // TODO
    }
};

class ImageMessage : public AbstractMessage
{
    QImage _image;

public:
    explicit ImageMessage(QString sender, const QImage& image)
        : AbstractMessage(sender, MessageType::Image), _image(image)
    {
    }

    QImage image() const { return _image; }

    void pack(uint64_t cid,
              msgpack::packer<msgpack::sbuffer>& pk) const override
    {
        // TODO
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

    void pack(uint64_t cid,
              msgpack::packer<msgpack::sbuffer>& pk) const override
    {
        // TODO
    }
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

    void pack(uint64_t cid,
              msgpack::packer<msgpack::sbuffer>& pk) const override
    {
        // _audio.data();
        // _audio.format();
        // TODO
    }
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

    void pack(uint64_t cid,
              msgpack::packer<msgpack::sbuffer>& pk) const override
    {
        // _video.bits(int plane);
        // TODO
    }
};

} // namespace MINIOICQ

#endif
