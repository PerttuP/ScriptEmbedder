#-------------------------------------------------
#
# Project created by QtCreator 2016-05-29T19:44:44
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_serialscriptembeddertest
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    ../../../ScriptEmbedder/include \
    ../../../ScriptEmbedder/src \

DEPENDPATH += \
    ../../../ScriptEmbedder/src \
    ../../../ScriptEmbedder/include

SOURCES += \
    tst_serialscriptembeddertest.cc \
    ../../../ScriptEmbedder/src/serialscriptembedder.cc \
    ../../../ScriptEmbedder/src/configuration.cc \
    ../../../ScriptEmbedder/src/interpreterloader.cc \


DEFINES += SRCDIR=\\\"$$PWD/\\\"
