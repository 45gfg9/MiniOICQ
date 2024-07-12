#ifndef LOGIN_VIEWMODEL_H
#define LOGIN_VIEWMODEL_H

#include <QImage>
#include <QSortFilterProxyModel>
#include <QSqlTableModel>

#include "login_model.h"

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
    bool insertItem(const QVariant& userId, const QVariant& userName,
                    const QVariant& password, const QVariant& avatar);

Q_SIGNALS:
    // websocket
    void login(const QString& userId, const QString& password);
    void reg(QString userName, QString password);
    // LoginView
    void loginSuccess(const QString& userId);
    void loginFailed();

public slots:

    // from LoginView
    void on_login(QString userId, QString password);

    void on_reg(QString userId, QString userName, QString password,
                QImage avatar);

    // websocket
    void on_loginSuccess(QString userId, QString userName, QString password,
                         QImage avatar);
    void on_loginFailed();
    void on_registerSuccess(QString userId, QImage avatar);

private:
    int _userIdColumn = -1;
    int _userNameColumn = -1;
    int _passwordColumn = -1;
    int _avatarColumn = -1;
    QString _loggedUserId;
};

} // namespace MINIOICQ

#endif // LOGIN_VIEWMODEL_H
