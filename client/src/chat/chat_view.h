#ifndef CHAT_VIEW_H
#define CHAT_VIEW_H

#include <QAbstractItemModel>

namespace MINIOICQ
{

class ChatView {};

void bindChatView(ChatView* view, QAbstractItemModel* model);

} // namespace MINIOICQ

#endif // CHAT_VIEW_H
