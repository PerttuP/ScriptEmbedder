/**
 * @file
 * @brief Defines the InterpreterLoader class that is responsible for
 * loading and unloading InterpreterPlugins.
 * @author Perttu Paarlahti 2016.
 */

#ifndef INTERPRETERLOADER_HH
#define INTERPRETERLOADER_HH

#include <QPluginLoader>
#include "interpreterplugin.hh"
#include "configuration.hh"

namespace ScriptEmbedderNS
{

/**
 * @brief The InterpreterLoader class is responsible for loading interpreter
 * plugins. Class is trivial, but seperating it from ScriptEmbedder
 * implementations is reasonable for easier testing.
 */
class InterpreterLoader
{
public:

    /**
     * @brief Constructor.
     * @param entry Interpreter information.
     * @pre -
     * @post Ready to load plugins. No active errors.
     */
    InterpreterLoader(const InterpreterEntry& entry);

    /**
     * @brief Destructor. Unless unloadPlugin() has been called, plugin
     * and the interpreter object remains in memory untill application ends.
     */
    ~InterpreterLoader();

    /**
     * @brief Loads the plugin and returns the root object.
     * @return New Interpreter or nullptr, if loading fails.
     * @pre -
     * @post If loading fails, error message is available calling errorString().
     * The returned object is deleted when this object is destroyed or
     * unloadPlugin() is called explicitly.
     */
    InterpreterPlugin* instance();

    /**
     * @brief Unloads plugin previously loaded by this InterpreterLoader.
     * @pre The plugin is no longer in use.
     * @post Plugin is unloaded and the root object (Interpreter) is deleted.
     */
    void unloadPlugin();

    /**
     * @brief Retrurns error message.
     * @return Error message describing errors in lates load attempt.
     * @pre -
     */
    QString errorString() const;


private:

    InterpreterEntry entry_;
    QPluginLoader loader_;
    QString errorStr_;
};

} // namespace ScriptEmbedderNS

#endif // INTERPRETERLOADER_HH
