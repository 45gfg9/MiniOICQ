#include "list_view.h"
#include "qtmaterialiconbutton.h"
#include "qtmaterialscrollbar.h"
#include <QDate>
#include <QDebug>
#include <QHBoxLayout>
#include <QTime>
#include <QVBoxLayout>

namespace MINIOICQ
{

static int constexpr kListViewWidth = 252;
static int constexpr kListViewItemHeight = 72;

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
    auto vLayout = new QVBoxLayout;
    auto hLayout = new QHBoxLayout;
    hLayout->addWidget(_chatName);
    hLayout->addWidget(_lastTime);
    vLayout->addLayout(hLayout);
    vLayout->addWidget(_lastMsg);
    layout->addWidget(_avatar);
    layout->addLayout(vLayout);

    // style
    // item
    this->setFixedSize(kListViewWidth, kListViewItemHeight);
    // layout: border 16, space 8
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(8);
    // avatar: 40 * 40
    _avatar->setSize(40);
    // vLayout: height 40, space 4
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(4);
    // chatName: height 20, font 16
    _chatName->setStyleSheet("font-size: 16px;");
    _chatName->setFixedSize(kListViewWidth - (16 * 2 + 40 + 8), 20);
    // lastTime: font 10
    _lastTime->setStyleSheet("font-size: 10px; color: gray;");
    // lastMsg: height 16, font 12
    _lastMsg->setStyleSheet("font-size: 12px; color:gray;");
    _lastMsg->setFixedSize(kListViewWidth - (16 * 2 + 40 + 8), 16);
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

ListView::ListView(QWidget* parent) : QWidget(parent)
{
    initUi();
    initConnect();
}

ListView::~ListView() {}

static int constexpr kListViewHeight = 7 * kListViewItemHeight;

void ListView::initUi()
{
    _appBar = new QtMaterialAppBar(this);
    _scrollArea = new QScrollArea(this);
    _itemList = new QWidget(_scrollArea);
    _closeButton =
        new QtMaterialFloatingActionButton(QIcon(":/close.svg"), this);

    // arrange
    auto layout = new QVBoxLayout(this);
    auto itemListLayout = new QVBoxLayout(_itemList);

    // fill default data
    for(int i = 0; i < 11; i++)
    {
        itemListLayout->addWidget(new ListViewItem(_itemList));
    }

    _scrollArea->setWidget(_itemList);
    layout->addWidget(_appBar);
    layout->addWidget(_scrollArea);

    // style
    // ListView
    this->setFixedSize(kListViewWidth + 2,
                       kListViewHeight); // can't understand this border 1
    this->layout()->setSpacing(0);
    this->layout()->setContentsMargins(0, 0, 0, 0);
    // this->setAttribute(Qt::WA_DeleteOnClose);
    // this->setWindowFlag(Qt::FramelessWindowHint);
    // appBar
    QLabel* appBarTitle = new QLabel("Inbox");
    appBarTitle->setStyleSheet("color: white; font-size: 18px;");
    QtMaterialIconButton* button =
        new QtMaterialIconButton(QIcon(":/menu.svg"), this);
    button->setIconSize(QSize(24, 24));
    button->setColor(Qt::white);
    button->setFixedWidth(42);
    _appBar->appBarLayout()->addWidget(button);
    _appBar->appBarLayout()->addWidget(appBarTitle);
    _appBar->appBarLayout()->addStretch(1);
    // scrollArea
    _scrollArea->setWidgetResizable(false);
    _scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _scrollArea->setVerticalScrollBar(new QtMaterialScrollBar);
    // itemList
    _itemList->setFixedWidth(kListViewWidth);
    // itemListLayout
    itemListLayout->setContentsMargins(0, 0, 0, 0);
    // clostButton
    _closeButton->setOffset(16, 16);
    // _closeButton->setMini(true);
}

void ListView::initConnect()
{
    connect(_closeButton, &QtMaterialFloatingActionButton::clicked, this,
            &ListView::close);
}

void ListView::sourceModelChanged(const QModelIndex& topLeft,
                                  const QModelIndex& bottomRight,
                                  const QVector<int>& /* roles*/)
{
    qDebug() << "sourceModelChanged";
}

void ListView::setModel(QAbstractItemModel* model)
{
    connect(model, &QAbstractItemModel::dataChanged, this,
            &ListView::sourceModelChanged);
}

} // namespace MINIOICQ
