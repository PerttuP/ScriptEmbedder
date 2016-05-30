/**
 * @file
 * @brief Stub implementation for InterpreterPlugin. Usage: Testing the
 * ScriptEmbedder::InterpreterLoader class.
 * @author Perttu Paarlahti 2016.
 */

#ifndef INTERPRETERPLUGINSTUB_HH
#define INTERPRETERPLUGINSTUB_HH

#include <QtPlugin>
#include "interpreterplugin.hh"


/**
 * @brief The Actual 'interpreter' used in testing InterpreterLoader.
 */
class InterpreterStub : public ScriptEmbedderNS::ScriptInterpreter
{
public:

    InterpreterStub() : ScriptEmbedderNS::ScriptInterpreter() {}

    virtual ~InterpreterStub() {}

    void SetScriptAPI(std::shared_ptr<ScriptEmbedderNS::ScriptAPI> api)
    {
        Q_UNUSED(api);
    }

    ScriptRunResult runScript(const QString& script, const QStringList& params)
    {
        Q_UNUSED(script);
        Q_UNUSED(params);
        return ScriptRunResult();
    }

    QString language() const
    {
        return "TestLanguage";
    }
};


/**
 * @brief The InterpreterPluginStub class
 * Stub implementation for InterpreterPlugin. This plugin is used for
 * testing the InterpreterLoader class.
 */
class InterpreterPluginStub :
        public QObject,
        public ScriptEmbedderNS::InterpreterPlugin
{
    Q_OBJECT
    Q_INTERFACES(ScriptEmbedderNS::InterpreterPlugin)
    Q_PLUGIN_METADATA(IID INTERPRETER_PLUGIN_IID)

public:

    InterpreterPluginStub() :
        ScriptEmbedderNS::InterpreterPlugin() {}

    virtual ~InterpreterPluginStub() {}

    /**
     * @brief Stub implementation for the InterpreterPlugin interface
     * @return 'TestLanguage'.
     */
    QString language() const
    {
        return QString("TestLanguage");
    }


    /**
     * @brief Stub implementation.
     * @return nullptr.
     */
    ScriptEmbedderNS::ScriptInterpreter* getInstance() const
    {
        return new InterpreterStub();
    }
};

#endif // INTERPRETERPLUGINSTUB_HH
