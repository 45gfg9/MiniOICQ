#include "login_viewmodel.h"
#include "common/misc.h"

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
    return QSortFilterProxyModel::setData(index, value, Qt::EditRole);
}

bool LoginViewModel::insertItem(const UserInfo& info)
{
    QSqlRecord record;
    record.append(QSqlField("userId", QVariant::Int));
    record.append(QSqlField("userName", QVariant::String));
    record.append(QSqlField("password", QVariant::String));
    record.append(QSqlField("avatar", QVariant::ByteArray));
    record.setValue("userId", info.userId().toInt());
    record.setValue("userName", info.username());
    record.setValue("password", info.password());
    QByteArray arr;
    QBuffer buffer(&arr);
    buffer.open(QIODevice::WriteOnly);
    info.avatar().save(&buffer, "JPG");
    record.setValue("avatar", arr);

    // qt base
    // QByteArray ba = field.value().toByteArray();
    // QString res;
    // static const char hexchars[] = "0123456789abcdef";
    // for (int i = 0; i < ba.size(); ++i)
    // {
    //     uchar s = (uchar)ba[i];
    //     res += QLatin1Char(hexchars[s >> 4]);
    //     res += QLatin1Char(hexchars[s & 0x0f]);
    // }
    // qDebug() << QLatin1Char('\'') + res + QLatin1Char('\'');
    // qt base
    // qDebug() << "LoginViewModel::insertItem" << record;
    QSqlTableModel* model = qobject_cast<QSqlTableModel*>(sourceModel());
    if (model->insertRecord(-1, record))
    {
        return model->submitAll();
    }
    Warning("LoginViewModel::insertItem: failed to insert record");
    return false;
}

void LoginViewModel::setWsConnector(WebSocketConnector* wsConnector)
{
    // to WebSocketConnector
    connect(this, &LoginViewModel::login, wsConnector,
            &WebSocketConnector::on_login);
    connect(this, &LoginViewModel::reg, wsConnector,
            &WebSocketConnector::on_reg);
    // from WebSocketConnector
    connect(wsConnector, &WebSocketConnector::loginSuccess, this,
            &LoginViewModel::on_loginSuccess);
    connect(wsConnector, &WebSocketConnector::loginFailed, this,
            &LoginViewModel::on_loginFailed);
    connect(wsConnector, &WebSocketConnector::regSuccess, this,
            &LoginViewModel::on_regSuccess);
    connect(wsConnector, &WebSocketConnector::regFailed, this,
            &LoginViewModel::on_regFailed);
}

void LoginViewModel::on_login(QString userId, QString password)
{
    emit login(userId, password);
}

void LoginViewModel::on_reg(QString userName, QString password)
{
    emit reg(userName, password);
}

void LoginViewModel::on_loginSuccess(const UserInfo& info)
{
    // edit or insert user data
    auto match_res = match(LoginViewModel::index(0, userIdColumn()),
                           Qt::DisplayRole, info.userId(), 1, Qt::MatchExactly);
    bool res = false;
    if (!match_res.empty())
    {
        // edit user data
        QModelIndex index = match_res.first();
        res |=
            setData(index.siblingAtColumn(userNameColumn()), info.username());
        res |=
            setData(index.siblingAtColumn(passwordColumn()), info.password());
    QByteArray arr;
    QBuffer buffer(&arr);
    buffer.open(QIODevice::WriteOnly);
    info.avatar().save(&buffer, "JPG");
        res |= setData(index.siblingAtColumn(avatarColumn()), arr);
        if (!res)
        {
            Error("LoginViewModel::on_loginSuccess: failed to edit user data");
        }
    }
    else
    {
        // update user data
        res = insertItem(info);
        if (!res)
        {
            Error(
                "LoginViewModel::on_loginSuccess: failed to update user data");
        }
    }

    _loggedUserId = info.userId();

    // to LoginView on_loginSuccess
    emit loginSuccess();
}

void LoginViewModel::on_loginFailed(const QString& reason)
{
    emit loginFailed(reason);
}

void LoginViewModel::on_regSuccess(const UserInfo& info)
{
    // update user data
    if (!insertItem(info))
    {
        Error("LoginViewModel::on_regSuccess: failed to update user data");
    }
    emit regSuccess(info.userId());
}

void LoginViewModel::on_regFailed(const QString& reason)
{
    emit regFailed(reason);
}

} // namespace MINIOICQ
