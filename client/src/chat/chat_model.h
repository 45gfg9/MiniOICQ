#ifndef CHAT_MODEL_H
#define CHAT_MODEL_H

#include <QSqlTableModel>

namespace MINIOICQ
{

class ChatModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    ChatModel(QSqlDatabase db, QVariant chatId, QObject* parent = nullptr);
    ~ChatModel();

    QVariant chatId() const { return _chatId; }

Q_SIGNALS:
    void message_received();

public slots:
    void on_message_received();

private:
    void refresh();

    QVariant _chatId;
    QSqlDatabase _db;
    QString _viewName;
};

} // namespace MINIOICQ

#endif // CHAT_MODEL_H
