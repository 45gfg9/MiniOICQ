# Qt 数据库

```cpp
QSqlDatabse DB;
DB = QSqlDatabase::addDatabase("QSQLITE");
DB.setDatabaseName("test.db");
DB.open();
QSqlTableModel model(nullptr, DB);
model.setTable("table");
model.setEditStrategy(QSqlTableModel::OnManualSubmit);
model.select();
model.setHeaderData(0, Qt::Horizontal, "ID");
```
