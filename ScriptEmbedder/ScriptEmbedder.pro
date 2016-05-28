
QT += core
QT -= gui

CONFIG += c++11

TEMPLATE = lib
TARGET = ScriptEmbedder

INCLUDEPATH += include

HEADERS += \
    include/configuration.hh \
    include/scriptapi.hh \
    include/scriptembedder.hh \
    include/logger.hh \
    include/interpreterplugin.hh \
    src/serialscriptembedder.hh \
    src/interpreterloader.hh \
    include/scriptinterpreter.hh

SOURCES += \
    src/configuration.cc \
    src/serialscriptembedder.cc \
    src/interpreterloader.cc
