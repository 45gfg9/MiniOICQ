#include "login_view.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace MINIOICQ {
LoginView::LoginView (QWidget* parent) : QDialog (parent) {
    initUI ();
    initConnect ();
}

LoginView::~LoginView () {
}

void LoginView::initUI () {
    // components
    QVBoxLayout* layout = new QVBoxLayout;
    setLayout (layout);

    _title = new QLabel ("Login");
    layout->addWidget (_title);

    QHBoxLayout* avatarLayout = new QHBoxLayout;
    _avatar                   = new QtMaterialAvatar;
    avatarLayout->addWidget (_avatar);

    QVBoxLayout* inputLayout = new QVBoxLayout;
    _userId                  = new QtMaterialAutoComplete;
    _password                = new QtMaterialTextField;
    inputLayout->addWidget (_userId);
    inputLayout->addWidget (_password);

    avatarLayout->addLayout (inputLayout);
    layout->addLayout (avatarLayout);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    _login                    = new QtMaterialRaisedButton ("Login");
    _register                 = new QtMaterialRaisedButton ("Register");
    buttonLayout->addWidget (_login);
    buttonLayout->addWidget (_register);
    layout->addLayout (buttonLayout);

    // window
}

void LoginView::initConnect () {
    // userid select or edit
    connect (_userId, &QtMaterialAutoComplete::textChanged, [this] (const QString& text) {
        qDebug () << "[LoginView._userId] textChanged: " << text;
        _mapper->model ()->setData (
        _mapper->model ()->index (_mapper->currentIndex (), _userIdColumn), text);
    });
    connect (_userId, &QtMaterialAutoComplete::itemSelected, [this] (const QString& text) {
        qDebug () << "[LoginView._userId] itemSelected: " << text;
        _mapper->model ()->setData (
        _mapper->model ()->index (_mapper->currentIndex (), _userIdColumn), text);
    });

    // password edit
    connect (_password, &QtMaterialTextField::textChanged, [this] (const QString& text) {
        qDebug () << "[LoginView._password] textChanged: " << text;
        _mapper->model ()->setData (
        _mapper->model ()->index (_mapper->currentIndex (), _passwordColumn), text);
    });

    // login click
    connect (
    _login, &QtMaterialRaisedButton::clicked, [this] { qDebug () << "Login"; });

    // register click
    connect (_register, &QtMaterialRaisedButton::clicked,
    [this] { qDebug () << "Register"; });
}

void LoginView::setModel (QAbstractItemModel* model) {
    _model  = model;
    _mapper = new QDataWidgetMapper (this);
    _mapper->setModel (_model);
    // find user id, password, avatar column
    for (int i = 0; i < _model->columnCount (); i++) {
        if (_model->headerData (i, Qt::Horizontal).toString () == "userId") {
            _userIdColumn = i;
        } else if (_model->headerData (i, Qt::Horizontal).toString () == "password") {
            _passwordColumn = i;
        } else if (_model->headerData (i, Qt::Horizontal).toString () == "avatar") {
            _avatarColumn = i;
        }
    }
    _mapper->addMapping (_userId, _userIdColumn);
    _mapper->addMapping (_password, _passwordColumn);
    _mapper->addMapping (_avatar, _avatarColumn);
    _mapper->toFirst ();
}
} // namespace MINIOICQ