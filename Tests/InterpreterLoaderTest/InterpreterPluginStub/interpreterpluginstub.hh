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
     * @brief Stub implementation for the InterpreterPlugin interface
     * @return 'TestLanguage'.
     */
    QString language() const;

    /**
     * @brief Stub implementation.
     * @return nullptr.
     */
    ScriptEmbedderNS::ScriptInterpreter* getInstance() const;
};

#endif // INTERPRETERPLUGINSTUB_HH
