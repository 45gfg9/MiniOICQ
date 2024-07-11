#ifndef LIST_MODEL_H
#define LIST_MODEL_H

#include <QSqlTableModel>

namespace MINIOICQ
{

class ListModel : public QSqlTableModel
{
    Q_OBJECT

public:
    ListModel(QObject* parent = nullptr, QSqlDatabase db = QSqlDatabase());
    ~ListModel();

private:
};

} // namespace MINIOICQ

#endif // LIST_MODEL_H