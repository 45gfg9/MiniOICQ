#ifndef USER_INFO_H
#define USER_INFO_H

#include <QString>

class UserInfo {
public:
  UserInfo() {}
  UserInfo(QString username, QString password, QByteArray icon) : username(username), password(password), icon(icon) {}
  ~UserInfo() {}

  QString getUsername() { return username; }
  void setUsername(QString username) { this->username = username; }
  QString getPassword() { return password; }
  void setPassword(QString password) { this->password = password; }
  QByteArray getIcon() { return icon; }
  void setIcon(QByteArray icon) { this->icon = icon; }

private:
  QString username;
  QString password;
  QByteArray icon;
};

#endif // USER_INFO_H