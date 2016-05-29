/**
 * @file
 * @brief Implements the InterpreterLoader class defined in interpreterloader.hh.
 * @author Perttu Paarlahti 2016.
 */

#include "interpreterloader.hh"

namespace ScriptEmbedderNS
{

InterpreterLoader::InterpreterLoader(const InterpreterEntry& entry) :
    entry_(entry), loader_(entry.pluginPath), errorStr_()
{
}


InterpreterLoader::~InterpreterLoader()
{
}


InterpreterPlugin* InterpreterLoader::instance()
{
    QObject* plugin = loader_.instance();

    // Check that plugin was loaded.
    if (plugin == nullptr) {
        errorStr_ = QString("Failed to load %1 plugin: %2.")
                .arg(entry_.scriptLanguage).arg(entry_.pluginPath);
        return nullptr;
    }

    // Check that root object implements the InterpreterPlugin interface.
    InterpreterPlugin* interpreter = qobject_cast<InterpreterPlugin*>(plugin);
    if (interpreter == nullptr){
        errorStr_ = QString("Plugin '%1' is not an interpreter plugin.")
                .arg(entry_.pluginPath);
        this->unloadPlugin();
        return nullptr;
    }

    // Check that interpreter is for desired language.
    if (interpreter->language() != entry_.scriptLanguage) {
        errorStr_ = QString("Plugin '%1' is not for desired language '%2'.")
                .arg(entry_.pluginPath).arg(entry_.scriptLanguage);
        this->unloadPlugin();
        return nullptr;
    }

    return interpreter;
}


void InterpreterLoader::unloadPlugin()
{
    loader_.unload(); // Deletes root object.
}


QString InterpreterLoader::errorString() const
{
    return errorStr_;
}


InterpreterEntry InterpreterLoader::getInterpreterEntry() const
{
    return entry_;
}

} // namespace ScriptEmbedderNS
