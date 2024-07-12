#include <QCryptographicHash>

#include "msgpack.hpp"
#include "websocket.h"

WebSocketConnector::WebSocketConnector(QObject* parent)
    : QObject(parent), _socket(new QWebSocket)
{
    connect(_socket, &QWebSocket::connected, this,
            &WebSocketConnector::onConnected);
    connect(_socket, &QWebSocket::disconnected, this,
            &WebSocketConnector::onDisconnected);
    connect(_socket, &QWebSocket::binaryMessageReceived, this,
            &WebSocketConnector::onMessageReceived);
}

WebSocketConnector::~WebSocketConnector()
{
    if (_socket)
    {
        _socket->deleteLater();
        _socket = nullptr;
    }
}

void WebSocketConnector::connectSocket(const QString& wsUrl)
{
    _wsUrl = QUrl(wsUrl);
    _socket->open(_wsUrl);
}

void WebSocketConnector::disconnectSocket() { _socket->close(); }

void WebSocketConnector::sendMessage(const QString& message)
{
    _socket->sendTextMessage(message);
}

QAbstractSocket::SocketState WebSocketConnector::state() const
{
    return _socket->state();
}

void WebSocketConnector::onConnected() { qDebug() << "Connected to" << _wsUrl; }

void WebSocketConnector::onDisconnected()
{
    qDebug() << "Disconnected from" << _wsUrl;
}

void WebSocketConnector::onMessageReceived(const QByteArray& message)
{
    msgpack::object_handle oh = msgpack::unpack(message.data(), message.size());
    msgpack::object obj = oh.get();

    if (obj.type != msgpack::type::MAP)
    {
        qDebug() << "Invalid message format";
        return;
    }

    auto map = obj.as<std::map<std::string, msgpack::object>>();
    auto action = map["action"].as<std::string>();

    qDebug() << "Received message: action: " << action.c_str();

    if (action == "auth.login.success")
    {
        auto user_id = QString::fromStdString(map["user_id"].as<std::string>());
        auto user_name =
            QString::fromStdString(map["user_name"].as<std::string>());
        auto password =
            QString::fromStdString(map["password"].as<std::string>());
        auto avatarData = map["avatar"].as<std::vector<uint8_t>>();
        QByteArray byteArray(reinterpret_cast<const char*>(avatarData.data()),
                             avatarData.size());
        QImage image;
        image.loadFromData(byteArray);
        emit loginSuccess(MINIOICQ::UserInfo(user_id, user_name, password, image));
    }
    else if (action == "auth.login.fail")
    {
        auto reason = QString::fromStdString(map["reason"].as<std::string>());
        qDebug() << "Login failed: " << reason;
        emit loginFail();
    }
    else
    {
        qDebug() << "Unknown action" << action.c_str();
    }
}

void WebSocketConnector::on_login(const QString& userid,
                                  const QString& password)
{
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> pk(&sbuf);

    pk.pack_map(3);
    pk.pack("action");
    pk.pack("auth.login");
    pk.pack("user_id");
    pk.pack(userid.toStdString());
    pk.pack("password");
    pk.pack(password.toStdString());

    _socket->sendBinaryMessage(QByteArray(sbuf.data(), sbuf.size()));
}

void WebSocketConnector::on_register(const QString& username,
                                     const QString& password)
{
    qDebug() << "Registering user" << username;
}

void WebSocketConnector::on_send(const AbstractMessage& msg)
{
    qDebug() << "Sending message";
}
