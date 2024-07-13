#include <QHBoxLayout>
#include <QVBoxLayout>

#include "chat_view.h"
#include "common/misc.h"

namespace MINIOICQ
{

QMargins const ChatViewItem::MessageMargins{
    ChatViewItem::MessageHMargin, ChatViewItem::MessageVMargin,
    ChatViewItem::MessageHMargin, ChatViewItem::MessageVMargin};

ChatViewItem::ChatViewItem(Message& message, QWidget* parent) : QWidget(parent)
{
    initUi(message);
}

void ChatViewItem::initUi(Message& message)
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
    if (message.type().startsWith("text"))
    {
        QLabel* text = new QLabel(_message);

        // style
        // use QFontMetrics to calculate height
        // https://stackoverflow.com/questions/37671839/how-to-use-qfontmetrics-boundingrect-to-measure-size-of-multilne-message/37674690#37674690
        text->setStyleSheet("font-size: 14px;");
        auto height = QFontMetrics(text->font())
                          .boundingRect(0, 0, MessageWidth, 0, Qt::TextWordWrap,
                                        message.content())
                          .height();
        text->setFixedSize(MessageWidth, 2 * MessageVMargin + height);
        text->setWordWrap(true);
        text->setContentsMargins(MessageMargins);

        // data
        text->setText(message.content());

        // set format
        if (message.type() == "text/html")
        {
            text->setTextFormat(Qt::RichText);
        }
        else if (message.type() == "text/plain")
        {
            text->setTextFormat(Qt::PlainText);
        }

        // set parent height
        _message->setFixedHeight(text->height());
    }
    else if (message.type().startsWith("image"))
    {
        if (message.type() == "image/jpeg" || message.type() == "image/png")
        {
            QImage image;
            image.loadFromData(message.content());
            QLabel* imageLabel = new QLabel(_message);
            imageLabel->setPixmap(QPixmap::fromImage(image));
            imageLabel->setFixedSize(MessageWidth, MessageWidth);
            imageLabel->setContentsMargins(MessageMargins);
            _message->setFixedHeight(MessageWidth + 2 * MessageVMargin);
        }
    }
    else
    {
        // display unsupported message
        QLabel* text = new QLabel(_message);
        text->setText("Unsupported message type");
        text->setStyleSheet("color: red; font-size: 14px;");
        text->setFixedSize(MessageWidth, 2 * MessageVMargin + 20);
        text->setContentsMargins(MessageMargins);
        _message->setFixedHeight(2 * MessageVMargin + 20);
    }
    this->setFixedHeight(2 * VMargin + NameHeight + _message->height());
    // qDebug() << "this height" << this->height();
    // qDebug() << "name height" << _name->height();
    // qDebug() << "vlayout size" << vLayout->minimumSize().height();
    // qDebug() << "message height" << _message->height();
}

ChatView::ChatView(QWidget* parent) : QWidget(parent)
{
    initUi();
    initConnect();
}

void ChatView::initUi()
{
    _splitter = new QSplitter(this);
    _appBar = new QtMaterialAppBar(_splitter);
    _chatArea = new QScrollArea(_splitter);
    _chatList = new QWidget(_chatArea);
    _input = new QTextEdit(_splitter);
    _sendButton = new QtMaterialRaisedButton(this);
    _closeButton = new QtMaterialRaisedButton(this);

    // arrange
    auto chatListLayout = new QVBoxLayout(_chatList);
    _chatArea->setWidget(_chatList);
    _splitter->addWidget(_appBar);
    _splitter->addWidget(_chatArea);
    _splitter->addWidget(_input);

    // style
    this->setWindowFlag(Qt::Window);
    this->setFixedSize(Width, Height);
    this->setPalette(themePalette);
    _splitter->setOrientation(Qt::Vertical);
    _splitter->setFixedSize(Width, Height);
    // appbar
    QLabel* title = new QLabel("ChatName", _appBar);
    title->setStyleSheet("color: white; font-size: 16px;");
    QtMaterialIconButton* button =
        new QtMaterialIconButton(QIcon(":/chat.svg"), this);
    button->setIconSize(QSize(24, 24));
    button->setColor(Qt::white);
    button->setFixedWidth(42);
    _appBar->setFixedSize(Width, AppBarHeight);
    _appBar->appBarLayout()->addWidget(button);
    _appBar->appBarLayout()->addWidget(title);
    _appBar->appBarLayout()->addStretch(1);
    // chatArea
    // _chatArea->setWidgetResizable(false);
    _chatArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _chatArea->setVerticalScrollBar(new QtMaterialScrollBar);
    // chatList
    _chatList->setFixedWidth(Width);
    // chatListLayout
    chatListLayout->setContentsMargins(zeroMargins);
    // textEdit
    _input->setStyleSheet("font-size: 14px;");
    _input->setFixedWidth(Width);
    _input->setMinimumHeight(InputHeight);
    _input->setVerticalScrollBar(new QtMaterialScrollBar);
    // button
    _sendButton->setFixedSize(ButtonWidth, ButtonHeight);
    auto buttonX =
        ChatViewItem::Width - ButtonHMargin - 2 * ButtonWidth - ButtonGap;
    auto buttonY = Height - ButtonVMargin - ButtonHeight;
    _sendButton->setGeometry(buttonX + ButtonWidth + ButtonGap, buttonY,
                             ButtonWidth, ButtonHeight);
    _sendButton->setStyleSheet("font-size: 14px;");
    _sendButton->setText("Send");
    _closeButton->setFixedSize(ButtonWidth, ButtonHeight);
    _closeButton->setGeometry(buttonX, buttonY, ButtonWidth, ButtonHeight);
    _closeButton->setText("Close");

    // fill default data
    _input->setText(
        "<p>The distinction between the subjects of syntax and semantics "
        "has its origin in the study of natural languages.</p><p>The "
        "distinction between the subjects of syntax and semantics has its "
        "origin in the study of natural languages.</p><p>The distinction "
        "between the subjects of syntax and semantics has its origin in "
        "the study of natural languages.</p><p>The distinction between the "
        "subjects of syntax and semantics has its origin in the study of "
        "natural languages.</p><p>The distinction between the subjects of "
        "syntax and semantics has its origin in the study of natural "
        "languages.</p><p>The distinction between the subjects of syntax "
        "and semantics has its origin in the study of natural "
        "languages.</p><p>The distinction between the subjects of syntax "
        "and semantics has its origin in the study of natural "
        "languages.</p><p>The distinction between the subjects of syntax "
        "and semantics has its origin in the study of natural "
        "languages.</p>");
    for (int i = 0; i < 10; i++)
    {

        MINIOICQ::Message message(
            "-1", "-1", "me", "text/plain",
            QByteArray("a very very very very long long long message a "
                       "very very very very long long long message"),
            QDateTime::currentDateTime());
        message.setAvatar(QImage(":/testImage.jpg"));
        auto item = new ChatViewItem(message, _chatList);
        chatListLayout->addWidget(item);
    }
    // set parent height
    _chatList->setFixedHeight(_chatList->layout()->minimumSize().height());
}

void ChatView::initConnect() {}

void ChatView::setModel(QAbstractItemModel* model) {}

void ChatView::on_sendButton_clicked() {}

void bindChatView(ChatView* view, QAbstractItemModel* model) {}

} // namespace MINIOICQ
