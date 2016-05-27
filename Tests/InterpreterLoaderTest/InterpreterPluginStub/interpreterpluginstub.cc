/**
 * @file
 * @brief Implements the InterpreterPluginStub class defined in interpreterpluginstub.hh.
 * @author Perttu Paarlahti 2016.
 */

#include "interpreterpluginstub.hh"

InterpreterPluginStub::InterpreterPluginStub() :
    ScriptEmbedderNS::InterpreterPlugin()
{

}

InterpreterPluginStub::~InterpreterPluginStub()
{

}

void InterpreterPluginStub::SetScriptAPI(std::shared_ptr<ScriptEmbedderNS::ScriptAPI> api)
{
    Q_UNUSED(api);
}


ScriptEmbedderNS::InterpreterPlugin::ScriptRunResult
InterpreterPluginStub::runScript(const QString& script, const QStringList& params)
{
    Q_UNUSED(script);
    Q_UNUSED(params);
    return ScriptRunResult();
}

QString InterpreterPluginStub::language() const
{
    return QString("TestLanguage");
}

