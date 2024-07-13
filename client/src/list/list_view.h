#ifndef LIST_VIEW_H
#define LIST_VIEW_H

#include <QAbstractItemModel>
#include <QDataWidgetMapper>
#include <QLabel>
#include <QScrollArea>
#include <QDialog>
#include <QWidget>

#include "qtmaterialappbar.h"
#include "qtmaterialavatar.h"
#include "qtmaterialfab.h"
#include "qtmaterialcheckbox.h"

#include "common/user_info.h"

namespace MINIOICQ
{

class InviteItem: public QWidget
{
    Q_OBJECT

public:
    InviteItem(QWidget* parent = 0);
    void setUser(const UserInfo& user);

private:
    void initUi();
    QtMaterialCheckBox* _checkBox;
    QtMaterialAvatar* _avatar;
    QLabel* _name;

    friend class InviteDialog;
};

class InviteDialog: public QDialog
{
    Q_OBJECT
public:
    InviteDialog(QWidget* parent = 0);
    void setUsers(const QVector<UserInfo>& users);

private:
    void initUi();
    void initConnect();

private:
    QScrollArea* _scrollArea;
    QWidget* _itemList;
    QtMaterialRaisedButton* _inviteButton;
    QtMaterialRaisedButton* _cancelButton;

};

/**
 * @brief
 *
 *
  ┌───────────────────────────────────┐
  │                                   │
  │  ┌───────┐  chatName     lastTime │
  │  │avatar │                        │
  │  └───────┘  lastMsg      unRead   │
  │                                   │
  └───────────────────────────────────┘

    - QHBoxLayout
        - QtMaterialAvatar
        - QVBoxLayout
            - QHBoxLayout
                - QLabel
                - QLabel
            - QLabel
    - QLabel ( float right bottom )

    Static display widget without interaction
 *
 */
class ListViewItem : public QWidget
{
    Q_OBJECT

public:
    ListViewItem(QWidget* parent = 0);
    ~ListViewItem();
    static void setUnreadCount(QLabel* label, int count);

private:
    void initUi();

private:
    // components
    QtMaterialAvatar* _avatar;
    QLabel* _chatName;
    QLabel* _lastTime;
    QLabel* _lastMsg;
    QLabel* _unRead;

    friend class ListView;
};

/**
 * @brief
 *
 *
 ┌────────────┐
 │   title    │
 ├────────────┤
 │  item     s│
 │  item     c│
 │  item     r│
 │  item     o│
 │  item     l│
 │  item     l│
 │  item ┌─┐ B│
 │  item │x│ a│
 │  item └─┘ r│
 │            │
 └────────────┘

 - QVBoxLayout
    - QtMaterialAppBar
    - QScrollArea
        - QVBoxLayout
            - ListViewItems
 - QtMaterialFloatingActionButton
 */

class ListView : public QWidget
{
    Q_OBJECT

public:
    ListView(QWidget* parent = 0);
    ~ListView();

private:
    void initUi();
    void initConnect();
    void setModel(QAbstractItemModel* model);

    // event
    void mouseReleaseEvent(QMouseEvent* event) override;

public slots:
    // from ListViewModel
    void on_dataChanged();

private slots:
    // from components
    void on_invite();
    void on_close();

Q_SIGNALS:
    // to ListViewModel
    void openChat(int cid);
    void closeAll();
    void invite(QVector<int> uids);

private:
    // bind
    QVector<QDataWidgetMapper*> _mappers;
    QAbstractItemModel* _model;

    // components
    QtMaterialAppBar* _appBar;
    QScrollArea* _scrollArea;
    QWidget* _itemList;
    QtMaterialFloatingActionButton* _closeButton;
    QtMaterialFloatingActionButton* _inviteButton;

    friend void bindListView(ListView* view, QAbstractItemModel* model);
};

void bindListView(ListView* view, QAbstractItemModel* model);

} // namespace MINIOICQ

#endif
