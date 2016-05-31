/**
 * @file
 * @brief Implements the QtScriptPlugin defined in qtscriptplugin.hh.
 * @author Perttu Paarlahti 2016.
 */

#include "qtscriptplugin.hh"
#include "qtscriptinterpreter.hh"

QtScriptPlugin::QtScriptPlugin() :
    QObject(), ScriptEmbedderNS::InterpreterPlugin()
{   
}

QtScriptPlugin::~QtScriptPlugin()
{
}

QString QtScriptPlugin::language() const
{
    return "QtScript";
}

ScriptEmbedderNS::ScriptInterpreter* QtScriptPlugin::getInstance() const
{
    return new QtScriptInterpreter();
}

