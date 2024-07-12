#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <QWebSocket>
#include <QImage>

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
    void on_register(const QString& username, const QString& password);
    void on_send(const AbstractMessage &msg);

signals:
    void messageReceived(const QByteArray& message);

    void loginSuccess(QString userId, QString userName, QString password,
                      QImage avatar);
    void loginFail();

    void registerSuccess(QString userId, QImage avatar);

private:
    QWebSocket* _socket;
    QUrl _wsUrl;
};

#endif // WEBSOCKET_H
