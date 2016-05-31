#-------------------------------------------------
#
# Project created by QtCreator 2016-05-31T13:03:51
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = QtScriptExample
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ScriptEmbedder/release/ -lScriptEmbedder
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ScriptEmbedder/debug/ -lScriptEmbedder
else:unix: LIBS += -L$$OUT_PWD/../../../ScriptEmbedder/ -lScriptEmbedder

INCLUDEPATH += \
    ../../../ScriptEmbedder/include

SOURCES += main.cc \
    myscriptapiimplementation.cc

HEADERS += \
    myscriptapi.hh \
    myscriptapiimplementation.hh
