#include "login_model.h"

namespace MINIOICQ {

LoginModel::LoginModel (QObject* parent, QSqlDatabase db) : QSqlTableModel (parent, db) {
    setTable ("users");
}

LoginModel::~LoginModel () {
}

} // namespace MINIOICQ