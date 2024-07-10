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
#include <qtmaterialautocomplete.h>
#include <qtmaterialavatar.h>
#include <qtmaterialraisedbutton.h>
#include <qtmaterialtextfield.h>
#include "login_proxymodel.h"

namespace MINIOICQ {
class LoginView : public QDialog {
    Q_OBJECT
    public:
    LoginView (QWidget* parent = nullptr);
    ~LoginView ();
    void setModel (LoginProxyModel* model);

    private:
    void initUI ();
    void initConnect ();

    public slots:
    void on_model_dataChanged (const QModelIndex& topLeft, const QModelIndex& bottomRight,
                               const QVector<int>& roles);

    private:
    // data
    QDataWidgetMapper* _mapper;
    LoginProxyModel* _model;
    // components
    QLabel* _title;
    QLabel* _userName;
    QtMaterialAvatar* _avatar;
    QtMaterialAutoComplete* _userId;
    QtMaterialTextField* _password;
    QtMaterialRaisedButton* _login;
    QtMaterialRaisedButton* _register;
};
} // namespace MINIOICQ

#endif // LOGIN_VIEW_H