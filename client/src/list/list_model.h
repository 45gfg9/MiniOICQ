#ifndef LIST_MODEL_H
#define LIST_MODEL_H

#include <QSqlTableModel>

namespace MINIOICQ
{

class ListModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    void setDatabase(QSqlDatabase db);
    QSqlDatabase database() const { return _db; }

Q_SIGNALS:
    void message_received();

public slots:
    void on_message_received();

private:
    void refresh();

    QSqlDatabase _db;
};

} // namespace MINIOICQ

#endif // LIST_MODEL_H