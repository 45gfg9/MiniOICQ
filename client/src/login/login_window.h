#ifndef LOGIN_WINDOW_H
#define LOGIN_WINDOW_H

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

#include <QDialog>
#include <QLabel>
#include <qtmaterialavatar.h>
#include <qtmaterialautocomplete.h> 
#include <qtmaterialtextfield.h>
#include <qtmaterialraisedbutton.h>

namespace MINIOICQ {
    class LoginWindow : public QDialog {
        Q_OBJECT
    public:
        LoginWindow(QWidget *parent = nullptr);
        ~LoginWindow();
    private:
        void initUI();
        void initConnect();
    private:
        QLabel *_title;
        QtMaterialAvatar *_avatar;
        QtMaterialAutoComplete *_username;
        QtMaterialTextField *_password;
        QtMaterialRaisedButton *_login;
        QtMaterialRaisedButton *_register;
    };
} // namespace MINIOICQ

#endif // LOGIN_WINDOW_H