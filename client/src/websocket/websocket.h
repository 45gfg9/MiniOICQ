#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <QThread>
#include <QWebSocket>

class WebSocketThread : public QThread {
    Q_OBJECT

public:
    explicit WebSocketThread(QObject *parent = nullptr);
    ~WebSocketThread();

    void connectSocket(const QString &wsUrl);
    void disconnectSocket();
    void sendMessage(const QString &message);
    QAbstractSocket::SocketState state() const;
    void run() override;

public slots:
    void onConnected();
    void onDisconnected();
    void onMessageReceived(const QByteArray &message);

    // LoginProxyModel
    void on_login(const QString &_username, const QString &password);

    void on_send(/* type todo */);
 
signals:
    void messageReceived(const QByteArray &message);

    void loginSuccess();
    void loginFail();

private:
    QWebSocket *_socket;
    QUrl _wsUrl;
};

#endif // WEBSOCKET_H