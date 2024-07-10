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
    _userIdColumn = model->fieldIndex ("userId");
    _userNameColumn = model->fieldIndex ("userName");
    _passwordColumn = model->fieldIndex ("password");
    _avatarColumn = model->fieldIndex ("avatar");
}

QVariant LoginProxyModel::data (const QModelIndex& index, int role) const {
    qDebug () << "LoginProxyModel::data";
    if (role == Qt::DisplayRole) {
        return QSortFilterProxyModel::data (index, role);
    }
    return QVariant ();
}

} // namespace MINIOICQ