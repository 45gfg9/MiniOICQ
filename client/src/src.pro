TEMPLATE = app
QT += core gui widgets sql websockets multimedia
CONFIG += c++11 debug
HEADERS = \
    login/login_view.h \
    login/login_viewmodel.h \
    login/login_model.h \
    websocket/websocket.h \
    list/list_view.h \
    list/list_viewmodel.h \
    list/list_model.h \
    chat/chat_view.h \
    chat/chat_viewmodel.h \
    chat/chat_model.h \
    chat/chat_manager.h \
    common/user_info.h \
    common/chat_info.h \
    common/message.h \
    common/misc.h \

SOURCES = \
    main.cpp \
    login/login_view.cpp \
    login/login_viewmodel.cpp \
    login/login_model.cpp \
    websocket/websocket.cpp \
    list/list_view.cpp \
    list/list_viewmodel.cpp \
    list/list_model.cpp \
    chat/chat_view.cpp \
    chat/chat_viewmodel.cpp \
    chat/chat_model.cpp \
    chat/chat_manager.cpp \
    common/misc.cpp \

RESOURCES += \
    resources.qrc

DEFINES += MSGPACK_NO_BOOST
LIBS += ../third_party/qt-material-widgets/components/libcomponents.a
INCLUDEPATH += \
    ../third_party/qt-material-widgets/components/ \
    ../third_party/qt-material-widgets/components/lib \

TARGET = client.exe
PRE_TARGETDEPS += ../third_party/qt-material-widgets/components/libcomponents.a
