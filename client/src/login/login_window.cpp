#include "login_window.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

namespace MINIOICQ {
    LoginWindow::LoginWindow(QWidget *parent)
        : QDialog(parent)
    {
        initUI();
        initConnect();
    }

    LoginWindow::~LoginWindow()
    {
    }

    void LoginWindow::initUI()
    {
        // components
        QVBoxLayout *layout = new QVBoxLayout;
        setLayout(layout);

        _title = new QLabel("Login");
        layout->addWidget(_title);

        QHBoxLayout *avatarLayout = new QHBoxLayout;
        _avatar = new QtMaterialAvatar;
        avatarLayout->addWidget(_avatar);

        QVBoxLayout *inputLayout = new QVBoxLayout;
        _username = new QtMaterialAutoComplete;
        _password = new QtMaterialTextField;
        inputLayout->addWidget(_username);
        inputLayout->addWidget(_password);

        avatarLayout->addLayout(inputLayout);
        layout->addLayout(avatarLayout);

        QHBoxLayout *buttonLayout = new QHBoxLayout;
        _login = new QtMaterialRaisedButton("Login");
        _register = new QtMaterialRaisedButton("Register");
        buttonLayout->addWidget(_login);
        buttonLayout->addWidget(_register);
        layout->addLayout(buttonLayout);

        // window

    }

    void LoginWindow::initConnect()
    {
        connect(_login, &QtMaterialRaisedButton::clicked, [this] {
            qDebug() << "Login";
        });

        connect(_register, &QtMaterialRaisedButton::clicked, [this] {
            qDebug() << "Register";
        });
    }
} // namespace MINIOICQ