#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "websocket.h"

static MINIOICQ::ChatInfo parseChatInfo(const QJsonObject& obj)
{
    // qDebug() << obj;
    QString chat_id = QString::number(obj["chat_id"].toInt());
    QString name = obj["chat_name"].toString();
    QByteArray byteArray =
        QByteArray::fromBase64(obj["avatar"].toString().toUtf8());
    QImage image;
    image.loadFromData(byteArray);
    QString owner_id = QString::number(obj["owner_id"].toInt());
    QDateTime created =
        QDateTime::fromString(obj["created"].toString(), Qt::ISODate);
    QDateTime last_view = QDateTime::currentDateTime();

    QJsonArray members = obj["members"].toArray();
    QStringList memberList;
    for (const auto& member : members)
    {
        QJsonObject memberObj = member.toObject();
        QString member_id = QString::number(memberObj["user_id"].toInt());
        memberList.push_back(member_id);
    }

    return MINIOICQ::ChatInfo(chat_id, name, image, owner_id, created,
                              last_view, memberList);
}

static MINIOICQ::Message parseMessage(const QJsonObject& obj)
{
    QString message_id = QString::number(obj["message_id"].toInt());
    QString chat_id = QString::number(obj["chat_id"].toInt());
    QString sender_id = QString::number(obj["sender_id"].toInt());
    QDateTime time = QDateTime::fromString(obj["time"].toString(), Qt::ISODate);
    QString type = obj["type"].toString();
    QByteArray content =
        QByteArray::fromBase64(obj["content"].toString().toUtf8());
    return MINIOICQ::Message(message_id, chat_id, sender_id, type, content,
                             time);
}

static MINIOICQ::UserInfo parseUserInfo(const QJsonObject& obj)
{
    QString id = QString::number(obj["user_id"].toInt());
    QString name = obj["user_name"].toString();
    QByteArray avatar =
        QByteArray::fromBase64(obj["avatar"].toString().toUtf8());
    QImage img;
    img.loadFromData(avatar);
    return MINIOICQ::UserInfo(id, name, "", img);
}

WebSocketConnector::WebSocketConnector(QObject* parent)
    : QObject(parent), _socket(new QWebSocket)
{
    connect(_socket, &QWebSocket::connected, this,
            &WebSocketConnector::onConnected);
    connect(_socket, &QWebSocket::disconnected, this,
            &WebSocketConnector::onDisconnected);
    connect(_socket, &QWebSocket::textMessageReceived, this,
            &WebSocketConnector::onMessageReceived);
}

WebSocketConnector::~WebSocketConnector()
{
    if (_socket)
    {
        disconnectSocket();
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

void WebSocketConnector::onConnected()
{
    qDebug() << "Connected to" << _wsUrl;
    _isConnected = true;
}

void WebSocketConnector::onDisconnected()
{
    qDebug() << "Disconnected from" << _wsUrl;
    _isConnected = false;
}

void WebSocketConnector::onMessageReceived(const QString& message)
{
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject obj = doc.object();
    QString action = obj["action"].toString();

    qDebug() << "Received message: action: " << action;

    if (action == "auth.login.success" || action == "auth.register.success")
    {
        MINIOICQ::UserInfo info = parseUserInfo(obj);
        if (action == "auth.login.success")
            emit loginSuccess(info);
        else
            emit regSuccess(info);
    }
    else if (action == "user.sync")
    {
        QVector<MINIOICQ::UserInfo> userInfos;
        for (const auto& user : obj["users"].toArray())
        {
            userInfos.push_back(parseUserInfo(user.toObject()));
        }
        emit syncUser(userInfos);

        QVector<MINIOICQ::ChatInfo> chatInfos;
        for (const auto& chat : obj["chats"].toArray())
        {
            chatInfos.push_back(parseChatInfo(chat.toObject()));
        }
        emit syncChat(chatInfos);

        QVector<MINIOICQ::Message> messages;
        for (const auto& message : obj["messages"].toArray())
        {
            messages.push_back(parseMessage(message.toObject()));
        }
        emit syncMsg(messages);
    }
    else if (action == "auth.login.fail" || action == "auth.register.fail")
    {
        QString reason = obj["reason"].toString();
        if (action == "auth.login.fail")
            emit loginFailed(reason);
        else
            emit regFailed(reason);
    }
    else if (action == "message.push")
    {
        QJsonArray messages = obj["messages"].toArray();
        QVector<MINIOICQ::Message> msgs;
        for (const auto& message : messages)
        {
            msgs.push_back(parseMessage(message.toObject()));
        }

        emit syncMsg(msgs);
    }
    else if (action == "chat.spawn")
    {
        QVector<MINIOICQ::ChatInfo> chats;
        chats.push_back(parseChatInfo(obj));
        emit syncChat(chats);
    }
    else
    {
        qDebug() << "Unknown action" << action;
    }
}

void WebSocketConnector::on_login(const QString& userid,
                                  const QString& password)
{
    qDebug() << "Login message";
    if (!_isConnected)
    {
        emit loginFailed("Not connected to server");
        return;
    }

    QJsonDocument doc({
        {"action", "auth.login"},
        {"user_id", userid},
        {"password", password},
    });
    _socket->sendTextMessage(doc.toJson());
}

void WebSocketConnector::on_reg(const QString& username,
                                const QString& password)
{
    if (!_isConnected)
    {
        emit regFailed("Not connected to server");
        return;
    }

    QJsonDocument doc({
        {"action", "auth.register"},
        {"user_name", username},
        {"password", password},
    });
    _socket->sendTextMessage(doc.toJson());
}

void WebSocketConnector::on_view()
{
    qDebug() << "View message";
    // TODO
}

void WebSocketConnector::on_send(const MINIOICQ::Message& msg)
{
    qDebug() << "WebSocketConnector::on_send";

    if (!_isConnected)
    {
        qDebug() << "Not connected to server";
        return;
    }

    QJsonDocument doc({
        {"action", "message.send"},
        {"message", msg.pack()},
    });
    _socket->sendTextMessage(doc.toJson());
}

void WebSocketConnector::on_sync()
{
    if (!_isConnected)
    {
        emit regFailed("Not connected to server");
        return;
    }

    QJsonDocument doc(QJsonObject({{"action", "user.sync"}}));
    _socket->sendTextMessage(doc.toJson());
}
