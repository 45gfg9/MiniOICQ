#include "list_view.h"
#include <QDate>
#include <QHBoxLayout>
#include <QTime>
#include <QVBoxLayout>

namespace MINIOICQ
{

ListViewItem::ListViewItem(QWidget* parent) : QWidget(parent) { initUi(); }

ListViewItem::~ListViewItem() {}

void ListViewItem::initUi()
{
    _avatar = new QtMaterialAvatar(this);
    _chatName = new QLabel(this);
    _lastTime = new QLabel(this);
    _lastMsg = new QLabel(this);
    _unRead = new QLabel(this);

    // arrange
    auto layout = new QHBoxLayout(this);
    auto vLayout = new QVBoxLayout(this);
    auto hLayout = new QHBoxLayout(this);

    hLayout->addWidget(_chatName);
    hLayout->addWidget(_lastTime);

    vLayout->addLayout(hLayout);
    vLayout->addWidget(_lastMsg);

    layout->addWidget(_avatar);
    layout->addLayout(vLayout);
    this->setLayout(layout);

    // style
    // item: 252 * 72
    this->setFixedSize(252, 72);
    // layout: border 16, space 8
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(8);
    // avatar: 40 * 40
    _avatar->setSize(40);

    // vLayout: 172 * 40, space 4
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(4);
    // chatName: 172 * 20, font 16
    _chatName->setStyleSheet("font-size: 16px;");
    _chatName->setFixedSize(172, 20);
    // lastTime: font 10
    _lastTime->setStyleSheet("font-size: 10px; color: gray;");
    // lastMsg: 172 * 16, font 12
    _lastMsg->setStyleSheet("font-size: 12px; color:gray;");
    _lastMsg->setFixedSize(172, 16);

    // unread: 16 * 16, font 12
    _unRead->setStyleSheet("color: white; border-radius: 8px; "
                           "background-color: red; font-size: 12px;");
    _unRead->setGeometry(220, 40, 16, 16);

    _unRead->setAlignment(Qt::AlignCenter);
    // vLayout->setContentsMargins(0, 0, 0, 0);

    // fill default data
    _chatName->setText("chatName");
    _lastTime->setText(QDateTime::currentDateTime().toString("hh:mm"));
    _lastMsg->setText("lastMsg");
    setUnreadCount(_unRead, 0);
}

void ListViewItem::setUnreadCount(QLabel* label, int count)
{
    label->setText(QString::number(count));
    // hide
    if (count == 0)
    {
        label->setVisible(false);
    }
    // show
    else
    {
        label->setVisible(true);
    }
}

} // namespace MINIOICQ
