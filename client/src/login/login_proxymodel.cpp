#include "login_proxymodel.h"
#include <QDebug>

namespace MINIOICQ {

LoginProxyModel::LoginProxyModel (QObject* parent)
: QSortFilterProxyModel (parent) {
    qDebug () << "LoginProxyModel::LoginProxyModel";
}

LoginProxyModel::~LoginProxyModel () {
    qDebug () << "LoginProxyModel::~LoginProxyModel";
}

void LoginProxyModel::setSourceModel (LoginModel* model) {
    qDebug () << "LoginProxyModel::setSourceModel";
    QSortFilterProxyModel::setSourceModel (model);
    _userIdColumn   = model->fieldIndex ("userId");
    _userNameColumn = model->fieldIndex ("userName");
    _passwordColumn = model->fieldIndex ("password");
    _avatarColumn   = model->fieldIndex ("avatar");
    qDebug () << "userIdColumn: " << _userIdColumn;
    qDebug () << "userNameColumn: " << _userNameColumn;
    qDebug () << "passwordColumn: " << _passwordColumn;
    qDebug () << "avatarColumn: " << _avatarColumn;
}

QVariant LoginProxyModel::data (const QModelIndex& index, int /*role*/) const {
    qDebug () << "LoginProxyModel::data" << index;
    if (index.column () == _avatarColumn) {
        return QSortFilterProxyModel::data (index, Qt::DisplayRole);
    } else {
        return QSortFilterProxyModel::data (index, Qt::DisplayRole).toString ();
    }
    return QVariant ();
}

bool LoginProxyModel::setData (const QModelIndex& index, const QVariant& value, int /*role*/) {
    qDebug () << "LoginProxyModel::setData" << index << value;
    return QSortFilterProxyModel::setData (index, value, Qt::DisplayRole);
}


} // namespace MINIOICQ