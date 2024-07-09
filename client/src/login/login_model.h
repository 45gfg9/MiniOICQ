#ifndef LOGIN_MODEL_H
#define LOGIN_MODEL_H

#include <QByteArray>
#include <QDir>
#include <QSqlTableModel>
#include <QString>
#include <QtSql>

namespace MINIOICQ {

class LoginModel: public QSqlTableModel {
    public:
    LoginModel (QObject* parent = nullptr, QSqlDatabase db = QSqlDatabase ());
    ~LoginModel ();

    private:
};

} // namespace MINIOICQ

#endif // LOGIN_MODEL_H