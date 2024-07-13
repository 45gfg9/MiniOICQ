#ifndef CHAT_VIEW_H
#define CHAT_VIEW_H

#include <QAbstractItemModel>
#include <QLabel>
#include <QMargins>
#include <QWidget>
#include <QSplitter>
#include <QTextEdit>
#include <QScrollArea>
#include <QVBoxLayout>

#include "qtmaterialavatar.h"
#include "qtmaterialappbar.h"
#include "qtmaterialscrollbar.h"
#include "qtmaterialraisedbutton.h"
#include "qtmaterialiconbutton.h"

#include "common/message.h"

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

class ChatView: public QWidget
{
    Q_OBJECT

public:
    ChatView(QWidget* parent = 0);

    static int constexpr Width = ChatViewItem::Width;
    static int constexpr Height = 540;
    static int constexpr ButtonWidth = 78;
    static int constexpr ButtonHeight = 26;
    static int constexpr ButtonGap = 8;
    static int constexpr ButtonHMargin = 20;
    static int constexpr ButtonVMargin = 8;
    static int constexpr AppBarHeight = 44;
    static int constexpr InputHeight = 150;
    static int constexpr ListHeight = Height - AppBarHeight - InputHeight;

private:
    void initUi();
    void initConnect();
    void setModel(QAbstractItemModel* model);

    // event

public slots:

private slots:
    // from component
    void on_sendButton_clicked();

Q_SIGNALS:
    void send(const AbstractMessage& message);

private:
    // components
    QSplitter* _splitter;
    QtMaterialAppBar* _appBar;
    QScrollArea* _chatArea;
    QWidget* _chatList;
    QTextEdit* _input;
    QtMaterialRaisedButton* _sendButton;
    QtMaterialRaisedButton* _closeButton;

    friend void bindChatView(ChatView* view, QAbstractItemModel* model);
};

void bindChatView(ChatView* view, QAbstractItemModel* model);

} // namespace MINIOICQ

#endif // CHAT_VIEW_H
