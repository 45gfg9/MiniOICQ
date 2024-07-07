#include "login_vm.h"

LoginVM::LoginVM(QObject *parent = nullptr) : QObject(parent), model(new LoginModel()) {
    users = model->getUsers();
}

LoginVM::~LoginVM() {
    delete model;
    for (UserInfo *user : *users) {
        delete user;
    }
    delete users;
}

void LoginVM::login() {
    // Get the username and password from the UI
    QString username = currentUser.getUsername();
    QString password = currentUser.getPassword();

    // Send request to the server
    // ...
}

void LoginVM::registerUser() {
    // Get the username and password from the UI
    QString username = currentUser.getUsername();
    QString password = currentUser.getPassword();

    // Send request to the server
    // ...
}