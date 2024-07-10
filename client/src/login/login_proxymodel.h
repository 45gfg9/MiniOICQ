#ifndef LOGIN_PROXYMODEL_H
#define LOGIN_PROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QSqlTableModel>

#include "login_model.h"

namespace MINIOICQ
{
class LoginProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    using QSortFilterProxyModel::QSortFilterProxyModel;

    int userIdColumn() const { return _userIdColumn; }
    int userNameColumn() const { return _userNameColumn; }
    int passwordColumn() const { return _passwordColumn; }
    int avatarColumn() const { return _avatarColumn; }

    using QSortFilterProxyModel::setSourceModel;
    void setSourceModel(LoginModel* model);
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value,
                 int role = Qt::DisplayRole) override;

private:
    int _userIdColumn = -1;
    int _userNameColumn = -1;
    int _passwordColumn = -1;
    int _avatarColumn = -1;
};

} // namespace MINIOICQ

#endif // LOGIN_PROXYMODEL_H