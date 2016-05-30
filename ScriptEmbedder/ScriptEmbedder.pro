
QT += core
QT -= gui

CONFIG += c++11

TEMPLATE = lib
TARGET = ScriptEmbedder

INCLUDEPATH += include src

HEADERS += \
    include/configuration.hh \
    include/scriptapi.hh \
    include/scriptembedder.hh \
    include/logger.hh \
    include/interpreterplugin.hh \
    src/serialscriptembedder.hh \
    src/interpreterloader.hh \
    include/scriptinterpreter.hh \
    include/scriptembedderbuilder.hh

SOURCES += \
    src/configuration.cc \
    src/serialscriptembedder.cc \
    src/interpreterloader.cc \
    include/scriptembedderbuilder.cc
