#include "configuration.hh"
#include <QFile>
#include <QLibrary>

namespace ScriptEmbedderNS {


Configuration::Configuration() :
    api_(nullptr), interpreters_(), scripts_()
{
    Q_ASSERT(!this->isValid());
}


Configuration::Configuration(std::shared_ptr<ScriptAPI> api,
                             std::map<QString, InterpreterEntry> interpreters,
                             std::map<unsigned, ScriptEntry> scripts) :
    api_(api), interpreters_(interpreters), scripts_(scripts)
{
}


void Configuration::setScriptAPI(std::shared_ptr<ScriptAPI> api)
{
    Q_ASSERT(api != nullptr);
    api_ = api;
}


std::shared_ptr<ScriptAPI> Configuration::scriptAPI() const
{
    return api_;
}


void Configuration::addInterpreter(const InterpreterEntry& interpreter)
{
    interpreters_[interpreter.ScriptLanguage] = interpreter;
}


std::map<QString, InterpreterEntry> Configuration::interpreters() const
{
    return interpreters_;
}


void Configuration::addScript(const ScriptEntry& script)
{
    scripts_[script.id] = script;
}


std::map<unsigned, ScriptEntry> Configuration::scripts() const
{
    return scripts_;
}


bool Configuration::isValid() const
{
    // Check api and number of interpreters.
    if (api_ == nullptr) return false;
    if (interpreters_.empty()) return false;

    // Check script entries.
    for (auto iter = scripts_.begin(); iter != scripts_.end(); ++iter){
        if (interpreters_.find(iter->second.scriptLanguage) == interpreters_.end()) {
            return false;
        }
        else if (!QFile::exists(iter->second.scriptPath)){
            return false;
        }
    }

    // Check interpreters
    for (auto iter = interpreters_.begin(); iter != interpreters_.end(); ++iter) {
        if (!QLibrary::isLibrary(iter->second.pluginPath)){
            return false;
        }
    }

    return true;
}


QString Configuration::errorString() const
{
    if (this->isValid()) return QString();

    if (api_ == nullptr){
        return QString("ScriptAPI object has not been set.");
    }
    if (interpreters_.empty()){
        return QString("At liest one interpreter has to be set.");
    }
    for (auto iter = scripts_.begin(); iter != scripts_.end(); ++iter) {
        if (!QFile::exists(iter->second.scriptPath)){
            return QString("Path (%1) for script(id=%2) is invalid.")
                    .arg(iter->second.scriptPath).arg(iter->second.id);
        }
        if (interpreters_.find(iter->second.scriptLanguage) == interpreters_.end()) {
            return QString("No suitable interpreter for script(id=%1 language='%2').")
                    .arg(iter->second.id).arg(iter->second.scriptLanguage);
        }
    }
    for (auto iter = interpreters_.begin(); iter != interpreters_.end(); ++iter) {
        if (!QLibrary::isLibrary(iter->second.pluginPath))
            return QString("Plugin does not exist: '%1'").arg(iter->second.pluginPath);
    }

    Q_ASSERT(false);  // This should never be executed.
    return QString(); // Suppress warnings.
}

} // namespace ScriptEmbedderNS
