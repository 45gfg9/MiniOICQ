#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "websocket.h"

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
        QString user_id = obj["user_id"].toString();
        QString user_name = obj["user_name"].toString();
        QString password = obj["password"].toString();
        QByteArray byteArray =
            QByteArray::fromBase64(obj["avatar"].toString().toUtf8());
        QImage image;
        image.loadFromData(byteArray);

        MINIOICQ::UserInfo info(user_id, user_name, password, image);
        if (action == "auth.login.success")
        {
            emit loginSuccess(info);
        }
        else
        {
            emit regSuccess(info);
        }
    }
    else if (action == "user.sync")
    {
        QJsonArray users = obj["users"].toArray();
        QVector<MINIOICQ::UserInfo> userInfos;
        for (const auto& user : users)
        {
            QJsonObject userObj = user.toObject();
            QString id = userObj["user_id"].toString();
            QString name = userObj["user_name"].toString();
            QByteArray avatar =
                QByteArray::fromBase64(userObj["avatar"].toString().toUtf8());
            QImage img;
            img.loadFromData(avatar);
            userInfos.push_back(MINIOICQ::UserInfo(id, name, "", img));
        }
        emit syncUser(userInfos);
    }
    else if (action == "auth.login.fail")
    {
        QString reason = obj["reason"].toString();
        qDebug() << "Login failed: " << reason;
        emit loginFailed(reason);
    }
    else if (action == "message.push")
    {
        QJsonArray messages = obj["messages"].toArray();
        QVector<MINIOICQ::Message> msgs;
        for (const auto& message : messages)
        {
            QJsonObject msgObj = message.toObject();
            QString message_id = msgObj["message_id"].toString();
            QString chat_id = msgObj["chat_id"].toString();
            QString sender_id = msgObj["sender"].toString();
            QDateTime time =
                QDateTime::fromString(msgObj["time"].toString(), Qt::ISODate);
            QString type = msgObj["type"].toString();
            QByteArray content = msgObj["content"].toString().toUtf8();
            msgs.push_back(MINIOICQ::Message(message_id, chat_id, sender_id,
                                             type, content, time));
        }

        emit newMsg(msgs);
    }
    else if (action == "chat.spawn")
    {
        QString chat_id = obj["chat_id"].toString();
        QString name = obj["name"].toString();
        QByteArray byteArray =
            QByteArray::fromBase64(obj["avatar"].toString().toUtf8());
        QImage image;
        image.loadFromData(byteArray);
        QString owner_id = obj["owner_id"].toString();
        QDateTime created =
            QDateTime::fromString(obj["created"].toString(), Qt::ISODate);
        QDateTime last_view = QDateTime::currentDateTime();
        QJsonArray members = obj["members"].toArray();
        (void)members;

        MINIOICQ::ChatInfo chat(chat_id, name, image, owner_id, created,
                                last_view);
        QVector<MINIOICQ::ChatInfo> chats;
        chats.push_back(chat);
        emit newChat(chats);
    }
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
