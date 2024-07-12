#ifndef LIST_VIEW_H
#define LIST_VIEW_H

#include <QWidget>
#include <QLabel>
#include <qtmaterialavatar.h>

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
        - ListViewItems
 - QtMaterialFloatingActionButton
 */

class ListView : public QWidget
{
public:
    ListView(QWidget* parent = 0);
    ~ListView();

private:
    void initUi();
    void initConnect();

    // event
    void mouseReleaseEvent(QMouseEvent* event) override;

Q_SIGNALS:
    void clicked(int cid);
};

} // namespace MINIOICQ

#endif
