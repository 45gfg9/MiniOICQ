# Qt 项目与构建工具

Qt 项目使用 `.pro` 文件来描述项目，通过 `qmake` 工具生成 Makefile 或 Visual Studio 工程文件等。

## 项目文件

`.pro` 文件的简单语法如下：

```text
# Comment
include(other.pro)
HEADER += myclass.h
HEADERS = mainwindow.h paintwidget.h
SOURCE += main.cpp
SOURCE += myclass.cpp
SOURCES = main.cpp mainwindow.cpp \
          paintwidget.cpp
CONFIG += console
CONFIG += debug
TEMP_SOURCES = $$SOURCES
DEST = "Program Files"
win32 {
    SOURCES += hellowin.cpp
}
unix {
    SOURCES += hellounix.cpp
}
!exists( main.cpp ) {
    error( "No main.cpp file found" )
}
CONFIG(opengl) {
    message(Building with OpenGL support.)
} else {
    message(OpenGL support is not available.)
}
```

几个重要变量：

- `TEMPLATE`：项目类型
    - `app`、`lib`
    - `subdirs`：用 `SUBDIRS` 变量指定子项目，子项目必须有自己的 `.pro` 文件
- `QT`：Qt 模块

## 构建项目

使用 `qmake` 生成相关文件：

```bash
qmake -o Makefile myproject.pro
qmake -tp vc myproject.pro
```