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

bool LoginProxyModel::insertItem(const QVariant& userId,
                                 const QVariant& userName,
                                 const QVariant& password,
                                 const QVariant& avatar)
{
    qDebug() << "LoginProxyModel::insertItem";
    QSqlRecord record;
    record.append(QSqlField("userId", QVariant::Int));
    record.append(QSqlField("userName", QVariant::String));
    record.append(QSqlField("password", QVariant::String));
    record.append(QSqlField("avatar", QVariant::ByteArray));
    record.setValue("userId", userId.toInt());
    record.setValue("userName", userName.toString());
    record.setValue("password", password.toString());
    record.setValue("avatar", avatar.toBitArray());
    QSqlTableModel* model = qobject_cast<QSqlTableModel*>(sourceModel());
    qDebug() << "insertRecord: " << record;
    if (model->insertRecord(-1, record))
    {
        return model->submitAll();
    }
    return false;
}

void LoginProxyModel::on_login(QString userId, QString password)
{
    qDebug() << "LoginProxyModel::on_login";
    emit login(userId, password);
    on_loginSuccess(userId, "User", password, QImage());
}

void LoginProxyModel::on_reg(QString userId, QString userName, QString password,
                             QImage avatar)
{
    qDebug() << "LoginProxyModel::on_register";
}

void LoginProxyModel::on_loginSuccess(QString userId, QString userName,
                                      QString password, QImage avatar)
{
    // edit or insert user data
    LoginModel* model = qobject_cast<LoginModel*>(sourceModel());
    auto match = model->match(model->index(0, userIdColumn()), Qt::DisplayRole,
                              userId, 1, Qt::MatchExactly);
    bool res = false;
    if (!match.empty())
    {
        // edit user data
        QModelIndex index = match.first();
        res |= model->setData(index, userName, userNameColumn());
        res |= model->setData(index, password, passwordColumn());
        res |= model->setData(index, avatar, avatarColumn());
    }
    else
    {
        // update user data
        res = insertItem(userId, userName, password, avatar);
    }

    if (!res)
    {
        qDebug() << "LoginProxyModel::on_loginSuccess: failed to update user "
                    "data in model";
        throw std::runtime_error("Update user data failed");
    }
    qDebug() << "LoginProxyModel::loginSuccess";

    // emit signal to LoginView to show main window
    emit loginSuccess(userId);
    _loggedUserId = userId;
}

void LoginProxyModel::on_loginFailed()
{
    qDebug() << "LoginProxyModel::loginFailed";
    emit loginFailed();
}

} // namespace MINIOICQ