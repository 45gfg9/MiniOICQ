#ifndef LOGIN_MODEL_H
#define LOGIN_MODEL_H

#include <QString>
#include <QVector>
#include <QByteArray>
#include <QtSql>
#include <QDir>

#include "login/user_info.h"

class LoginModel {
public:
  LoginModel();
  ~LoginModel();

  QVector<UserInfo *> *getUsers();
  void addUser(UserInfo *user);

private:
  QSqlDatabase db;
};

#endif // LOGIN_MODEL_H