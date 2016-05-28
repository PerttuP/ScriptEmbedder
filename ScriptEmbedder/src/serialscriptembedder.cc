#include "serialscriptembedder.hh"
#include <QFileInfo>
#include <QPluginLoader>

namespace ScriptEmbedderNS
{

SerialScriptEmbedder::SerialScriptEmbedder(const Configuration& conf)
{
    Q_UNUSED(conf);
}

bool SerialScriptEmbedder::reset(const Configuration& conf)
{
    Q_UNUSED(conf);
    return false;
}

Configuration SerialScriptEmbedder::configuration() const
{
    return Configuration();
}

bool SerialScriptEmbedder::isValid() const
{
    return false;
}

QString SerialScriptEmbedder::errorString() const
{
    return QString("Not implemented.");
}

void SerialScriptEmbedder::execute(unsigned scriptId, const QStringList& params)
{
    Q_UNUSED(scriptId);
    Q_UNUSED(params);
}

bool SerialScriptEmbedder::addScript(const ScriptEntry& script)
{
    Q_UNUSED(script);
    return false;
}

void SerialScriptEmbedder::removeScript(unsigned scriptId)
{
    Q_UNUSED(scriptId);
}

bool SerialScriptEmbedder::addInterpreter(const InterpreterEntry& interpreter)
{
    Q_UNUSED(interpreter);
    return false;
}

void SerialScriptEmbedder::setLogger(Logger* logger)
{
    Q_UNUSED(logger);
}

} // namespace ScriptEmbedderNS
