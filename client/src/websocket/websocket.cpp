#include <QThread>
#include <QWebSocket>

#include "msgpack.hpp"
#include "websocket.h"

WebSocketThread::WebSocketThread(QObject* parent)
    : QThread(parent), _socket(new QWebSocket)
{
    connect(_socket, &QWebSocket::connected, this,
            &WebSocketThread::onConnected);
    connect(_socket, &QWebSocket::disconnected, this,
            &WebSocketThread::onDisconnected);
    connect(_socket, &QWebSocket::binaryMessageReceived, this,
            &WebSocketThread::onMessageReceived);
}

WebSocketThread::~WebSocketThread()
{
    if (_socket)
    {
        _socket->deleteLater();
        _socket = nullptr;
    }
}

void WebSocketThread::connectSocket(const QString& wsUrl)
{
    _wsUrl = QUrl(wsUrl);
    _socket->open(_wsUrl);
}

void WebSocketThread::disconnectSocket() { _socket->close(); }

void WebSocketThread::sendMessage(const QString& message)
{
    _socket->sendTextMessage(message);
}

QAbstractSocket::SocketState WebSocketThread::state() const
{
    return _socket->state();
}

void WebSocketThread::run() { exec(); }

void WebSocketThread::onConnected() { qDebug() << "Connected to" << _wsUrl; }

void WebSocketThread::onDisconnected()
{
    qDebug() << "Disconnected from" << _wsUrl;
}

void WebSocketThread::onMessageReceived(const QByteArray& message)
{
    qDebug() << "Received message from [" << _wsUrl
             << "]: " << message.length();
    emit messageReceived(message);
}

void WebSocketThread::on_login(const QString& userId, const QString& password)
{
    // somewhere else connect to this slot
    // connect(loginProxyModel, &LoginProxyModel::login, this,
    // &WebSocketThread::login); Send login message if success
    onMessageReceived("loginSuccess");
    // emit loginSuccess(userId, userName, password, _avatar);
    // else
    // emit loginFailed();
}
