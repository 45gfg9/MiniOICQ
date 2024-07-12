#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <QImage>
#include <QWebSocket>

#include "common/user_info.h"
#include "message/message.h"

class WebSocketConnector : public QObject
{
    Q_OBJECT

public:
    explicit WebSocketConnector(QObject* parent = nullptr);
    ~WebSocketConnector();

    void connectSocket(const QString& wsUrl);
    void disconnectSocket();
    void sendMessage(const QString& message);
    QAbstractSocket::SocketState state() const;

public slots:
    void onConnected();
    void onDisconnected();
    void onMessageReceived(const QByteArray& message);

    // LoginViewModel
    void on_login(const QString& userid, const QString& password);
    void on_reg(const QString& username, const QString& password);
    void on_send(const AbstractMessage& msg);

signals:
    void messageReceived(const QByteArray& message);

    void loginSuccess(const MINIOICQ::UserInfo& info);
    void loginFailed(const QString& reason);

    void regSuccess(const MINIOICQ::UserInfo& info);

private:
    QWebSocket* _socket;
    QUrl _wsUrl;
};

#endif // WEBSOCKET_H
