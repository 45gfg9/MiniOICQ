#ifndef LIST_VIEWMODEL_H
#define LIST_VIEWMODEL_H

#include "list_model.h"
#include <QSortFilterProxyModel>

namespace MINIOICQ
{

class ListViewModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    int chatIdColumn() const { return _chatIdColumn; }
    int chatNameColumn() const { return _chatNameColumn; }
    int chatAvatarColumn() const { return _chatAvatarColumn; }
    int chatLastMessageColumn() const { return _chatLastMessageColumn; }
    int chatLastMessageTimeColumn() const { return _chatLastMessageTimeColumn; }
    int chatUnreadMessageCountColumn() const
    {
        return _chatUnreadMessageCountColumn;
    }

    void setSourceModel(ListModel* model);
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override;

Q_SIGNALS:
    void open_chat(QVariant chatId);

public slots:
    void on_click_chat(const QVariant& chatId);

private:
    int _chatIdColumn = -1;
    int _chatNameColumn = -1;
    int _chatAvatarColumn = -1;
    int _chatLastMessageColumn = -1;
    int _chatLastMessageTimeColumn = -1;
    int _chatUnreadMessageCountColumn = -1;
};

} // namespace MINIOICQ

#endif // LIST_VIEWMODEL_H
