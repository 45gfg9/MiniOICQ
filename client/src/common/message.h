#ifndef MESSAGE_H
#define MESSAGE_H

#include <msgpack.hpp>

namespace MINIOICQ
{

enum class MessageType
{
    text,
    image,
    file,
    voice,
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

    void pack(uint64_t cid,
              msgpack::packer<msgpack::sbuffer>& pk) const override
    {
        pk.pack_map(3);
        pk.pack("type");
        pk.pack("text");

        // TODO
    }
};

} // namespace MINIOICQ

#endif
