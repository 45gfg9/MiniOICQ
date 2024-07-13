#include <QHBoxLayout>
#include <QVBoxLayout>

#include "chat_view.h"
#include "common/misc.h"

namespace MINIOICQ
{

ChatViewItem::ChatViewItem(AbstractMessage& message, QWidget* parent)
    : QWidget(parent)
{
    initUi(message);
}

void ChatViewItem::initUi(AbstractMessage& message)
{
    _name = new QLabel(this);
    _avatar = new QtMaterialAvatar(this);
    _message = new QWidget(this);
    _time = new QLabel(this);

    // arrange
    auto layout = new QHBoxLayout(this);
    auto vLayout = new QVBoxLayout;
    vLayout->addWidget(_name);
    vLayout->addWidget(_message);
    layout->addWidget(_avatar);
    layout->addLayout(vLayout);
    _time->setGeometry(Width - HMargin - MessagePadding, VMargin, 0, 0);

    // style
    // item
    //this->setPalette(themePalette);
    this->setContentsMargins(zeroMargins);
    this->setFixedWidth(Width);
    // layout
    layout->setContentsMargins(HMargin, VMargin, HMargin, VMargin);
    layout->setSpacing(0);
    // avatar
    _avatar->setSize(AvatarSize);
    _avatar->setIcon(QIcon(":/close.svg"));
    layout->setAlignment(_avatar, Qt::AlignTop);
    // vLayout
    vLayout->setContentsMargins(zeroMargins);
    vLayout->setSpacing(0);
    // name
    _name->setStyleSheet(themeStyleSheetDeep + "font-size: 12px;");
    _name->setFixedSize(MessageWidth, 0);
    _name->setContentsMargins(0, 0, MessagePadding, 0);
    // message
    _message->setStyleSheet(themeStyleSheetLight);
    // time
    _time->setStyleSheet("font-size: 10px; color: gray;");
    _time->setText(message.time().toString("hh:mm"));

    // fill data
    _name->setText(message.sender());
    qDebug() << _name->text();
    // init message
    switch (message.type())
    {
        case MessageType::Text:
            initText(dynamic_cast<TextMessage&>(message));
            break;
        case MessageType::Image:
            initImage(dynamic_cast<ImageMessage&>(message));
            break;
        case MessageType::File:
            initFile(dynamic_cast<FileMessage&>(message));
            break;
        case MessageType::Audio:
            initVoice(dynamic_cast<AudioMessage&>(message));
            break;
        case MessageType::Video:
            initVideo(dynamic_cast<VideoMessage&>(message));
            break;
        default:
            break;
    }
}

void ChatViewItem::initText(TextMessage& message)
{
    QLabel* text = new QLabel(_message);
    text->setText(message.text());
    text->setWordWrap(true);
    text->setStyleSheet("font-size: 14px;");
    // text->setFixedSize(MessageWidth - 2 * MessageHMargin, 0);
    text->setContentsMargins(MessageHMargin, MessageVMargin, MessageHMargin,
                             MessageVMargin);
}

void ChatViewItem::initImage(ImageMessage& message) {}

void ChatViewItem::initFile(FileMessage& message) {}

void ChatViewItem::initVoice(AudioMessage& message) {}

void ChatViewItem::initVideo(VideoMessage& message) {}

void bindChatView(ChatView* view, QAbstractItemModel* model) {}

} // namespace MINIOICQ
