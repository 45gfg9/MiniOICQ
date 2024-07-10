#ifndef LOGIN_VIEW_H
#define LOGIN_VIEW_H

/*
 ┌──────────────────────────┐
 │                          │
 │          Login           │
 │   ─────────────────────  │
 │                          │
 │   ┌──────┐               │
 │   │      │  UserName     │
 │   │Avatar│  UserId       │
 │   │      │  Password     │
 │   └──────┘               │
 │                          │
 │    Login    Register     │
 │                          │
 └──────────────────────────┘
- QVBoxLayout
    - QLabel
    - QHBoxLayout
        - QtMaterialAvatar
        - QVBoxLayout
            - QtMaterialAutoComplete
            - QtMaterialTextField
    - QHBoxLayout
        - QtMaterialRaisedButton
        - QtMaterialRaisedButton
*/

#include "login_proxymodel.h"
#include <QAbstractItemModel>
#include <QDataWidgetMapper>
#include <QDialog>
#include <QLabel>
#include <QStyledItemDelegate>
#include <qtmaterialautocomplete.h>
#include <qtmaterialavatar.h>
#include <qtmaterialraisedbutton.h>
#include <qtmaterialtextfield.h>

namespace MINIOICQ
{
class LoginDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    using QStyledItemDelegate::QStyledItemDelegate;

private:
    void setEditorData(QWidget* editor,
                       const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const override;
};

class LoginView : public QDialog
{
    Q_OBJECT
public:
    LoginView(QWidget* parent = nullptr);
    ~LoginView();
    void setModel(LoginProxyModel* model);

private:
    void initUI();
    void initConnect();

public slots:

    void loginSuccess();
    void loginFailed();
    void registerSuccess();
    void registerFailed();

private slots:

    void on_login_clicked();
    void on_register_clicked();
    void on_userId_itemSelected(QString userId);

Q_SIGNALS:

    void login(QString userId, QString password);
    void reg(QString userId, QString userName, QString password,
                         QImage avatar);

private:
    // data
    QDataWidgetMapper* _mapper;
    LoginProxyModel* _model;
    QStringListModel* _userIdModel;

    // components
    QtMaterialTextField* _userName;
    QtMaterialAvatar* _avatar;
    QtMaterialAutoComplete* _userId;
    QtMaterialTextField* _password;
    QtMaterialRaisedButton* _login;
    QtMaterialRaisedButton* _register;

    //misc
    void keyPressEvent(QKeyEvent* event) override;
};
} // namespace MINIOICQ

#endif // LOGIN_VIEW_H