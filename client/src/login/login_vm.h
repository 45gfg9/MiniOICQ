#ifndef LOGIN_VM_H
#define LOGIN_VM_H

#include <QObject>
#include <QVector>

#include "login/user_info.h"
#include "login/login_model.h"

class LoginVM : public QObject {
  Q_OBJECT

public:
  LoginVM(QObject *parent = nullptr);
	~LoginVM();
	UserInfo &getCurrentUser() { return currentUser; }

public slots:
	void login();
	void registerUser();

private:
	QVector<UserInfo *> *users;
	UserInfo currentUser;
	LoginModel *model;
};

#endif // LOGIN_VM_H