#-------------------------------------------------
#
# Project created by QtCreator 2016-05-31T13:03:51
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = QtScriptExample
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../../ScriptEmbedder/include

SOURCES += main.cc \
    myscriptapiimplementation.cc

HEADERS += \
    myscriptapi.hh \
    myscriptapiimplementation.hh
