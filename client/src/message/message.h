#ifndef MESSAGE_H
#define MESSAGE_H

#include <msgpack.hpp>

enum class MessageType {
    text,
    image,
    file,
    voice,
    video,
};

struct AbstractMessage {
    virtual ~AbstractMessage() = default;

    virtual void pack(uint64_t cid, msgpack::packer<msgpack::sbuffer>& pk) const = 0;
};

class Text : public AbstractMessage {
    QString _text;

public:
    Text(const QString& text) : _text(text) {}

    void pack(uint64_t cid, msgpack::packer<msgpack::sbuffer>& pk) const override
    {
        pk.pack_map(3);
        pk.pack("type");
        pk.pack("text");

        // TODO
    }
};

#endif
