#ifndef LIST_MODEL_H
#define LIST_MODEL_H

#include <QSqlTableModel>

namespace MINIOICQ
{

class ListModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    ListModel(QObject* parent = nullptr, QSqlDatabase db);
    ~ListModel();

Q_SIGNALS:
    void message_received();

public slots:
    void on_message_received();

private:
    void refresh();
};

} // namespace MINIOICQ

#endif // LIST_MODEL_H