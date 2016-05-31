/**
 * @file
 * @brief Simple ScriptApi for example.
 * @author Perttu Paarlahti.
 */

#ifndef MYSCRIPTAPI_HH
#define MYSCRIPTAPI_HH

#include "scriptapi.hh"
#include <QString>
#include <QDateTime>


/**
 * @brief For testing custom types in API.
 */
struct CustomStruct
{
    int foo;
    double bar;
    char baz;
};


/**
 * @brief Interface for scripts to interact with the host application.
 */
class MyScriptAPI : public ScriptEmbedderNS::ScriptAPI
{
public:

    MyScriptAPI() : ScriptEmbedderNS::ScriptAPI() {}

    virtual ~MyScriptAPI() {}

    /**
     * @brief Example for method taking no parameters and returning nothing.
     * Prints 'Hello.'.
     */
    virtual void sayHello() = 0;

    /**
     * @brief Example for method taking parameters and retuning nothing.
     * @param msg Message to be printed.
     * @param repeat How many times message is repeated.
     */
    virtual void saySomething(const QString& msg, int repeat) = 0;

    /**
     * @brief Example  for using qt class as return value.
     * @return Datetime now.
     */
    virtual QDateTime timeNow() = 0;

    /**
     * @brief Example for returning custom types and taking one as a parameter.
     * @param myStruct Struct to be done stuff to.
     * @return Other CustomStruct.
     */
    virtual CustomStruct doStuffToMyStruct(const CustomStruct& myStruct) = 0;
};

#endif // MYSCRIPTAPI_HH

