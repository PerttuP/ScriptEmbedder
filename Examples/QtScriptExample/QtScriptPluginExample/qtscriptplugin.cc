#include "qtscriptplugin.hh"

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

ScriptEmbedderNS::ScriptInterpreter*QtScriptPlugin::getInstance() const
{

}

