/**
 * @file
 * @brief Implements the InterpreterPluginStub class defined in interpreterpluginstub.hh.
 * @author Perttu Paarlahti 2016.
 */

#include "interpreterpluginstub.hh"

InterpreterPluginStub::InterpreterPluginStub() :
    ScriptEmbedderNS::InterpreterPlugin()
{

}

InterpreterPluginStub::~InterpreterPluginStub()
{

}

QString InterpreterPluginStub::language() const
{
    return QString("TestLanguage");
}

ScriptEmbedderNS::ScriptInterpreter*InterpreterPluginStub::getInstance() const
{
    return new InterpreterStub();
}

