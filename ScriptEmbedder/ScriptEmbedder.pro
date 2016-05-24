
QT += core
QT -= gui

CONFIG += c++11

TEMPLATE = lib
TARGET = ScriptEmbedder

INCLUDEPATH += include

HEADERS += \
    include/configuration.hh \
    include/scriptapi.hh

SOURCES += \
    src/configuration.cc
