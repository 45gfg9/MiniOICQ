#ifndef LOGIN_PROXYMODEL_H
#define LOGIN_PROXYMODEL_H

#include <QObject>
#include <QVector>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>

#include "login_model.h"

namespace MINIOICQ {
class LoginProxyModel : public QSortFilterProxyModel{

    public:
    LoginProxyModel (QObject* parent = nullptr);
    ~LoginProxyModel ();

};
} // namespace MINIOICQ

#endif // LOGIN_PROXYMODEL_H