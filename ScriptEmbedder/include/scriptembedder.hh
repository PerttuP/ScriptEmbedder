/**
 * @file
 * @brief Defines the ScriptEmbedder interface for interacting with the
 * ScriptEmbedder component.
 * @author Perttu Paarlahti 2016.
 */

#ifndef SCRIPTEMBEDDER_HH
#define SCRIPTEMBEDDER_HH

#include "configuration.hh"
#include "logger.hh"
#include <QStringList>

namespace ScriptEmbedderNS
{

/**
 * @brief The ScriptEmbedder class is the interface for
 * interacting with the ScriptEmbedder component.
 */
class ScriptEmbedder
{
public:

    /**
     * @brief Constructor.
     */
    ScriptEmbedder() {}

    /**
     * @brief Mandatory virtual destructor.
     */
    virtual ~ScriptEmbedder() {}

    /**
     * @brief Set new configuration for the ScriptEmbedder.
     * @param conf New configuration.
     * @return True, if new configuration was successfully set.
     * @pre conf is valid.
     * @post ScriptEmbedder starts working according to the new configuration.
     * If setting configuration failed, error message is available in errorString().
     * If setting configuration fails, embedder becomes invalid.
     */
    virtual bool reset(const Configuration& conf) = 0;

    /**
     * @brief Get currently applied configuration.
     * @return Current configuration.
     * @pre -
     */
    virtual Configuration configuration() const = 0;

    /**
     * @brief Check if ScriptEmbedder is in a valid state.
     * This method may return false only after instantiation or reset. Invalid object
     * cannot be used to run scripts. If this returns false, error message is
     * available in errorString(). If ScriptEmbedder is invalid, it may be
     * fixed by reseting.
     * @return True, if ScriptEmbedder is in a valid state.
     * @pre -
     */
    virtual bool isValid() const = 0;

    /**
     * @brief This method returns the latest error message.
     * @return Message describing the latest error, or empty string if no
     * errors have occured.
     * @pre -
     */
    virtual QString errorString() const = 0;

    /**
     * @brief Execute script having given id. Pass given parameters to script.
     * @param scriptId Script's unique identifier.
     * @param params Parameters to be passed to the script.
     * @pre -
     * @post Script is executed if it does exist. Logger is notified if script
     * does not exist, and when script finishes or fails.
     */
    virtual void execute(unsigned scriptId,
                         const QStringList& params = QStringList()) = 0;

    /**
     * @brief Add new script into current configuration.
     * @param script Script to be added.
     * @return True, if script was added successfully.
     * @pre -
     * @post Script is added to configuration. If script path is invalid
     * or suitable interpreter has not been set, script is not added
     * and method returns false. In case of error, error message can be found
     * in errorString(). If there already exists a script with same id, it is
     * replaced. Logger is notified.
     */
    virtual bool addScript(const ScriptEntry& script) = 0;

    /**
     * @brief Remove script from current configuration.
     * @param scriptId Id of script to be removed.
     * @pre -
     * @post Script is removed from configuration. Logger is notified.
     */
    virtual void removeScript(unsigned scriptId) = 0;

    /**
     * @brief Add new interpreter into current configuration.
     * @param interpreter New Interpreter.
     * @return True, if interpreter was set and plugin was loaded successfully.
     * @pre -
     * @post New interpreter is set and plugin is loaded. If plugin is not found
     * or loading fails for other reason, no changes are made and method returns
     * false. Error message can be found in errorString(). If interpreter of
     * same language already exists, it is replaced and unloaded.
     */
    virtual bool addInterpreter(const InterpreterEntry& interpreter) = 0;

    /**
     * @brief Set logger.
     * @param logger Logger that is responsible for receiving log messages and
     * reports from ScriptEmbedder.
     * @pre -
     * @post Logger receives log messages and reports from now on. If logger is
     * nullptr or this method is never called, log messages and reports are
     * disabled.
     */
    virtual void setLogger(Logger* logger) = 0;
};

} // namespace ScriptEmbedderNS

#endif // SCRIPTEMBEDDER_HH

