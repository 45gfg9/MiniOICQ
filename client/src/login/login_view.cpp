#include "login_view.h"
#include "login_viewmodel.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QVBoxLayout>
#include "common/misc.h"

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
     *          - QtMaterialRaisedButton: reg
     */

    _avatar = new QtMaterialAvatar(this);
    _userName = new QtMaterialTextField(this);
    _userId = new QtMaterialAutoComplete(this);
    _password = new QtMaterialTextField(this);
    _login = new QtMaterialRaisedButton("Login");
    _reg = new QtMaterialRaisedButton("Register");
    _snackbar = new QtMaterialSnackbar(this);
    QLabel* title = new QLabel(this);

    // arrange
    QVBoxLayout* layout = new QVBoxLayout(this);
    QHBoxLayout* avatarLayout = new QHBoxLayout;
    QVBoxLayout* inputLayout = new QVBoxLayout;
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    inputLayout->addWidget(_userName);
    inputLayout->addWidget(_userId);
    inputLayout->addWidget(_password);
    avatarLayout->addWidget(_avatar);
    avatarLayout->addLayout(inputLayout);
    buttonLayout->addWidget(_login);
    buttonLayout->addWidget(_reg);
    layout->addWidget(title);
    layout->addLayout(avatarLayout);
    layout->addLayout(buttonLayout);

    // style
    this->setPalette(themePalette);
    this->setAttribute(Qt::WA_DeleteOnClose);
    // layout
    layout->setSpacing(30);
    layout->setContentsMargins(30, 30, 30, 30); // left, top, right, bottom
    // title
    title->setPixmap(
        QIcon(":/minioicq.svg").pixmap(QSize(300, 300)).scaledToWidth(300));
    // avatar
    _avatar->setSize(150);
    // inputLayout
    inputLayout->setSpacing(5);
    // inputs
    _userName->setLabel("User Name");
    _userId->setLabel("User Id");
    _password->setLabel("Password");
    _password->setEchoMode(QLineEdit::Password);
    // button
    _login->setBackgroundColor(QColor(0, 188, 212));
    _reg->setBackgroundColor(QColor(0, 188, 212));
    buttonLayout->setSpacing(5);
}

void LoginView::initConnect()
{
    // component
    connect(_userId, &QtMaterialAutoComplete::itemSelected, this,
            &LoginView::on_userId_itemSelected);
    connect(_login, &QtMaterialRaisedButton::clicked, this,
            &LoginView::on_login_clicked);
    connect(_reg, &QtMaterialRaisedButton::clicked, this,
            &LoginView::on_reg_clicked);
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
    connect(loginViewModel, &LoginViewModel::loginSuccess, this,
            &LoginView::on_loginSuccess);
    connect(loginViewModel, &LoginViewModel::loginFailed, this,
            &LoginView::on_loginFailed);
    connect(loginViewModel, &LoginViewModel::regSuccess, this,
            &LoginView::on_regSuccess);
    connect(loginViewModel, &LoginViewModel::regFailed, this,
            &LoginView::on_regFailed);
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

void LoginView::on_reg_clicked()
{
    qDebug() << "LoginView::on_reg_clicked";
    QString userName = _userName->text();
    QString password = _password->text();
    emit reg(userName, password);
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

void LoginView::on_loginSuccess()
{
    qDebug() << "LoginView::loginSuccess";
    _snackbar->addMessage("Login Success");
    accept();
}
void LoginView::on_loginFailed(QString message)
{
    qDebug() << "LoginView::loginFailed";
    _snackbar->addMessage(message);
}
void LoginView::on_regSuccess()
{
    qDebug() << "LoginView::regSuccess";
    _snackbar->addMessage("Register Success");
    accept();
}
void LoginView::on_regFailed(QString message)
{
    qDebug() << "LoginView::registerFailed";
    _snackbar->addMessage(message);
}

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
        else if (_reg->hasFocus())
        {
            on_reg_clicked();
        }
    }
}

void bindLoginView(LoginView* loginView, QAbstractItemModel* loginViewModel)
{
    loginView->setModel(loginViewModel);
};

} // namespace MINIOICQ
