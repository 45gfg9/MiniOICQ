#ifndef CHAT_VIEW_H
#define CHAT_VIEW_H

#include "common/message.h"
#include "qtmaterialavatar.h"
#include <QAbstractItemModel>
#include <QLabel>
#include <QWidget>

namespace MINIOICQ
{

class ChatViewItem : public QWidget
{
    Q_OBJECT

public:
    ChatViewItem(AbstractMessage& message, QWidget* parent = 0);

private:
    void initUi(AbstractMessage& message);
    void initText(TextMessage& message);
    void initImage(ImageMessage& message);
    void initFile(FileMessage& message);
    void initVoice(AudioMessage& message);
    void initVideo(VideoMessage& message);


private:
    // components
    QLabel* _name;
    QtMaterialAvatar* _avatar;
    QWidget* _message;
    QLabel* _time;
};

class ChatView
{
};

void bindChatView(ChatView* view, QAbstractItemModel* model);

} // namespace MINIOICQ

#endif // CHAT_VIEW_H
