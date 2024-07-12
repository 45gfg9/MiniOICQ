#ifndef USER_INFO_H
#define USER_INFO_H

#include <QString>
#include <QImage>

namespace MINIOICQ {

/**
 * @brief Auxiliary class to store user information to be used in websocket communication.
 * 
 */
class UserInfo {
public:
    UserInfo() = default;
    UserInfo(QString _userId, QString _username, QString _password, QImage _avatar)
    : _userId(_userId), _username(_username), _password(_password), _avatar(_avatar) {}
    
    // data
    QString userId() const { return _userId; }
    QString username() const { return _username; }
    QString password() const { return _password; }
    QImage avatar() const { return _avatar; }

    // set data
    void setUserId(QString userId) { _userId = userId; }
    void setUsername(QString username) { _username = username; }
    void setPassword(QString password) { _password = password; }
    void setAvatar(QImage avatar) { _avatar = avatar; }

private:
    QString _userId;
    QString _username;
    QString _password;
    QImage _avatar;
};

} // namespace MINIOICQ

#endif // USER_INFO_H