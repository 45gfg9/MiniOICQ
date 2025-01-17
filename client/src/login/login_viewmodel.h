#ifndef LOGIN_VIEWMODEL_H
#define LOGIN_VIEWMODEL_H

#include <QImage>
#include <QSortFilterProxyModel>
#include <QSqlTableModel>

#include "common/user_info.h"
#include "login_model.h"
#include "websocket/websocket.h"

namespace MINIOICQ
{
class LoginViewModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    using QSortFilterProxyModel::QSortFilterProxyModel;

    int userIdColumn() const { return _userIdColumn; }
    int userNameColumn() const { return _userNameColumn; }
    int passwordColumn() const { return _passwordColumn; }
    int avatarColumn() const { return _avatarColumn; }
    QString loggedUserId() const { return _loggedUserId; }

    using QSortFilterProxyModel::setSourceModel;
    void setSourceModel(LoginModel* model);
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value,
                 int role = Qt::DisplayRole) override;
    bool insertItem(const UserInfo& info);

    void setWsConnector(WebSocketConnector* wsConnector);

Q_SIGNALS:

    // to WebSocketConnector
    void login(const QString& userId, const QString& password);
    void reg(QString userName, QString password);

    // to LoginView
    void loginSuccess();
    void loginFailed(QString message);
    void regSuccess(QString uid);
    void regFailed(QString message);

public slots:

    // from LoginView
    void on_login(QString userId, QString password);
    void on_reg(QString userName, QString password);

    // from WebSockerConnector
    void on_loginSuccess(const UserInfo& info);
    void on_loginFailed(const QString& reason);
    void on_regSuccess(const UserInfo& info);
    void on_regFailed(const QString& reason);

private:
    int _userIdColumn = -1;
    int _userNameColumn = -1;
    int _passwordColumn = -1;
    int _avatarColumn = -1;
    QString _loggedUserId;
};

} // namespace MINIOICQ

#endif // LOGIN_VIEWMODEL_H
