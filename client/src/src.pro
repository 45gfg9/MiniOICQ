TEMPLATE = app
QT += core gui widgets sql websockets
CONFIG += c++11 debug
HEADERS = \
    login/login_view.h \
    login/login_viewmodel.h \
    login/login_model.h \
    websocket/websocket.h \
    list/list_view.h \
    list/list_viewmodel.h \
    list/list_model.h \

SOURCES = \
    main.cpp \
    login/login_view.cpp \
    login/login_viewmodel.cpp \
    login/login_model.cpp \
    websocket/websocket.cpp \
    list/list_view.cpp \
    list/list_viewmodel.cpp \
    list/list_model.cpp \

RESOURCES += \
    resources.qrc
 
DEFINES += MSGPACK_NO_BOOST
LIBS += ../third_party/qt-material-widgets/components/libcomponents.a
INCLUDEPATH += \
    ../third_party/qt-material-widgets/components/ \
    ../third_party/qt-material-widgets/components/lib \
    ../third_party/msgpack/include \

TARGET = client.exe
PRE_TARGETDEPS += ../third_party/qt-material-widgets/components/libcomponents.a
