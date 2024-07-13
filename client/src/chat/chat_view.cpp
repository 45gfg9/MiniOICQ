#include <QHBoxLayout>
#include <QVBoxLayout>

#include "chat_view.h"
#include "common/misc.h"

namespace MINIOICQ
{

QMargins const ChatViewItem::MessageMargins{
    ChatViewItem::MessageHMargin, ChatViewItem::MessageVMargin,
    ChatViewItem::MessageHMargin, ChatViewItem::MessageVMargin};

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
    // vLayout->addStretch();
    layout->addWidget(_avatar);
    layout->addLayout(vLayout);
    _time->setGeometry(Width - HMargin - MessagePadding, VMargin, 0, 0);

    // style
    // item
    this->setPalette(themePalette);
    // this->setContentsMargins(zeroMargins);
    this->setFixedWidth(Width);
    // layout
    layout->setContentsMargins(HMargin, VMargin, HMargin, VMargin);
    layout->setSpacing(0);
    // layout->setAlignment(Qt::AlignLeft);
    //  avatar
    _avatar->setImage(message.avatar());
    _avatar->setSize(AvatarSize);
    _avatar->setContentsMargins(zeroMargins);
    layout->setAlignment(_avatar, Qt::AlignTop);
    // vLayout
    vLayout->setContentsMargins(MessageGap, 0, MessagePadding, 0);
    vLayout->setSpacing(0);
    // name
    _name->setStyleSheet(themeStyleSheetLight + "font-size: 12px;");
    _name->setFixedSize(MessageWidth + MessagePadding, NameHeight);
    _name->setContentsMargins(zeroMargins);
    // message
    _message->setStyleSheet(themeStyleSheetDeep +
                            "border-radius: 8px; font-size: 14px;");
    _message->setFixedWidth(MessageWidth);
    _message->setContentsMargins(zeroMargins);
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
    this->setFixedHeight(2 * VMargin + NameHeight + _message->height());
    // qDebug() << "this height" << this->height();
    // qDebug() << "name height" << _name->height();
    // qDebug() << "vlayout size" << vLayout->minimumSize().height();
    // qDebug() << "message height" << _message->height();
}

void ChatViewItem::initText(TextMessage& message)
{
    QLabel* text = new QLabel(_message);

    // style
    // use QFontMetrics to calculate height
    // https://stackoverflow.com/questions/37671839/how-to-use-qfontmetrics-boundingrect-to-measure-size-of-multilne-message/37674690#37674690
    text->setStyleSheet("font-size: 14px;");
    auto height = QFontMetrics(text->font())
                      .boundingRect(0, 0, MessageWidth, 0, Qt::TextWordWrap,
                                    message.text())
                      .height();
    text->setFixedSize(MessageWidth, 2 * MessageVMargin + height);
    text->setWordWrap(true);
    text->setContentsMargins(MessageMargins);

    // data
    text->setText(message.text());

    // set parent height
    _message->setFixedHeight(text->height());
}

void ChatViewItem::initImage(ImageMessage& message) {}

void ChatViewItem::initFile(FileMessage& message) {}

void ChatViewItem::initVoice(AudioMessage& message) {}

void ChatViewItem::initVideo(VideoMessage& message) {}

void bindChatView(ChatView* view, QAbstractItemModel* model) {}

} // namespace MINIOICQ
