/**
 * @file
 * @brief Test plugin for testing ScriptEmbedder.
 * @author Perttu Paarlahti 2016.
 */

#ifndef INTERPRETERTESTPLUGIN
#define INTERPRETERTESTPLUGIN

#include "interpreterplugin.hh"


/**
 * @brief Test ScriptInterpreter for testing ScriptEmbedder.
 */
class TestInterpreter : public ScriptEmbedderNS::ScriptInterpreter
{
    ScriptEmbedderNS::ScriptInterpreter::ScriptRunResult& nextResult;
    std::shared_ptr<ScriptEmbedderNS::ScriptAPI>& myApi;
    QString& latestScript;
    QStringList& latestParams;

public:

    TestInterpreter(std::shared_ptr<ScriptEmbedderNS::ScriptAPI>& api,
                    QString& script,
                    QStringList& params,
                    ScriptEmbedderNS::ScriptInterpreter::ScriptRunResult& result) :
        ScriptEmbedderNS::ScriptInterpreter(),
        nextResult(result), myApi(api), latestScript(script), latestParams(params)
    {
    }

    virtual ~TestInterpreter() {}

    void SetScriptAPI(std::shared_ptr<ScriptEmbedderNS::ScriptAPI> api)
    {
        myApi = api;
    }

    ScriptRunResult runScript(const QString& script, const QStringList& params)
    {
        latestScript = script;
        latestParams = params;
        return nextResult;
    }

    QString language() const
    {
        return "TestLanguage";
    }
};


/**
 * @brief Test InterpreterPlugin for testing ScriptEmbedder.
 */
class InterpreterTestPlugin :
        public QObject,
        public ScriptEmbedderNS::InterpreterPlugin
{
    Q_OBJECT
    Q_INTERFACES(ScriptEmbedderNS::InterpreterPlugin)
    Q_PLUGIN_METADATA(IID INTERPRETER_PLUGIN_IID)

public:

    InterpreterTestPlugin() :
        QObject(), ScriptEmbedderNS::InterpreterPlugin(),
        api(nullptr), result(), script(), params() {}

    virtual ~InterpreterTestPlugin() {}

    QString language() const
    {
        return "TestLanguage";
    }

    ScriptEmbedderNS::ScriptInterpreter* getInstance() const
    {
        return new TestInterpreter(api, script, params, result);
    }


    // Members used to control and verify tests.
    mutable std::shared_ptr<ScriptEmbedderNS::ScriptAPI> api;
    mutable ScriptEmbedderNS::ScriptInterpreter::ScriptRunResult result;
    mutable QString script;
    mutable QStringList params;
};


#endif // INTERPRETERTESTPLUGIN

