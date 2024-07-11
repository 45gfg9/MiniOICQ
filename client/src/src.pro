TEMPLATE = app
QT += core gui widgets sql
CONFIG += c++11 debug
HEADERS = \
    login/login_view.h \
    login/login_proxymodel.h \
    login/login_model.h \

SOURCES = \
    main.cpp \
    login/login_view.cpp \
    login/login_proxymodel.cpp \
    login/login_model.cpp \

RESOURCES += \
    resources.qrc

LIBS += ../third_party/qt-material-widgets/components/libcomponents.a
INCLUDEPATH += ../third_party/qt-material-widgets/components/
TARGET = client.exe
PRE_TARGETDEPS += ../third_party/qt-material-widgets/components/libcomponents.a
