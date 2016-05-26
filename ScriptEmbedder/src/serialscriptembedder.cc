#include "serialscriptembedder.hh"
#include <QFileInfo>
#include <QPluginLoader>

namespace ScriptEmbedderNS
{

SerialScriptEmbedder::SerialScriptEmbedder(const Configuration& conf) :
    ScriptEmbedder(),
    conf_(), logger_(nullptr), interpreters_(), valid_(true), errorStr_(),
    scriptsInRAM_()
{
    Q_ASSERT(conf.isValid());
    this->reset(conf);
}


bool SerialScriptEmbedder::reset(const Configuration& conf)
{
    Q_ASSERT(conf.isValid());
    conf_ = conf;
    errorStr_.clear();
    valid_ = true;
    interpreters_.clear();
    scriptsInRAM_.clear();

    // Load interpreters.
    std::map<QString, InterpreterEntry> interpreters = conf_.interpreters();
    for (auto it = interpreters.begin(); it != interpreters.end(); ++it){
        std::shared_ptr<InterpreterPlugin> plugin = this->loadPlugin(it->second);
        if (plugin == nullptr){
            errorStr_ = "Could not load plugin: " + it->second.pluginPath + ".";
            valid_ = false;
            interpreters_.clear();
            return false;
        }
        interpreters_[it->first] = plugin;
    }

    // Load scripts to RAM.
    std::map<unsigned, ScriptEntry> scripts = conf_.scripts();
    for (auto it = scripts.begin(); it != scripts.end(); ++it) {
        if (it->second.readToRAM){
            QString scriptStr = this->readScript(it->second.scriptPath);
            if (scriptStr.isEmpty()){
                errorStr_ = QString("Script file (%1) is empty or does not open")
                        .arg(it->second.scriptPath);
                valid_ = false;
                interpreters_.clear();
                scriptsInRAM_.clear();
                return false;
            }
            scriptsInRAM_[it->first] = scriptStr;
        }
    }
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
    ScriptEntry script = conf_.getScript(scriptId);
    if (script == ScriptEntry()){
        if (logger_ != nullptr){
            script.id = scriptId;
            logger_->scriptFailed(script, params, "No such script.");
        }
        return;
    }

    // Execute script.
    std::shared_ptr<InterpreterPlugin> interpreter = interpreters_[script.scriptLanguage];
    InterpreterPlugin::ScriptRunResult result;

    if (script.readToRAM){
        // Run script from RAM.
        result = interpreter->runScript(scriptsInRAM_[scriptId], params);
    }
    else {
        // Read and run script from disk.
        QString str = this->readScript(script.scriptPath);
        if (str.isEmpty()){
            // File does not open.
            result.result = InterpreterPlugin::FAILURE;
            result.errorString = "File does not open.";
        }
        result = interpreter->runScript(str, params);
    }

    // Notify logger.
    if (logger_ != nullptr){
        if (result.result == InterpreterPlugin::SUCCESS){
            logger_->scriptExecuted(script, params, result.returnValue);
        } else {
            logger_->scriptFailed(script, params, result.errorString);
        }
    }
}


bool SerialScriptEmbedder::addScript(const ScriptEntry& script)
{
    // Check that script file exists
    if (!QFileInfo::exists(script.scriptPath)){
        errorStr_ = QString("Could not add script. File '%1' does not exist")
                .arg(script.scriptPath);
        return false;
    }
    // Check that script language is supported.
    if (!conf_.hasInterpreter(script.scriptLanguage)) {
        errorStr_ = QString("Could not add script. Language '%1' is not supported")
                .arg(script.scriptLanguage);
        return false;
    }

    conf_.addScript(script);
    return true;

}


void SerialScriptEmbedder::removeScript(unsigned scriptId)
{
    conf_.removeScript(scriptId);
}


bool SerialScriptEmbedder::addInterpreter(const InterpreterEntry& interpreter)
{
    // Check if interpreter already exists.
    if (conf_.getInterpteter(interpreter.scriptLanguage) == interpreter) {
        return true;
    }

    // Load plugin
    std::shared_ptr<InterpreterPlugin> plugin = this->loadPlugin(interpreter);
    if (plugin == nullptr){
        return false;
    }

    // Add interpreter / replace old one.
    interpreters_[interpreter.scriptLanguage] = plugin;
    conf_.addInterpreter(interpreter);
    return true;
}


void SerialScriptEmbedder::setLogger(Logger* logger)
{
    logger_ = logger;
}


std::shared_ptr<InterpreterPlugin> SerialScriptEmbedder::loadPlugin(const InterpreterEntry& conf)
{
    QPluginLoader loader(conf.pluginPath);
    QObject* plugin = loader.instance();
    InterpreterPlugin* interpreter = qobject_cast<InterpreterPlugin*>(plugin);
    return std::shared_ptr<InterpreterPlugin>(interpreter);
}


QString SerialScriptEmbedder::readScript(const QString& fileName)
{
    QFile f(fileName);
    if (!f.open(QFile::ReadOnly)){
        return QString();
    }

    QString script = f.readAll();
    f.close();
    return script;
}

} // namespace ScriptEmbedderNS
