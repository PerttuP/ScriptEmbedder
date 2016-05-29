#ifndef SCRIPTINTERPRETER
#define SCRIPTINTERPRETER

#include <QString>
#include <QStringList>
#include <memory>
#include "scriptapi.hh"

namespace ScriptEmbedderNS
{

/**
 * @brief Interface for script interpreters.
 * This interface allows running scripts in a certain language language.
 */
class ScriptInterpreter
{
public:

    /**
     * @brief The result of a script run. SUCCESS means, that script was
     * executed successfully, and FAILURE means that there were errors
     * running the script (syntax errors or crashes).
     */
    enum Result {
        SUCCESS, FAILURE
    };

    /**
     * @brief The ScriptRunResult struct describes result of a singlme script run.
     */
    struct ScriptRunResult
    {
        /**
         * @brief Script end resutl (SUCCESS / FAILURE).
         */
        Result result;

        /**
         * @brief Script's return value.
         */
        int returnValue;

        /**
         * @brief If result was FAILURE, error message can be included in
         * this field.
         */
        QString errorString;
    };


    /**
     * @brief Mandatory virtual destructor.
     */
    virtual ~ScriptInterpreter() {}

    /**
     * @brief Set the ScriptAPI object for the interpreter.
     * @param api The ScriptAPI object.
     * @pre This method is called exactly once when interpreter is instantiated.
     * @post Scripts are allowed to use services of the api object.
     */
    virtual void SetScriptAPI(std::shared_ptr<ScriptAPI> api) = 0;

    /**
     * @brief runScript
     * @param script
     * @param params
     * @return Script run results containing Result, return value and
     * possibly error message describing errors.
     * @pre ScriptAPI object has been set.
     */
    virtual ScriptRunResult runScript(const QString& script,
                                      const QStringList& params) = 0;

    /**
     * @brief Get the name of scripting language supported by this interpreter.
     * @return Language name as a string.
     * @pre -
     */
    virtual QString language() const = 0;
};

} // namespace ScriptEmbeddetNS

#endif // SCRIPTINTERPRETER

