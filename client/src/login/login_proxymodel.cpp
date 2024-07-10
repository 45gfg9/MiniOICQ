#include "login_proxymodel.h"
#include <QDebug>

namespace MINIOICQ
{

void LoginProxyModel::setSourceModel(LoginModel* model)
{
    qDebug() << "LoginProxyModel::setSourceModel";
    QSortFilterProxyModel::setSourceModel(model);
    _userIdColumn = model->fieldIndex("userId");
    _userNameColumn = model->fieldIndex("userName");
    _passwordColumn = model->fieldIndex("password");
    _avatarColumn = model->fieldIndex("avatar");
    qDebug() << "userIdColumn: " << _userIdColumn;
    qDebug() << "userNameColumn: " << _userNameColumn;
    qDebug() << "passwordColumn: " << _passwordColumn;
    qDebug() << "avatarColumn: " << _avatarColumn;
}

QVariant LoginProxyModel::data(const QModelIndex& index, int /*role*/) const
{
    return QSortFilterProxyModel::data(index, Qt::DisplayRole);
}

bool LoginProxyModel::setData(const QModelIndex& index, const QVariant& value,
                              int /*role*/)
{
    qDebug() << "LoginProxyModel::setData" /* << index */ << value.type()
             << (value.type() == QVariant::ByteArray
                     ? value.toByteArray().size()
                     : value);
    return QSortFilterProxyModel::setData(index, value, Qt::DisplayRole);
}

} // namespace MINIOICQ