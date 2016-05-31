
QT += core script
QT -= gui

CONFIG += c++11

TEMPLATE = lib
TARGET = QtScriptPluginExample

INCLUDEPATH += \
    ../../../ScriptEmbedder/include \
    ../QtScriptExample

HEADERS += \
    qtscriptplugin.hh \
    qtscriptinterpreter.hh \
    qtscriptapiadapter.hh

SOURCES += \
    qtscriptplugin.cc \
    qtscriptinterpreter.cc \
    qtscriptapiadapter.cc
