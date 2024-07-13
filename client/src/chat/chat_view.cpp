#include <QHBoxLayout>
#include <QVBoxLayout>

#include "chat_view.h"
#include "chat_viewmodel.h"
#include "common/misc.h"

namespace MINIOICQ
{

void ChatDelegate::setEditorData(QWidget* editor,
                                 const QModelIndex& index) const
{
    auto model = qobject_cast<const ChatViewModel*>(index.model());
    // if(index.column() != model->avatarColumn())
    // _avatar->setImage(message.avatar());
    if (index.column() != model->messageColumn())
    {
        QStyledItemDelegate::setEditorData(editor, index);
        return;
    }

    // set message widget
    QString type = index.siblingAtColumn(model->mtypeColumn()).data().toString();
    QByteArray message = index.data().toByteArray();
    if (type.startsWith("text"))
    {
        // convert
        QString content = QString::fromUtf8(message);

        // create widget
        QLabel* text = new QLabel(editor);

        // style
        // use QFontMetrics to calculate height
        // https://stackoverflow.com/questions/37671839/how-to-use-qfontmetrics-boundingrect-to-measure-size-of-multilne-message/37674690#37674690
        text->setStyleSheet("font-size: 14px;");
        auto height = QFontMetrics(text->font())
                          .boundingRect(0, 0, ChatViewItem::MessageWidth, 0,
                                        Qt::TextWordWrap, content)
                          .height();
        text->setFixedSize(ChatViewItem::MessageWidth,
                           2 * ChatViewItem::MessageVMargin + height);
        text->setWordWrap(true);
        text->setContentsMargins(ChatViewItem::MessageMargins);

        // data
        text->setText(content);

        // format
        if (type == "text/html")
        {
            text->setTextFormat(Qt::RichText);
        }
        else if (type == "text/plain")
        {
            text->setTextFormat(Qt::PlainText);
        }

        // set parent height
        editor->setFixedHeight(text->height());
    }
    else if (type.startsWith("image"))
    {
        // Qt support common image format probe:
        // bmp, gif, jpg, jpeg, png
        if (type == "image/jpeg" || type == "image/png" ||
            type == "image/bmp" || type == "image/gif")
        {
            QImage image;
            image.loadFromData(message);
            QLabel* imageLabel = new QLabel(editor);
            imageLabel->setPixmap(QPixmap::fromImage(image));
            imageLabel->setFixedSize(ChatViewItem::MessageWidth,
                                     ChatViewItem::MessageWidth);
            imageLabel->setContentsMargins(ChatViewItem::MessageMargins);
            editor->setFixedHeight(ChatViewItem::MessageWidth +
                                   2 * ChatViewItem::MessageVMargin);
        }
        else
        {
            // display unsupported message
            QLabel* text = new QLabel(editor);
            text->setText("Unsupported image format");
            text->setStyleSheet("color: red; font-size: 14px;");
            text->setFixedSize(ChatViewItem::MessageWidth,
                               2 * ChatViewItem::MessageVMargin + 20);
            text->setContentsMargins(ChatViewItem::MessageMargins);
            editor->setFixedHeight(text->height());
        }
    }
    else
    {
        // display unsupported message
        QLabel* text = new QLabel(editor);
        text->setText("Unsupported message type");
        text->setStyleSheet("color: red; font-size: 14px;");
        text->setFixedSize(ChatViewItem::MessageWidth,
                           2 * ChatViewItem::MessageVMargin + 20);
        text->setContentsMargins(ChatViewItem::MessageMargins);
        editor->setFixedHeight(text->height());
    }
    editor->parentWidget()->setFixedHeight(2 * ChatViewItem::VMargin +
                                           ChatViewItem::NameHeight +
                                           editor->height());
    // qDebug() << "this height" << this->height();
    // qDebug() << "name height" << _name->height();
    // qDebug() << "vlayout size" << vLayout->minimumSize().height();
    // qDebug() << "message height" << _message->height();
}

QMargins const ChatViewItem::MessageMargins{
    ChatViewItem::MessageHMargin, ChatViewItem::MessageVMargin,
    ChatViewItem::MessageHMargin, ChatViewItem::MessageVMargin};

void ChatViewItem::initUi()
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

    // set parent height
    this->setFixedHeight(2 * VMargin + NameHeight + _message->height());
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
    // _input->setText(
    //     "<p>The distinction between the subjects of syntax and semantics "
    //     "has its origin in the study of natural languages.</p><p>The "
    //     "distinction between the subjects of syntax and semantics has its "
    //     "origin in the study of natural languages.</p><p>The distinction "
    //     "between the subjects of syntax and semantics has its origin in "
    //     "the study of natural languages.</p><p>The distinction between the "
    //     "subjects of syntax and semantics has its origin in the study of "
    //     "natural languages.</p><p>The distinction between the subjects of "
    //     "syntax and semantics has its origin in the study of natural "
    //     "languages.</p><p>The distinction between the subjects of syntax "
    //     "and semantics has its origin in the study of natural "
    //     "languages.</p><p>The distinction between the subjects of syntax "
    //     "and semantics has its origin in the study of natural "
    //     "languages.</p><p>The distinction between the subjects of syntax "
    //     "and semantics has its origin in the study of natural "
    //     "languages.</p>");
    // for (int i = 0; i < 10; i++)
    // {
    //     MINIOICQ::Message message(
    //         "-1", "-1", "me", "text/plain",
    //         QByteArray("a very very very very long long long message a "
    //                    "very very very very long long long message"),
    //         QDateTime::currentDateTime());
    //     message.setAvatar(QImage(":/testImage.jpg"));
    //     auto item = new ChatViewItem(message, _chatList);
    //     chatListLayout->addWidget(item);
    // }
    // set parent height
    _chatList->setFixedHeight(_chatList->layout()->minimumSize().height());
}

void ChatView::initConnect() {
    // from component
    connect(_sendButton, &QtMaterialRaisedButton::clicked, this,
            &ChatView::on_sendButton_clicked);
    connect(_closeButton, &QtMaterialRaisedButton::clicked, this,
            &ChatView::on_closeButton_clicked);
}

void ChatView::on_dataChanged()
{
    qDebug() << "ChatView::update";
    if (_mappers.empty())
    {
        qDebug() << "ChatView::update: mappers is empty";
        return;
    }
    auto* model = qobject_cast<ChatViewModel*>(_mappers[0]->model());

    // clear mapping
    for (auto* mapper : _mappers)
    {
        // TODO: delegate need to be deleted?
        mapper->clearMapping();
    }

    // adjust item count
    auto* layout = _chatList->layout();
    auto modelItemNum = model->rowCount();
    auto layoutItemNum = layout->count();
    if (modelItemNum > layoutItemNum)
    {
        for (int i = layoutItemNum; i < modelItemNum; i++)
        {
            // widget
            auto* item = new ChatViewItem(_chatList);
            layout->addWidget(item);
            // mapper
            auto* mapper = new QDataWidgetMapper(this);
            _mappers.push_back(mapper);
        }
    }
    else if (modelItemNum < layoutItemNum)
    {
        for (int i = layoutItemNum; i > modelItemNum; i--)
        {
            // widget
            auto* item = layout->itemAt(i - 1)->widget();
            layout->removeWidget(item);
            delete item;
            // mapper
            auto* mapper = _mappers.back();
            _mappers.pop_back();
            delete mapper;
        }
    }

    // refresh item
    for (int i = 0; i < modelItemNum; i++)
    {
        auto* item = qobject_cast<ChatViewItem*>(layout->itemAt(i)->widget());
        auto* mapper = new QDataWidgetMapper(this);
        mapper->setModel(model);
        mapper->setItemDelegate(new ChatDelegate(this));
        mapper->addMapping(item->_name, model->nameColumn(), "text");
        mapper->addMapping(item->_avatar, model->avatarColumn(), "image");
        mapper->addMapping(item->_message, model->messageColumn());
        mapper->addMapping(item->_time, model->sendTimeColumn(), "text");
        mapper->setCurrentIndex(i);
        _mappers.push_back(mapper);
    }
}

void ChatView::setModel(QAbstractItemModel* model)
{
    if (!_mappers.empty())
    {
        qDebug() << "ChatView::setModel: mappers is not empty";
        return;
    }

    // convert
    auto chatViewModel = qobject_cast<ChatViewModel*>(model);

    // connect
    connect(chatViewModel, &ChatViewModel::dataChanged, this, &ChatView::on_dataChanged);
    connect(this, &ChatView::send, chatViewModel, &ChatViewModel::on_send);

    // bind
    auto initMapper = new QDataWidgetMapper(this);
    initMapper->setModel(model);
    _mappers.push_back(initMapper);
    update();
}

void ChatView::on_sendButton_clicked() {
    qDebug() << "ChatView::on_sendButton_clicked";
    Message message(QString(), QString(), QString(), "text/plain",
                    _input->toPlainText().toUtf8(), QDateTime::currentDateTime());
    emit send(message);
}

void ChatView::on_closeButton_clicked() {
    qDebug() << "ChatView::on_closeButton_clicked";
    emit closeChat();
}

void bindChatView(ChatView* view, QAbstractItemModel* model) {
    view->setModel(model);
}

} // namespace MINIOICQ
