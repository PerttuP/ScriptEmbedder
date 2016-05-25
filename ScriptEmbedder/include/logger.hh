/**
 * @file
 * @brief Defines the interface for reporting ScriptEmbedder events to the
 * component user. Component user provides implementation for this interface.
 * @author Perttu Paarlahti 2016.
 */

#ifndef LOGGER
#define LOGGER

#include <QString>
#include "configuration.hh"

namespace ScriptEmbedderNS
{

/**
 * @brief This is the interface for reporting ScriptEmbedder events to user.
 * The user provides implementation for this interface.
 */
class Logger
{
public:

    /**
     * @brief Constructor.
     */
    Logger() {}

    /**
     * @brief Mandatory virtual destructor.
     */
    virtual ~Logger() {}

    /**
     * @brief Method for receiving general log messages.
     * ScriptEmbedder sends log messages on significant events such as
     * new configuration, new script, new interpreters and failures.
     * @param msg Log message.
     */
    virtual void logMessage(const QString& msg) = 0;

    /**
     * @brief Method for receiving notifications on successfully run scripts.
     * @param script Script that has been executed.
     * @param params Parameters that were passed to the script.
     * @param returnValue Script's return value.
     * @pre The script really have been executed. Params are the actual parameters.
     */
    virtual void scriptExecuted(const ScriptEntry& script,
                               const QStringList& params,
                               int returnValue) = 0;

    /**
     * @brief Method for receiving notifications on failed (crashed) script runs.
     * @param script Script that failed.
     * @param params Parameters that were passed to the script.
     * @param errorMsg Error message produced by the interpreter.
     * @pre Script was really run and it failed. Params are the actual parameters.
     */
    virtual void scriptFailed(const ScriptEntry& script,
                              const QStringList& params,
                              const QString& errorMsg) = 0;
};

} // namespace ScriptEmbedderNS

#endif // LOGGER

