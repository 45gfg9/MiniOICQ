#ifndef LOGIN_VIEW_H
#define LOGIN_VIEW_H

/*
 ┌──────────────────────────┐
 │                          │
 │          Login           │
 │   ─────────────────────  │
 │                          │
 │   ┌──────┐               │
 │   │      │  Username     │
 │   │Avatar│               │
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

namespace MINIOICQ {
class LoginView : public QDialog {
    Q_OBJECT
    public:
    LoginView (QWidget* parent = nullptr);
    ~LoginView ();
    void setModel (QAbstractItemModel* model);

    private:
    void initUI ();
    void initConnect ();

    private:
    // data
    QDataWidgetMapper* _mapper;
    QAbstractItemModel* _model;
    int _userIdColumn   = -1;
    int _passwordColumn = -1;
    int _avatarColumn   = -1;
    // components
    QLabel* _title;
    QtMaterialAvatar* _avatar;
    QtMaterialAutoComplete* _userId;
    QtMaterialTextField* _password;
    QtMaterialRaisedButton* _login;
    QtMaterialRaisedButton* _register;
};
} // namespace MINIOICQ

#endif // LOGIN_VIEW_H