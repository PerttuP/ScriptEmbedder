#-------------------------------------------------
#
# Project created by QtCreator 2016-05-25T02:07:32
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_configurationtest
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../ScriptEmbedder/include

SOURCES += tst_configurationtest.cc \
           ../../ScriptEmbedder/src/configuration.cc

DEFINES += SRCDIR=\\\"$$PWD/\\\"
