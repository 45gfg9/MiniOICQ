#ifndef LIST_VIEW_H
#define LIST_VIEW_H

#include <QAbstractItemModel>
#include <QDataWidgetMapper>
#include <QLabel>
#include <QScrollArea>
#include <QWidget>

#include "qtmaterialappbar.h"
#include "qtmaterialavatar.h"
#include "qtmaterialfab.h"

namespace MINIOICQ
{

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
    // void mouseReleaseEvent(QMouseEvent* event) override;

public slots:
    // debug
    // void on_debug();

    // from ListViewModel
    void on_dataChanged(QModelIndex topLeft, QModelIndex bottomRight, QVector<int> roles);

Q_SIGNALS:
    void clicked(int cid);

private:
    // bind
    QVector<QDataWidgetMapper*> _mappers;

    // components
    QtMaterialAppBar* _appBar;
    QScrollArea* _scrollArea;
    QWidget* _itemList;
    QtMaterialFloatingActionButton* _closeButton;

    friend void bindListView(ListView* view, QAbstractItemModel* model);
};

void bindListView(ListView* view, QAbstractItemModel* model);

} // namespace MINIOICQ

#endif
