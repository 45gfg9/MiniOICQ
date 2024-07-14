#include <QDate>
#include <QDebug>
#include <QDialog>
#include <QHBoxLayout>
#include <QTime>
#include <QVBoxLayout>

#include "qtmaterialcheckbox.h"
#include "qtmaterialiconbutton.h"
#include "qtmaterialscrollbar.h"

#include "common/misc.h"
#include "list_view.h"
#include "list_viewmodel.h"

namespace MINIOICQ
{

InviteItem::InviteItem(QWidget* parent) : QWidget(parent) { initUi(); }

void InviteItem::setUser(const UserInfo& user)
{
    _avatar->setImage(user.avatar());
    _checkBox->setText(user.username());
}

void InviteItem::initUi()
{
    _avatar = new QtMaterialAvatar(this);
    _checkBox = new QtMaterialCheckBox(this);

    // arrange
    auto layout = new QHBoxLayout(this);
    layout->addWidget(_avatar);
    layout->addWidget(_checkBox);
    layout->addStretch(1);

    // style
    // // item
    this->setPalette(themePalette);
    this->setFixedSize(Width, Height);
    this->setContentsMargins(zeroMargins);
    // layout: border 16, space 8
    layout->setContentsMargins(8, 0, 10, 0);
    layout->setSpacing(0);
    // // avatar: 40 * 40
    // _avatar->setSize(40);
    // // name: height 20, font 16
    // _name->setStyleSheet("font-size: 16px;");
    // _name->setFixedSize(kListViewWidth - (16 * 2 + 40 + 8), 20);
    // // checkBox: 20 * 20
    // _checkBox->setFixedSize(20, 20);
    _checkBox->setStyleSheet("font-size: 14px;");
    _checkBox->setCheckedIcon(QIcon(":/checkBox.svg"));
    _checkBox->setUncheckedIcon(QIcon(":/checkBoxOutline.svg"));

    _avatar->setSize(32);
}

InviteDialog::InviteDialog(QWidget* parent) : QtMaterialDialog(parent)
{
    initUi();
}

void InviteDialog::setUsers(const QVector<UserInfo>& users)
{
    for (const auto& user : users)
    {
        auto* item = new InviteItem(_itemList);
        _itemList->layout()->addWidget(item);
        item->setUser(user);
    }
    _itemList->setFixedHeight(InviteItem::Height * users.size());
}

void InviteDialog::initUi()
{
    QWidget* widget = new QWidget(this);
    _scrollArea = new QScrollArea(widget);
    _itemList = new QWidget(_scrollArea);
    _inviteButton = new QtMaterialRaisedButton("Invite", widget);
    _cancelButton = new QtMaterialRaisedButton("Cancel", widget);

    // arrange
    auto layout = new QVBoxLayout;
    auto dialogLayout = new QVBoxLayout(widget);
    auto itemListLayout = new QVBoxLayout(_itemList);
    _scrollArea->setWidget(_itemList);
    dialogLayout->addWidget(_scrollArea);
    dialogLayout->addWidget(_inviteButton);
    dialogLayout->addWidget(_cancelButton);
    layout->addWidget(widget);
    this->setWindowLayout(layout);

    // style
    // // dialog
    this->setFixedSize(ListView::Width, ListView::Height);
    widget->setFixedSize(Width, Height);
    // widget->setContentsMargins(zeroMargins);
    // layout->setContentsMargins(zeroMargins);
    // // scrollArea
    _scrollArea->setWidgetResizable(false);
    _scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _scrollArea->setVerticalScrollBar(new QtMaterialScrollBar);
    // // itemList
    _itemList->setFixedWidth(Width);
    // // itemListLayout
    itemListLayout->setContentsMargins(zeroMargins);
    // // inviteButton
    _inviteButton->setFixedSize(Width, 40);
    // // cancelButton
    _cancelButton->setFixedSize(Width, 40);
}

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
    this->setPalette(themePalette);
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
    // _chatName->setText("chatName");
    // _lastTime->setText(QDateTime::currentDateTime().toString("hh:mm"));
    // _lastMsg->setText("lastMsg");
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
    _inviteButton =
        new QtMaterialFloatingActionButton(QIcon(":/invite.svg"), this);

    // arrange
    auto layout = new QVBoxLayout(this);
    auto itemListLayout = new QVBoxLayout(_itemList);
    _scrollArea->setWidget(_itemList);
    layout->addWidget(_appBar);
    layout->addWidget(_scrollArea);

    // style
    // ListView
    this->setFixedSize(kListViewWidth, kListViewHeight);
    this->layout()->setSpacing(0);
    this->layout()->setContentsMargins(zeroMargins);
    this->setAttribute(Qt::WA_DeleteOnClose);
    // this->setWindowFlag(Qt::FramelessWindowHint);
    // appBar
    QLabel* appBarTitle = new QLabel("Inbox", _appBar);
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
    itemListLayout->setContentsMargins(zeroMargins);
    // clostButton
    _closeButton->setOffset(16, 16);
    _closeButton->setMini(true);
    // inviteButton
    _inviteButton->setOffset(16, 48);
    _inviteButton->setMini(true);

    // inviteDialog
    _inviteDialog = new InviteDialog(this);
}

void ListView::initConnect()
{
    connect(_closeButton, &QtMaterialFloatingActionButton::clicked, this,
            &ListView::on_close);
    connect(_inviteButton, &QtMaterialFloatingActionButton::clicked,
            _inviteDialog, &InviteDialog::showDialog);
    connect(_inviteDialog->_cancelButton, &QtMaterialRaisedButton::clicked,
            _inviteDialog, &InviteDialog::hideDialog);
    connect(_inviteDialog->_inviteButton, &QtMaterialRaisedButton::clicked,
            this, &ListView::on_invite);
}

void ListView::on_invite()
{
    if (_inviteDialog->_uids.size() != 0)
        emit invite(_inviteDialog->_uids);
}

void ListView::on_close()
{
    qDebug() << "ListView::on_close";
    emit closeAll();
    close();
}

void ListView::on_dataChanged()
{
    auto* model = qobject_cast<ListViewModel*>(_model);
    // qDebug() << "ListView::on_dataChanged: " << model;
    // clear mapper
    for (auto* mapper : _mappers)
    {
        mapper->clearMapping();
        delete mapper;
    }
    _mappers.clear();
    auto* layout = _itemList->layout();

    // adjust item count
    auto modelItemNum = model->rowCount();
    auto layoutItemNum = layout->count();
    qDebug() << "ListView::on_dataChanged: modelItemNum" << modelItemNum
             << "layoutItemNum" << layoutItemNum;
    if (modelItemNum > layoutItemNum)
    {
        for (int i = layoutItemNum; i < modelItemNum; i++)
        {
            layout->addWidget(new ListViewItem(_itemList));
        }
    }
    else if (modelItemNum < layoutItemNum)
    {
        for (int i = layoutItemNum; i > modelItemNum; i--)
        {
            auto* item = layout->itemAt(i - 1)->widget();
            layout->removeWidget(item);
            delete item;
        }
    }

    // refresh item
    for (int i = 0; i < modelItemNum; i++)
    {
        auto* item = qobject_cast<ListViewItem*>(layout->itemAt(i)->widget());
        auto* mapper = new QDataWidgetMapper(this);
        mapper->setModel(model);
        mapper->addMapping(item->_chatName, model->chatNameColumn(), "text");
        mapper->addMapping(item->_lastTime, model->chatLastMessageTimeColumn(),
                           "text");
        mapper->addMapping(item->_lastMsg, model->chatLastMessageColumn(),
                           "text");
        mapper->setCurrentIndex(i);
        _mappers.push_back(mapper);
        item->setUnreadCount(
            item->_unRead,
            model->data(model->index(i, model->chatUnreadMessageCountColumn()))
                .toInt());
        QImage image;
        image.loadFromData(
            model->data(model->index(i, model->chatAvatarColumn()))
                .toByteArray());
        item->_avatar->setImage(image);
    }

    // adjust height
    this->_itemList->setFixedHeight(kListViewItemHeight * modelItemNum);
    qDebug() << "ListView::on_dataChanged: height"
             << kListViewHeight * modelItemNum;
    _inviteDialog->setUsers(model->selectUser());
}

void ListView::setModel(QAbstractItemModel* model)
{
    _model = model;
    auto listViewModel = qobject_cast<ListViewModel*>(model);
    // connect
    connect(listViewModel, &QAbstractItemModel::dataChanged, this,
            &ListView::on_dataChanged);
    connect(listViewModel, &QAbstractProxyModel::sourceModelChanged, this,
            &ListView::on_dataChanged);
    connect(this, &ListView::openChat, listViewModel,
            &ListViewModel::on_openChat);
    connect(this, &ListView::closeAll, listViewModel,
            &ListViewModel::on_closeAll);

    // qDebug() << "ListView::setModel: " << model;
    auto mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    _mappers.push_back(mapper);
}

void ListView::mouseReleaseEvent(QMouseEvent* event)
{
    QWidget* widget = childAt(event->pos());
    if (widget)
        qDebug() << "ListView::mouseReleaseEvent: " << widget;
    while (widget)
    {
        // break when widget is ListViewItem
        if (qobject_cast<ListViewItem*>(widget))
        {
            qDebug() << "ListView::mouseReleaseEvent: " << widget;
            break;
        }
        widget = widget->parentWidget();
    }
    if (!widget)
    {
        QDebug(QtWarningMsg) << "ListView::mouseReleaseEvent: no widget";
        return;
    }
    auto index = _itemList->layout()->indexOf(widget);
    qDebug() << "ListView::mouseReleaseEvent: " << index;
    emit openChat(index);
    // print parent widget
    // if (widget)
    // {
    //     const QLabel* const label = qobject_cast<QLabel*>(widget);
    //     if (label)
    //     {
    //         qDebug() << "label" << label->text();
    //     }
    //     const int index = centralWidget()->layout()->indexOf(widget);
    //     qDebug() << "layout index" << index;
    //     if (index >= 0)
    //     {
    //         const QLayoutItem* const item =
    //             centralWidget()->layout()->itemAt(index);
    //         qDebug() << "layout item" << item;
    //     }
    // }
}

void bindListView(ListView* view, QAbstractItemModel* model)
{
    view->setModel(model);
}

} // namespace MINIOICQ
