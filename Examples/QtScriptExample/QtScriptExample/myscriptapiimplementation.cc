/**
 * @file
 * @brief Implements the MyScriptApiImplementation class defubed in
 * myscriptapi.hh.
 * @author Perttu Paarlahti 2016.
 */

#include "myscriptapiimplementation.hh"
#include <QDebug>

MyScriptApiImplementation::MyScriptApiImplementation() :
    MyScriptAPI()
{
}

MyScriptApiImplementation::~MyScriptApiImplementation()
{
}

void MyScriptApiImplementation::sayHello()
{
    qDebug() << "Hello!";
}

void MyScriptApiImplementation::saySomething(const QString& msg, int repeat)
{
    for (int i=0; i<repeat; ++i){
        qDebug() << msg;
    }
}

QDateTime MyScriptApiImplementation::timeNow()
{
    return QDateTime::currentDateTime();
}

CustomStruct MyScriptApiImplementation::doStuffToMyStruct(const CustomStruct& myStruct)
{
    CustomStruct cs;
    cs.foo = myStruct.foo+1;
    cs.bar = myStruct.bar*10;
    cs.baz = myStruct.baz+1;
    return cs;
}

