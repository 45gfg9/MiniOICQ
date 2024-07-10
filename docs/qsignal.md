# Qt 信号与槽

```cpp
QObject::connect(sender, SIGNAL(signal()), receiver, SLOT(slot()));
```

- 一个信号可以连接多个槽，多个信号可以连接同一个槽，一个信号可以连接另一个信号。
- 使用信号和槽的类的定义中必须加入 `Q_OBJECT`。

## 事件

除了信号与槽，事件在 Qt Widget 中是重要的机制。

事件发生时，Qt 构造事件对象，调用对应目标（通常是 Widget）的 `event()` 函数。


