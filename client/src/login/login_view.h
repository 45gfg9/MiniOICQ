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

#include <QAbstractItemModel>
#include <QDataWidgetMapper>
#include <QDialog>
#include <QLabel>
#include <QStyledItemDelegate>
#include <qtmaterialautocomplete.h>
#include <qtmaterialavatar.h>
#include <qtmaterialraisedbutton.h>
#include <qtmaterialsnackbar.h>
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

private:
    void initUI();
    void initConnect();
    void setModel(QAbstractItemModel* model);

    // event
    void keyPressEvent(QKeyEvent* event) override;

public slots:

    // LoginViewModel
    void on_loginSuccess();
    void on_loginFailed(QString message);
    void on_regSuccess(QString uid);
    void on_regFailed(QString message);

private slots:

    // components
    void on_login_clicked();
    void on_reg_clicked();
    void on_userId_itemSelected(QString userId);

Q_SIGNALS:

    // LoginViewModel
    void login(QString userId, QString password);
    void reg(QString userName, QString password);

private:
    // bind
    QDataWidgetMapper* _mapper;

    // components
    QtMaterialTextField* _userName;
    QtMaterialAvatar* _avatar;
    QtMaterialAutoComplete* _userId;
    QtMaterialTextField* _password;
    QtMaterialRaisedButton* _login;
    QtMaterialRaisedButton* _reg;
    QtMaterialSnackbar* _snackbar;

    friend void bindLoginView(LoginView* view, QAbstractItemModel* model);
};

void bindLoginView(LoginView* view, QAbstractItemModel* model);

} // namespace MINIOICQ

#endif // LOGIN_VIEW_H
