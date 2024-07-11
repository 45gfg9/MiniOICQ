#include "login_model.h"

namespace MINIOICQ
{

LoginModel::LoginModel(QObject* parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{
    setTable("localUser");
    this->select();
    this->setEditStrategy(QSqlTableModel::OnFieldChange);
}

LoginModel::~LoginModel() {}

} // namespace MINIOICQ