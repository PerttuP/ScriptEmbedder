/**
 * @file
 * @brief Defines the interface for different interpreter plugins.
 * @author Perttu Paarlahti 2016.
 */

#ifndef INTERPRETERPLUGIN_HH
#define INTERPRETERPLUGIN_HH

#include <QtPlugin>
#include <memory>
#include "scriptapi.hh"
#include "scriptinterpreter.hh"

// Unique IID for this type of plugin interface
// (Required for using QPluginLoader).
#define INTERPRETER_PLUGIN_IID "PerttuP.ScriptEmbedder.InterpreterPlugin"

namespace ScriptEmbedderNS
{

/**
 * @brief Interface for interpreter plugins.
 * This interface allows instantiating interpreters of a specific language.
 */
class InterpreterPlugin
{
public:

    /**
     * @brief Mandatory virtual destructor.
     */
    virtual ~InterpreterPlugin() {}

    /**
     * @brief Get the name of scripting language supported by this interpreter.
     * @return Language name as a string.
     * @pre -
     */
    virtual QString language() const = 0;

    /**
     * @brief Create new instance of an interpreter.
     * @return New interpreter for language specific to this plugin.
     * Ownership over new object is passed to the caller.
     */
    virtual ScriptInterpreter* getInstance() const = 0;
};

} // namespace ScriptEmbeddetNS

// Introduce interface to Qt Meta Object Compiler
// (Required to use QPluginLoader).
Q_DECLARE_INTERFACE(ScriptEmbedderNS::InterpreterPlugin, INTERPRETER_PLUGIN_IID)

#endif // INTERPRETERPLUGIN_HH

