/**
 * @file
 * @brief Defines example interpreter for the QtScript language.
 * @author Perttu Paarlahti 2016.
 */

#ifndef QTSCRIPTINTERPRETER_HH
#define QTSCRIPTINTERPRETER_HH

#include "scriptinterpreter.hh"
#include "myscriptapi.hh"
#include "qtscriptapiadapter.hh"
#include <QScriptValue>

/**
 * @brief Example interpreter for the QtScript language.
 */
class QtScriptInterpreter : public ScriptEmbedderNS::ScriptInterpreter
{
public:

    QtScriptInterpreter();
    virtual ~QtScriptInterpreter();

    // ScriptInterpreter interface
    void SetScriptAPI(std::shared_ptr<ScriptEmbedderNS::ScriptAPI> api);
    ScriptRunResult runScript(const QString& script, const QStringList& params);
    QString language() const;

private:

    std::shared_ptr<MyScriptAPI> api_;
    QScriptEngine eng_;
    QtScriptApiAdapter* adapter_;
    QScriptValue apiObject_;
};

#endif // QTSCRIPTINTERPRETER_HH
