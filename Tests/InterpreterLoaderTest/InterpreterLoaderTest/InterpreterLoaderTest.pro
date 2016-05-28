#-------------------------------------------------
#
# Project created by QtCreator 2016-05-28T17:12:44
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_interpreterloadertest
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../../ScriptEmbedder/src \
               ../../../ScriptEmbedder/include

SOURCES += tst_interpreterloadertest.cc \
           ../../../ScriptEmbedder/src/interpreterloader.cc \
           ../../../ScriptEmbedder/src/configuration.cc


DEFINES += SRCDIR=\\\"$$PWD/\\\"
