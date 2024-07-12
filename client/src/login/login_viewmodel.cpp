#include "login_viewmodel.h"
#include <QDebug>

namespace MINIOICQ
{

void LoginViewModel::setSourceModel(LoginModel* model)
{
    QSortFilterProxyModel::setSourceModel(model);
    _userIdColumn = model->fieldIndex("userId");
    _userNameColumn = model->fieldIndex("userName");
    _passwordColumn = model->fieldIndex("password");
    _avatarColumn = model->fieldIndex("avatar");
}

QVariant LoginViewModel::data(const QModelIndex& index, int /*role*/) const
{
    return QSortFilterProxyModel::data(index, Qt::DisplayRole);
}

bool LoginViewModel::setData(const QModelIndex& index, const QVariant& value,
                             int /*role*/)
{
    qDebug() << "LoginViewModel::setData" /* << index */ << value.type()
             << (value.type() == QVariant::ByteArray
                     ? value.toByteArray().size()
                     : value);
    return QSortFilterProxyModel::setData(index, value, Qt::EditRole);
}

bool LoginViewModel::insertItem(const QVariant& userId,
                                const QVariant& userName,
                                const QVariant& password,
                                const QVariant& avatar)
{
    qDebug() << "LoginViewModel::insertItem";
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
    qDebug() << "LoginViewModel::insertItem: failed to insert record";
    return false;
}

void LoginViewModel::on_login(QString userId, QString password)
{
    qDebug() << "LoginViewModel::on_login";
    emit login(userId, password);
}

void LoginViewModel::on_reg(QString userId, QString userName, QString password,
                            QImage avatar)
{
    qDebug() << "LoginViewModel::on_register";
    emit reg(userName, password);
}

void LoginViewModel::on_loginSuccess(QString userId, QString userName,
                                     QString password, QImage avatar)
{
    // edit or insert user data
    auto match_res = match(LoginViewModel::index(0, userIdColumn()), Qt::DisplayRole,
                              userId, 1, Qt::MatchExactly);
    bool res = false;
    if (!match_res.empty())
    {
        // edit user data
        QModelIndex index = match_res.first();
        res |= setData(index.siblingAtColumn(userNameColumn()), userName);
        res |= setData(index.siblingAtColumn(passwordColumn()), password);
        res |= setData(index.siblingAtColumn(avatarColumn()), avatar);
    }
    else
    {
        // update user data
        res = insertItem(userId, userName, password, avatar);
    }

    if (!res)
    {
        qDebug() << "LoginViewModel::on_loginSuccess: failed to update user "
                    "data in model";
        throw std::runtime_error("Update user data failed");
    }
    qDebug() << "LoginViewModel::loginSuccess";

    // emit signal to LoginView to show main window
    emit loginSuccess();
    _loggedUserId = userId;
}

void LoginViewModel::on_loginFailed()
{
    qDebug() << "LoginViewModel::loginFailed";
    emit loginFailed();
}

void LoginViewModel::on_registerSuccess(QString userId, QImage avatar)
{
    qDebug() << "LoginViewModel::on_registerSuccess";
    // emit registerSuccess();
}

} // namespace MINIOICQ
