greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = minioicq_client
TEMPLATE = subdirs
SUBDIRS = third_party src
CONFIG += ordered

DEFINES += QT_DEPRECATED_WARNINGS
