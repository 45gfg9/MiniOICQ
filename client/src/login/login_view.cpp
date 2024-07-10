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
    // connect (_userId, &QtMaterialAutoComplete::textChanged, [this] (const QString& text) {
    //     qDebug () << "[LoginView._userId] textChanged: " << text;
    //     _mapper->model ()->setData (
    //     _mapper->model ()->index (_mapper->currentIndex (), _model->userIdColumn()), text);
    // });
    connect (_userId, &QtMaterialAutoComplete::itemSelected, [this] (const QString& text) {
        qDebug () << "[LoginView._userId] itemSelected: " << text;
        auto match = _mapper->model ()->match (
        _mapper->model ()->index (0, _model->userIdColumn ()), Qt::DisplayRole,
        text, 1, Qt::MatchExactly);
        if (!match.empty ()) {
            _mapper->setCurrentIndex (match.first ().row ());
            QImage avatar;
            avatar.loadFromData (_mapper->model ()
                                 ->data (_mapper->model ()->index (
                                 _mapper->currentIndex (), _model->avatarColumn ()))
                                 .toByteArray ());
            _avatar->setImage (avatar);
        }
    });

    // password edit
    connect (_password, &QtMaterialTextField::textChanged, [this] (const QString& text) {
        qDebug () << "[LoginView._password] textChanged: " << text;
        _mapper->model ()->setData (
        _mapper->model ()->index (_mapper->currentIndex (), _model->passwordColumn ()), text);
    });

    // login click
    connect (
    _login, &QtMaterialRaisedButton::clicked, [this] { qDebug () << "Login"; });

    // register click
    connect (_register, &QtMaterialRaisedButton::clicked,
    [this] { qDebug () << "Register"; });
}

void LoginView::setModel (LoginProxyModel* model) {
    _model  = model;
    _mapper = new QDataWidgetMapper (this);
    _mapper->setModel (_model);
    _mapper->addMapping (_userId, _model->userIdColumn ());
    _mapper->addMapping (_password, _model->passwordColumn ());
    _mapper->addMapping (_avatar, _model->avatarColumn ());
    _mapper->toFirst ();

    on_model_dataChanged (_model->index (0, 0),
    _model->index (_model->rowCount () - 1, 0), QVector<int>{ Qt::DisplayRole });
}

void LoginView::on_model_dataChanged (const QModelIndex& topLeft,
const QModelIndex& /* bottomRight */,
const QVector<int>& roles) {
    qDebug () << "LoginView::on_model_dataChanged";
    if (roles.contains (Qt::DisplayRole)) {
        _mapper->setCurrentIndex (topLeft.row ());
        QStringList userIds;
        for (int i = 0; i < _model->rowCount (); i++) {
            userIds << _model->data (_model->index (i, _model->userIdColumn ())).toString ();
        }
        _userId->setDataSource (userIds);
    }
}

} // namespace MINIOICQ