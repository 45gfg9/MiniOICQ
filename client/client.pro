TARGET = minioicq_client
TEMPLATE = subdirs
SUBDIRS = third_party src
CONFIG += ordered 
CONFIG += debug sanitizer sanitize_address
DEFINES += QT_DEPRECATED_WARNINGS
