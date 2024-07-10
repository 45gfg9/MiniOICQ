#include "login_view.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace MINIOICQ {

void LoginDelegate::paint (QPainter* painter,
const QStyleOptionViewItem& option,
const QModelIndex& index) const {
    qDebug () << "LoginDelegate::paint";
    QStyledItemDelegate::paint (painter, option, index);
}

QSize LoginDelegate::sizeHint (const QStyleOptionViewItem& option,
const QModelIndex& index) const {
    qDebug () << "LoginDelegate::sizeHint";
    return QStyledItemDelegate::sizeHint (option, index);
}

void LoginDelegate::setEditorData (QWidget* editor, const QModelIndex& index) const {
    qDebug () << "LoginDelegate::setEditorData";
    if (editor->metaObject ()->className () == QString ("QtMaterialAvatar")) {
        QImage avatar;
        avatar.loadFromData (index.data (Qt::DisplayRole).toByteArray ());
        qobject_cast<QtMaterialAvatar*> (editor)->setImage (avatar);
    } else {
        QStyledItemDelegate::setEditorData (editor, index);
    }
    qDebug () << "Done";
}

void LoginDelegate::setModelData (QWidget* editor,
QAbstractItemModel* model,
const QModelIndex& index) const {
    qDebug () << "LoginDelegate::setModelData";
    QStyledItemDelegate::setModelData (editor, model, index);
}


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

    QLabel* title = new QLabel ("Login");
    layout->addWidget (title);

    QHBoxLayout* avatarLayout = new QHBoxLayout;
    _avatar                   = new QtMaterialAvatar;
    _avatar->setSize (150);
    avatarLayout->addWidget (_avatar);

    // QGridLayout* inputLayout = new QGridLayout;
    // QLabel* userName         = new QLabel ("User Name");
    // QLabel* userId           = new QLabel ("User Id");
    // QLabel* password         = new QLabel ("Password");
    QVBoxLayout* inputLayout = new QVBoxLayout;
    _userName = new QtMaterialTextField;
    _userName->setLabel ("User Name");
    _userId   = new QtMaterialAutoComplete;
    _userId->setLabel ("User Id");
    _password = new QtMaterialTextField;
    _password->setLabel ("Password");
    _password->setEchoMode (QLineEdit::Password);
    inputLayout->addWidget (_userName);
    inputLayout->addWidget (_userId);
    inputLayout->addWidget (_password);
    // inputLayout->addWidget (userName, 0, 0);
    // inputLayout->addWidget (_userName, 0, 1);
    // inputLayout->addWidget (userId, 1, 0);
    // inputLayout->addWidget (_userId, 1, 1);
    // inputLayout->addWidget (password, 2, 0);
    // inputLayout->addWidget (_password, 2, 1);

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
    connect (_userId, &QtMaterialAutoComplete::itemSelected, [this] (const QString& text) {
        qDebug () << "[LoginView._userId] itemSelected: " << text;
        auto match = _mapper->model ()->match (
        _mapper->model ()->index (0, _model->userIdColumn ()), Qt::DisplayRole,
        text, 1, Qt::MatchExactly);
        if (!match.empty ()) {
            qDebug () << "match: " << match.first ().row ();
            _mapper->setCurrentIndex (match.first ().row ());
            QImage avatar;
            avatar.loadFromData (_mapper->model ()
                                 ->data (_mapper->model ()->index (
                                 _mapper->currentIndex (), _model->avatarColumn ()))
                                 .toByteArray ());
            _avatar->setImage (avatar);
        }
        _mapper->submit ();
    });

    // login click
    connect (
    _login, &QtMaterialRaisedButton::clicked, [this] { qDebug () << "Login"; });

    // register click
    connect (_register, &QtMaterialRaisedButton::clicked,
    [this] { qDebug () << "Register"; });
}

void LoginView::setModel (LoginProxyModel* model) {

    // create QStringListModel for AutoComplete
    _userIdModel = new QStringListModel (this);

    _model  = model;
    _mapper = new QDataWidgetMapper (this);
    _mapper->setModel (_model);
    // SubmitPolicy:
    // - AutoSubmit: submit data when widget loses focus
    // - ManualSubmit: submit data manually
    _mapper->setSubmitPolicy (QDataWidgetMapper::ManualSubmit);
    _mapper->setItemDelegate (new LoginDelegate (this));
    _mapper->addMapping (_userId, _model->userIdColumn ());
    _mapper->addMapping (_userName, _model->userNameColumn (), "text");
    _mapper->addMapping (_password, _model->passwordColumn ());
    _mapper->addMapping (_avatar, _model->avatarColumn ());
    _mapper->toFirst ();

    // Debug: print _mapper current row
    qDebug () << _mapper->currentIndex ();
    for (int i = 0; i < _mapper->model ()->columnCount () - 1; i++) {
        qDebug () << _mapper->model ()->data (
        _mapper->model ()->index (_mapper->currentIndex (), i), Qt::DisplayRole);
    }

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