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

    InterpreterPluginStub();

    virtual ~InterpreterPluginStub();

    /**
     * @brief Empty implementation (not needed in InterpreterLoaderTests).
     */
    void SetScriptAPI(std::shared_ptr<ScriptEmbedderNS::ScriptAPI> api);

    /**
     * brief Empty implementation (not needed in InterpreterLoaderTests).
     */
    ScriptRunResult runScript(const QString& script, const QStringList& params);

    /**
     * @brief Stub implementation for the InterpreterPlugin interface
     * @return 'TestLanguage'.
     */
    QString language() const;
};

#endif // INTERPRETERPLUGINSTUB_HH
