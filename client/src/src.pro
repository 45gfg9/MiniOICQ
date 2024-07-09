TEMPLATE = app
QT += core gui widgets
CONFIG += c++11
HEADERS = \
    login/login_window.h
SOURCES = \
    main.cpp \
    login/login_window.cpp
LIBS += ../third_party/qt-material-widgets/components/libcomponents.a
INCLUDEPATH += ../third_party/qt-material-widgets/components/
TARGET = client
PRE_TARGETDEPS += ../third_party/qt-material-widgets/components/libcomponents.a
