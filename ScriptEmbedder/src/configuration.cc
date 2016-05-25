/**
 * @file
 * @brief Implementation for the ScriptEmbedder::Configuration class defined in
 * configuration.hh.
 * @author Perttu Paarlahti 2016.
 */

#include "configuration.hh"
#include <QFileInfo>
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
    interpreters_[interpreter.scriptLanguage] = interpreter;
}


void Configuration::removeInterpreter(const QString& lang)
{
    interpreters_.erase(lang);
}


std::map<QString, InterpreterEntry> Configuration::interpreters() const
{
    return interpreters_;
}


void Configuration::addScript(const ScriptEntry& script)
{
    scripts_[script.id] = script;
}


void Configuration::removeScript(unsigned id)
{
    scripts_.erase(id);
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
        return QString("ScriptAPI has not been set.");
    }
    if (interpreters_.empty()){
        return QString("At liest one interpreter has to be set.");
    }
    for (auto iter = scripts_.begin(); iter != scripts_.end(); ++iter) {
        if (!QFileInfo::exists(iter->second.scriptPath)){
            return QString("Path (%1) for script(id=%2) is invalid.")
                    .arg(iter->second.scriptPath).arg(iter->second.id);
        }
        if (interpreters_.find(iter->second.scriptLanguage) == interpreters_.end()) {
            return QString("No suitable interpreter for '%1' required by script(id=%2).")
                    .arg(iter->second.scriptLanguage).arg(iter->second.id);
        }
    }
    for (auto iter = interpreters_.begin(); iter != interpreters_.end(); ++iter) {
        if (!QLibrary::isLibrary(iter->second.pluginPath)) {
            return QString("Invalid interpreter plugin path: '%1'.").arg(iter->second.pluginPath);
        }
    }

    Q_ASSERT(false);  // This should never be executed.
    return QString(); // Suppress warnings.
}


bool ScriptEntry::operator==(const ScriptEntry& rhs) const
{
    return this->id == rhs.id &&
            this->scriptLanguage == rhs.scriptLanguage &&
            this->scriptPath == rhs.scriptPath &&
            this->readToRAM == rhs.readToRAM &&
            this->priority == rhs.priority;
}


bool InterpreterEntry::operator==(const InterpreterEntry& rhs) const
{
    return this->pluginPath == rhs.pluginPath &&
            this->scriptLanguage == rhs.scriptLanguage;
}

} // namespace ScriptEmbedderNS
