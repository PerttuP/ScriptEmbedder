/**
 * @file
 * @brief Implements the MyScriptAPI interface.
 * @author Perttu Paarlahti 2016.
 */

#ifndef MYSCRIPTAPIIMPLEMENTATION_HH
#define MYSCRIPTAPIIMPLEMENTATION_HH

#include "myscriptapi.hh"

/**
 * @brief Implements the MyScriptAPI interface.
 */
class MyScriptApiImplementation : public MyScriptAPI
{
public:

    MyScriptApiImplementation();

    virtual ~MyScriptApiImplementation();

    // MyScriptAPI interface
    void sayHello();
    void saySomething(const QString& msg, int repeat);
    QDateTime timeNow();
    CustomStruct doStuffToMyStruct(const CustomStruct& myStruct);
};

#endif // MYSCRIPTAPIIMPLEMENTATION_HH
