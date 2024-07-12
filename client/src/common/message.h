#ifndef MESSAGE_H
#define MESSAGE_H

#include <QFile>
#include <QImage>
#include <QtMultimedia>
#include <msgpack.hpp>

namespace MINIOICQ
{

enum class MessageType
{
    text,
    image,
    file,
    audio,
    video,
};

class AbstractMessage
{
public:
    AbstractMessage(MessageType type) : _type(type) {}
    virtual ~AbstractMessage() = default;

    virtual void pack(uint64_t cid,
                      msgpack::packer<msgpack::sbuffer>& pk) const = 0;

protected:
    MessageType _type;
};

class TextMessage : public AbstractMessage
{
    QString _text;

public:
    explicit TextMessage(const QString& text)
        : AbstractMessage(MessageType::text), _text(text)
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
    explicit ImageMessage(const QImage& image)
        : AbstractMessage(MessageType::image), _image(image)
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
    explicit FileMessage(const QByteArray& file, const QString& fileName)
        : AbstractMessage(MessageType::file), _file(file), _fileName(fileName)
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
    explicit AudioMessage(const QAudioBuffer& audio)
        : AbstractMessage(MessageType::audio), _audio(audio)
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
    explicit VideoMessage(const QVideoFrame& video)
        : AbstractMessage(MessageType::video), _video(video)
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
