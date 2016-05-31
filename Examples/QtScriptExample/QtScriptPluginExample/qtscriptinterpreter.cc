/**
 * @file
 * @brief Implements the QtScriptInterpreter class defined in qtscriptinterpreter.hh.
 * @author Perttu Paarlahti 2016.
 */

#include "qtscriptinterpreter.hh"
#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueList>
#include "qtscriptapiadapter.hh"


QtScriptInterpreter::QtScriptInterpreter() :
    ScriptEmbedderNS::ScriptInterpreter(), api_(), eng_(),
    adapter_(nullptr), apiObject_()
{   
}

QtScriptInterpreter::~QtScriptInterpreter()
{
    delete adapter_;
}


void QtScriptInterpreter::SetScriptAPI(std::shared_ptr<ScriptEmbedderNS::ScriptAPI> api)
{
    api_ = std::dynamic_pointer_cast<MyScriptAPI>(api);
    Q_ASSERT(api_ != nullptr);
    adapter_ = new QtScriptApiAdapter(api_.get(), &eng_);
    apiObject_ = eng_.newQObject(adapter_);
    eng_.globalObject().setProperty("HostAppAPI", apiObject_);
}


ScriptEmbedderNS::ScriptInterpreter::ScriptRunResult
QtScriptInterpreter::runScript(const QString& script, const QStringList& params)
{
    Q_ASSERT(api_ != nullptr);

    // Set parameters
    QScriptValue args = qScriptValueFromSequence(&eng_, params);
    eng_.globalObject().setProperty("argv", args);
    eng_.globalObject().setProperty("argc", params.count());

    // Evaluate script.
    ScriptRunResult result;
    result.returnValue = eng_.evaluate(script).toInt32();
    if (eng_.hasUncaughtException()){
        result.result = FAILURE;
        result.errorString = eng_.uncaughtException().toString();
    }

    return result;
}


QString QtScriptInterpreter::language() const
{
    return "QtScript";
}

