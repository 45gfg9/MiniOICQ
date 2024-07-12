#include "login_view.h"
#include "login_viewmodel.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QVBoxLayout>

namespace MINIOICQ
{

void LoginDelegate::setEditorData(QWidget* editor,
                                  const QModelIndex& index) const
{
    // avatar
    if (editor->metaObject()->className() == QString("QtMaterialAvatar"))
    {
        QImage avatar;
        avatar.loadFromData(index.data(Qt::DisplayRole).toByteArray());
        qobject_cast<QtMaterialAvatar*>(editor)->setImage(avatar);
    }
    // autocomplete
    else if (editor->metaObject()->className() ==
             QString("QtMaterialAutoComplete"))
    {
        QStringList userIds;
        auto* model = qobject_cast<const LoginViewModel*>(index.model());
        for (int i = 0; i < model->rowCount(); i++)
        {
            userIds << model->data(model->index(i, model->userIdColumn()))
                           .toString();
        }
        qobject_cast<QtMaterialAutoComplete*>(editor)->setDataSource(userIds);
        QStyledItemDelegate::setEditorData(editor, index);
    }
    else
    {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void LoginDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                                 const QModelIndex& index) const
{
    // avatar
    if (editor->metaObject()->className() == QString("QtMaterialAvatar"))
    {
        QImage avatar = qobject_cast<QtMaterialAvatar*>(editor)->image();
        model->setData(index, QByteArray::fromRawData(
                                  reinterpret_cast<const char*>(avatar.bits()),
                                  avatar.sizeInBytes()));
    }
    else
    {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

LoginView::LoginView(QWidget* parent) : QDialog(parent)
{
    initUI();
    initConnect();
}

LoginView::~LoginView() {}

void LoginView::initUI()
{
    /**
     * @brief
     *
     * - QVBoxLayout
     *      - QLabel: title
     *      - QHBoxLayout: avatarLayout
     *          - QtMaterialAvatar: avatar
     *          - QVBoxLayout: inputLayout
     *              - QtMaterialTextField: userName
     *              - QtMaterialAutoComplete: userId
     *              - QtMaterialTextField: password
     *     - QHBoxLayout: buttonLayout
     *          - QtMaterialRaisedButton: login
     *          - QtMaterialRaisedButton: register
     */
    // components
    QVBoxLayout* layout = new QVBoxLayout;
    qDebug() << layout->spacing();
    layout->setSpacing(30);
    layout->setContentsMargins(30, 30, 30, 30); // left, top, right, bottom
    setLayout(layout);

    QLabel* title = new QLabel();
    title->setPixmap(
        QIcon(":/minioicq.svg").pixmap(QSize(300, 300)).scaledToWidth(300));
    layout->addWidget(title);

    QHBoxLayout* avatarLayout = new QHBoxLayout;
    _avatar = new QtMaterialAvatar;
    _avatar->setSize(150);
    avatarLayout->addWidget(_avatar);

    QVBoxLayout* inputLayout = new QVBoxLayout;
    _userName = new QtMaterialTextField;
    _userName->setLabel("User Name");
    _userId = new QtMaterialAutoComplete;
    _userId->setLabel("User Id");
    _password = new QtMaterialTextField;
    _password->setLabel("Password");
    _password->setEchoMode(QLineEdit::Password);
    inputLayout->addWidget(_userName);
    inputLayout->addWidget(_userId);
    inputLayout->addWidget(_password);
    inputLayout->setSpacing(5);
    avatarLayout->addLayout(inputLayout);
    layout->addLayout(avatarLayout);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    _login = new QtMaterialRaisedButton("Login");
    _register = new QtMaterialRaisedButton("Register");
    buttonLayout->addWidget(_login);
    buttonLayout->addWidget(_register);
    buttonLayout->setSpacing(5);
    layout->addLayout(buttonLayout);

    // attributes
    this->setAttribute(Qt::WA_DeleteOnClose);
}

void LoginView::initConnect()
{
    // component
    connect(_userId, &QtMaterialAutoComplete::itemSelected, this,
            &LoginView::on_userId_itemSelected);
    connect(_login, &QtMaterialRaisedButton::clicked, this,
            &LoginView::on_login_clicked);
    connect(_register, &QtMaterialRaisedButton::clicked, this,
            &LoginView::on_register_clicked);
}

void LoginView::setModel(QAbstractItemModel* model)
{
    auto* loginViewModel = qobject_cast<LoginViewModel*>(model);
    _mapper = new QDataWidgetMapper(this);
    _mapper->setModel(model);
    // SubmitPolicy:
    // - AutoSubmit: submit data when widget loses focus
    // - ManualSubmit: submit data manually
    _mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    _mapper->setItemDelegate(new LoginDelegate(this));
    _mapper->addMapping(_userId, loginViewModel->userIdColumn());
    _mapper->addMapping(_userName, loginViewModel->userNameColumn());
    _mapper->addMapping(_password, loginViewModel->passwordColumn());
    _mapper->addMapping(_avatar, loginViewModel->avatarColumn());
    _mapper->toFirst();
    // model
    connect(this, &LoginView::login, loginViewModel, &LoginViewModel::on_login);
    connect(this, &LoginView::reg, loginViewModel, &LoginViewModel::on_reg);
}

void LoginView::on_login_clicked()
{
    qDebug() << "LoginView::on_login_clicked";
    QString userId = _userId->text();
    QString password = _password->text();
    // to LoginModel on_login
    emit login(userId, password);
    // auto res = _mapper->submit();
    // qDebug() << "submit: " << res;
}

void LoginView::on_register_clicked()
{
    qDebug() << "LoginView::on_register_clicked";
    QString userId = _userId->text();
    QString userName = _userName->text();
    QString password = _password->text();
    QImage avatar = _avatar->image();
    emit reg(userId, userName, password, avatar);
}

void LoginView::on_userId_itemSelected(QString userId)
{
    qDebug() << "LoginView::on_userId_itemSelected";
    auto* model = qobject_cast<const LoginViewModel*>(_mapper->model());
    auto match = model->match(model->index(0, model->userIdColumn()),
                              Qt::DisplayRole, userId, 1, Qt::MatchExactly);
    if (!match.empty())
    {
        _mapper->setCurrentIndex(match.first().row());
    }
}

void LoginView::loginSuccess() { qDebug() << "LoginView::loginSuccess"; }
void LoginView::loginFailed() { qDebug() << "LoginView::loginFailed"; }
void LoginView::registerSuccess() { qDebug() << "LoginView::registerSuccess"; }
void LoginView::registerFailed() { qDebug() << "LoginView::registerFailed"; }

/**
 * @brief Override keyPressEvent from QDialog to prevent default button
 *
 * Reference:
 *
 * https://forum.qt.io/topic/119620/pressing-the-enter-key-triggers-unwanted-response-from-a-button/5
 *
 * https://doc.qt.io/qt-5/qdialog.html#default-button
 *
 * @param event
 */
void LoginView::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        if (_login->hasFocus())
        {
            on_login_clicked();
        }
        else if (_register->hasFocus())
        {
            on_register_clicked();
        }
    }
}

void bindLoginView(LoginView* loginView, QAbstractItemModel* loginViewModel)
{
    loginView->setModel(loginViewModel);
};

} // namespace MINIOICQ
