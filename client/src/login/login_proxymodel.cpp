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

} // namespace MINIOICQ