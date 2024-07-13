#ifndef CHAT_VIEW_H
#define CHAT_VIEW_H

#include "common/message.h"
#include "qtmaterialavatar.h"
#include <QAbstractItemModel>
#include <QLabel>
#include <QMargins>
#include <QWidget>

namespace MINIOICQ
{

class ChatViewItem : public QWidget
{
    Q_OBJECT

public:
    static int constexpr Width = 580;
    static int constexpr HMargin = 20;
    static int constexpr VMargin = 8;
    static int constexpr AvatarSize = 32;
    static int constexpr NameHeight = 18;
    static int constexpr MessagePadding = 40;
    static int constexpr MessageGap = 8;
    static int constexpr MessageWidth =
        Width - HMargin * 2 - AvatarSize - MessagePadding;
    static int constexpr MessageHMargin = 8;
    static int constexpr MessageVMargin = 10;

    static QMargins const MessageMargins;

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
