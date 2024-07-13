#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <QImage>
#include <QWebSocket>

#include "common/message.h"
#include "common/chat_info.h"
#include "common/user_info.h"

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
    void onMessageReceived(const QString& message);

    // from LoginViewModel
    void on_login(const QString& userid, const QString& password);
    void on_reg(const QString& username, const QString& password);

    void on_view();
    void on_send(const MINIOICQ::Message& msg);

signals:
    void messageReceived(const QString& message);

    // to LoginViewModel
    void loginSuccess(const MINIOICQ::UserInfo& info);
    void loginFailed(const QString& reason);
    void regSuccess(const MINIOICQ::UserInfo& info);
    void regFailed(const QString& reason);

    // to ListViewModel
    void newMsg(QVector<MINIOICQ::Message>& messages);
    void newChat(QVector<MINIOICQ::ChatInfo>& chat);
    void newUser(QVector<MINIOICQ::UserInfo>& users);

private:
    QWebSocket* _socket;
    bool _isConnected = false;
    QUrl _wsUrl;
};

#endif // WEBSOCKET_H
