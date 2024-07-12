#ifndef CHAT_MODEL_H
#define CHAT_MODEL_H

#include <QSqlTableModel>

namespace MINIOICQ
{

class ChatModel : public QSqlTableModel
{
    Q_OBJECT

public:
    ChatModel(QObject* parent = nullptr, QSqlDatabase db = QSqlDatabase(), QVariant chatId);
    ~ChatModel();

private:
    QVariant _chatId;
};

} // namespace MINIOICQ

#endif // CHAT_MODEL_H