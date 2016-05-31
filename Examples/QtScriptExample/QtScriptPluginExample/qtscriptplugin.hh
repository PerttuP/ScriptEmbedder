/**
 * @file
 * @brief Defines example plugin for QtScript.  Used in QtScriptExample.
 * @author Perttu Paarlahti 2016.
 */

#ifndef QTSCRIPTPLUGIN_HH
#define QTSCRIPTPLUGIN_HH

#include <QObject>
#include "interpreterplugin.hh"


/**
 * @brief Implements the InterpreterPlugin interface. Language: QtScript.
 */
class QtScriptPlugin : public QObject, ScriptEmbedderNS::InterpreterPlugin
{
    Q_OBJECT
    Q_INTERFACES(ScriptEmbedderNS::InterpreterPlugin)
    Q_PLUGIN_METADATA(IID INTERPRETER_PLUGIN_IID)

public:

    QtScriptPlugin();
    virtual ~QtScriptPlugin();
    QString language() const;
    ScriptEmbedderNS::ScriptInterpreter* getInstance() const;
};

#endif // QTSCRIPTPLUGIN_HH
