#ifndef CHAT_VIEW_H
#define CHAT_VIEW_H

#include <QAbstractItemModel>
#include <QDataWidgetMapper>
#include <QLabel>
#include <QMargins>
#include <QScrollArea>
#include <QSplitter>
#include <QStyledItemDelegate>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

#include "qtmaterialappbar.h"
#include "qtmaterialavatar.h"
#include "qtmaterialiconbutton.h"
#include "qtmaterialraisedbutton.h"
#include "qtmaterialscrollbar.h"

#include "common/message.h"

namespace MINIOICQ
{

class ChatDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    using QStyledItemDelegate::QStyledItemDelegate;

private:
    void setEditorData(QWidget* editor,
                       const QModelIndex& index) const override;
};

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

    ChatViewItem(QWidget* parent = 0) : QWidget(parent) { initUi(); }

private:
    void initUi();

private:
    // components
    QLabel* _name;
    QtMaterialAvatar* _avatar;
    QWidget* _message;
    QLabel* _time;

    friend class ChatView;
};

class ChatView : public QWidget
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
    void update();

private slots:
    // from component
    void on_sendButton_clicked();

Q_SIGNALS:
    void send(const Message& message);

private:
    // bind
    QVector<QDataWidgetMapper*> _mappers;

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
