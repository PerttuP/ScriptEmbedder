/**
 * @file
 * @brief Implements the SerialScriptEmbedder class defined in serialscriptembedder.hh.
 * @author Perttu Paarlahti 2016.
 */

#include "serialscriptembedder.hh"
#include <set>
#include <QFileInfo>
#include <QPluginLoader>

namespace ScriptEmbedderNS
{

SerialScriptEmbedder::SerialScriptEmbedder(const Configuration& conf) :
    ScriptEmbedder(),
    conf_(), logger_(nullptr), valid_(true), errorStr_(),
    loaders_(), interpreters_(), scripts_()
{
    Q_ASSERT(conf.isValid());
    this->reset(conf);
}


SerialScriptEmbedder::~SerialScriptEmbedder()
{
    this->clearConfiguration();
}


bool SerialScriptEmbedder::reset(const Configuration& conf)
{
    this->clearConfiguration();
    conf_ = conf;

    // Update plugins.
    if (!this->loadPlugins()) {
        errorStr_ = "Configuration failed: " + errorStr_;
        logMsg(errorString());
        this->clearConfiguration();
        return false;
    }

    // Update scripts.
    std::map<unsigned, ScriptEntry> entries = conf.scripts();
    for (auto it = entries.begin(); it != entries.end(); ++it) {
        if (it->second.readToRAM) {
            scripts_[it->first] = this->readScript(it->second.scriptPath);
            if (scripts_[it->first].isEmpty()){
                errorStr_ = QString("Configuration failed: source file '%1' "
                                    "for script '%2' does not open or is empty.")
                        .arg(it->second.scriptPath).arg(it->second.id);
                logMsg(errorString());
                this->clearConfiguration();
                return false;
            }
        }
    }

    logMsg("Configuration set successfully.");
    errorStr_.clear();
    valid_ = true;
    return true;
}


Configuration SerialScriptEmbedder::configuration() const
{
    return conf_;
}


bool SerialScriptEmbedder::isValid() const
{
    return valid_;
}


QString SerialScriptEmbedder::errorString() const
{
    return errorStr_;
}


void SerialScriptEmbedder::execute(unsigned scriptId, const QStringList& params)
{
    // Check that script exists.
    ScriptEntry scriptEntry = conf_.getScript(scriptId);
    if (scriptEntry == ScriptEntry()) {
        if (logger_ != nullptr){
            scriptEntry.id = scriptId;
            logger_->scriptFailed(scriptEntry, params,
                                  QString("Script '%1' does not exist.").arg(scriptId));
            return;
        }
    }

    ScriptInterpreter::ScriptRunResult result;
    std::shared_ptr<ScriptInterpreter> interpreter = interpreters_[scriptEntry.scriptLanguage];
    QString scriptStr;

    // Get script as a string.
    if (scriptEntry.readToRAM){
        scriptStr = scripts_[scriptEntry.id];
    }
    else {
        scriptStr = this->readScript(scriptEntry.scriptPath);
        if (scriptStr.isEmpty()) {
            if (logger_ != nullptr) {
                logger_->scriptFailed(scriptEntry, params,
                                      QString("File '%1' does not open or is empty.")
                                      .arg(scriptEntry.scriptPath));
                return;
            }
        }
    }

    // Run script and report results.
    result = interpreter->runScript(scriptStr, params);
    if (logger_ != nullptr) {
        if (result.result == ScriptInterpreter::FAILURE){
            logger_->scriptFailed(scriptEntry, params, result.errorString);
        } else {
            logger_->scriptExecuted(scriptEntry, params, result.returnValue);
        }
    }
}


bool SerialScriptEmbedder::addScript(const ScriptEntry& script)
{
    ScriptEntry entry = conf_.getScript(script.id);
    if (entry == script){
        // Identical script already exists.
        logMsg(QString("Script '%1' already exists.").arg(script.id));
        return true;
    }
    if (!conf_.hasInterpreter(script.scriptLanguage)){
        // There is no suitable interpreter for the script.
        errorStr_ = QString("Could not add script '%1': No suitable "
                            "interpreter for language '%2'.")
                .arg(script.id).arg(script.scriptLanguage);
        logMsg(errorString());
        return false;
    }
    if (!QFileInfo::exists(script.scriptPath)){
        // Script file does not exist.
        errorStr_ = QString("Could not add script: file '%1' does not exist.")
                .arg(script.scriptPath);
        logMsg(errorString());
        return false;
    }

    if (script.readToRAM){
        // Read script to RAM.
        QString scriptStr = this->readScript(script.scriptPath);
        if (scriptStr.isEmpty()) {
            errorStr_ = QString("Could not add script %1: File '%2' "
                                "does not open or is empty.")
                    .arg(script.id).arg(script.scriptPath);
            logMsg(errorString());
            return false;
        }
        scripts_[script.id] = scriptStr;
    }
    else if (scripts_.find(script.id) != scripts_.end()){
        scripts_.erase(script.id);
    }

    // Add to configuration and send log messages.
    if (conf_.hasScript(script.id)){
        logMsg(QString("Script '%1' replaced.").arg(script.id));
    } else {
        logMsg(QString("Script '%1' added.").arg(script.id));
    }
    conf_.addScript(script);
    return true;
}


void SerialScriptEmbedder::removeScript(unsigned scriptId)
{
    if (!conf_.hasScript(scriptId)){
        this->logMsg(QString("Could not remove script '%1': No such script.")
                     .arg(scriptId));
        return;
    }

    conf_.removeScript(scriptId);
    scripts_.erase(scriptId);
    this->logMsg(QString("Script '%1' removed.").arg(scriptId));
}


bool SerialScriptEmbedder::addInterpreter(const InterpreterEntry& interpreter)
{
    // Check if plugin already exists.
    auto it = loaders_.find(interpreter.scriptLanguage);
    if (it != loaders_.end()) {
        if (it->second->getInterpreterEntry() == interpreter) {
            logMsg(QString("Interpreter for '%1' already exists.")
                   .arg(interpreter.scriptLanguage) );
            return true;
        }
    }

    std::shared_ptr<InterpreterLoader> loader(new InterpreterLoader(interpreter));
    if (loader->instance() == nullptr){
        errorStr_ = "Could not add interpreter: " + loader->errorString();
        logMsg(errorString());
        return false;
    }
    std::shared_ptr<ScriptInterpreter> interpreter_ptr(loader->instance()->getInstance());

    // Check if loading fails.
    if (interpreter_ptr == nullptr){
        errorStr_ = "Could not add interpreter: " + loader->errorString();
        logMsg(errorString());
        loader->unloadPlugin();
        return false;
    }

    // Unload old interpreter
    if (it != loaders_.end()){
        interpreters_.erase(interpreter.scriptLanguage);
        it->second->unloadPlugin();
    }

    // Replace old interpreter or add new one.
    if (it != loaders_.end()) {
        logMsg(QString("Interpreter for '%1' replaced.")
               .arg(interpreter.scriptLanguage) );
    }
    else {
        logMsg(QString("Interpreter for '%1' added.")
               .arg(interpreter.scriptLanguage) );
    }

    loaders_[interpreter.scriptLanguage] = loader;
    interpreters_[interpreter.scriptLanguage] = interpreter_ptr;
    interpreter_ptr->SetScriptAPI(conf_.scriptAPI());
    conf_.addInterpreter(interpreter);
    return true;
}


void SerialScriptEmbedder::setLogger(Logger* logger)
{
    logger_ = logger;
}


void SerialScriptEmbedder::logMsg(const QString& msg)
{
    if (logger_ != nullptr){
        logger_->logMessage(msg);
    }
}


QString SerialScriptEmbedder::readScript(const QString& path)
{
    QFile f(path);
    if (!f.open(QFile::ReadOnly)){
        return QString();
    }
    QString script = f.readAll();
    f.close();
    return script;
}


bool SerialScriptEmbedder::loadPlugins()
{
    std::map<QString, InterpreterEntry> entries = conf_.interpreters();

    for (auto it = entries.begin(); it != entries.end(); ++it){
        loaders_[it->first] = std::shared_ptr<InterpreterLoader>(new InterpreterLoader(it->second));
        InterpreterPlugin* plugin = loaders_[it->first]->instance();

        if (plugin == nullptr){
            errorStr_ = loaders_[it->first]->errorString();
            return false;
        }

        std::shared_ptr<ScriptInterpreter> interpreter(plugin->getInstance());
        if (interpreter == nullptr){
            errorStr_ = loaders_[it->first]->errorString();
            return false;
        }

        interpreter->SetScriptAPI(conf_.scriptAPI());
        interpreters_[it->first] = interpreter;
    }

    return true;
}


void SerialScriptEmbedder::clearConfiguration()
{
    interpreters_.clear();
    scripts_.clear();

    for (auto it = loaders_.begin(); it != loaders_.end(); ++it) {
        it->second->unloadPlugin();
    }
    loaders_.clear();

    conf_ = Configuration();
    valid_ = false;
}


} // namespace ScriptEmbedderNS
