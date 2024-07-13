#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

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
        QByteArray byteArray = QByteArray::fromBase64(obj["avatar"].toString().toUtf8());
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
    else if (action == "auth.login.fail")
    {
        QString reason = obj["reason"].toString();
        qDebug() << "Login failed: " << reason;
        emit loginFailed(reason);
    }
    else
    {
        qDebug() << "Unknown action" << action;
    }
}

void WebSocketConnector::on_login(const QString& userid,
                                  const QString& password)
{
    if (!_isConnected)
    {
        emit loginFailed("Not connected to server");
        return;
    }

    QJsonObject obj;
    obj["action"] = "auth.login";
    obj["user_id"] = userid;
    obj["password"] = password;
    QJsonDocument doc(obj);
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

    QJsonObject obj;
    obj["action"] = "auth.register";
    obj["user_name"] = username;
    obj["password"] = password;
    QJsonDocument doc(obj);
    _socket->sendTextMessage(doc.toJson());
}

void WebSocketConnector::on_send(const MINIOICQ::AbstractMessage& msg)
{
    qDebug() << "Sending message";
}
