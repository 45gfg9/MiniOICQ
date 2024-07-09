# Qt Model/View 结构

## 相互作用

Qt 设计基于下图所示的 Model/View 结构：

![image-20240709215620382](qtmodel.assets/image-20240709215620382.png)

它们之间的相互作用机制如下：

- 源数据发生变化时，Model 发射信号通知 View。
- 用户在界面上操作数据时，View 发射信号表示 Command。
- 数据被编辑时，Delegate 发射信号告知 Model 和 View 编辑器的状态。

如果要适配课上所说的 MVVM 模型，只需把 Qt 中的 View 视为 VM 即可。

## Model 类

Qt 提供以下 `QAbstractItemModel` 类：

![image-20240709214928642](qtmodel.assets/image-20240709214928642.png)

Model 中数据的基本单元是 item，它具有 row、column 和 parent item 三个参数。通过这三个参数，可以构造 List、Table、Tree 等组织形式的 Model。

![image-20240709220640175](qtmodel.assets/image-20240709220640175.png)

`QAbstractItemModel` 的子类都以表格的层次结构表示数据，View 通过这种规则来存取 Model 中的数据。`QModelIndex` 表示 Model 索引，提供一个临时指针，用来通过 Model 提取或修改数据，如下所示：

```cpp
// QModelIndex() 表示 root item
QModelIndex index = model->index(0, 0, QModelIndex());
```

一个 item 具有多种 role，用于告知视图和代理如何显示数据。比如：

- `Qt::DisplayRole`：（标准）在 View 中显示的字符串
- `Qt::ToolTipRole`：鼠标悬停时显示的字符串
- `Qt::UserRole`：用户自定义的 role，用于在 Model 中存储额外的数据

role 是 `Qt::ItemDataRole` 枚举类型。存取 item 的数据时都需要指定 role：

```cpp
// data 函数的定义
QVariant QStandardItem::date(int role = Qt::UserRole + 1) const
// setData 函数的定义
void QStandardItem::setData(const QVariant &value, int role = Qt::UserRole + 1)
```

## View 类

Qt 提供以下 `QAbstractItemView` 类：

![image-20240709215103780](qtmodel.assets/image-20240709215103780.png)

调用 View 的 `setModel()` 函数可以将其关联到一个 Model。在 View 上的修改自动保存到关联的 Model。 

## Delegate 类

Delegate 在 View 上为编辑数据提供编辑器。

Qt 提供 `QAbstractItemDelegate` 作为所有代理类的基类，其子类 `QStyleItemDelegate` 是 View 默认使用的代理类，一般从它继承创建自定义代理类。

![image-20240709222315499](qtmodel.assets/image-20240709222315499.png)

