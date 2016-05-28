/**
 * @file
 * @brief Defines the SerialScriptEmbedder class, one implementation for the
 * ScriptEmbedder interface. This implemetation does not execute scripts
 * concurrently, but executes them imediately before returning.
 */

#ifndef SERIALSCRIPTEMBEDDER_HH
#define SERIALSCRIPTEMBEDDER_HH

#include "scriptembedder.hh"
#include "interpreterplugin.hh"

namespace ScriptEmbedderNS
{

/**
 * @brief Implemets the ScriptEmbedder interface.
 * This implemetation executes scripts imediately when execute method is called.
 * The execute method does not return before script has finished.
 */
class SerialScriptEmbedder : public ScriptEmbedder
{
public:

    /**
     * @brief Constructor.
     * @param conf Configuration.
     * @pre conf is valid.
     * @post ScriptRunner is initialized according to configuration.
     * If any of plugins fail to load or any of script files do not open,
     * ScriptEmbedder becomes invalid. Check validity with isValid(). Error
     * message is available calling errorString().
     */
    SerialScriptEmbedder(const Configuration& conf);

    // ScriptEmbedder interface
    bool reset(const Configuration& conf);
    Configuration configuration() const;
    bool isValid() const;
    QString errorString() const;
    void execute(unsigned scriptId, const QStringList& params);
    bool addScript(const ScriptEntry& script);
    void removeScript(unsigned scriptId);
    bool addInterpreter(const InterpreterEntry& interpreter);
    void setLogger(Logger* logger);
};

} // namespace ScriptEmbedderNS

#endif // SERIALSCRIPTEMBEDDER_HH
