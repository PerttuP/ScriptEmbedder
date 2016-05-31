/**
 * @file
 * @brief Implements the QtScriptApiAdapter class defined in qtscriptapiadapter.hh.
 * @author Perttu Paarlahti 2016.
 */

#include "qtscriptapiadapter.hh"

QtScriptApiAdapter::QtScriptApiAdapter(MyScriptAPI* api, QScriptEngine* engine, QObject* parent):
    QObject(parent), api_(api), eng_(engine)
{
    Q_ASSERT(api_ != nullptr);
    Q_ASSERT(eng_ != nullptr);
}

QtScriptApiAdapter::~QtScriptApiAdapter()
{
}

void QtScriptApiAdapter::sayHello()
{
    api_->sayHello();
}

void QtScriptApiAdapter::saySomething(const QString& msg, int repeat)
{
    api_->saySomething(msg, repeat);
}

QDateTime QtScriptApiAdapter::timeNow()
{
    return api_->timeNow();
}

QScriptValue QtScriptApiAdapter::doStuffToMyStruct(const QScriptValue& myStruct)
{
    CustomStruct cs;
    fromScriptValue(myStruct, cs);
    CustomStruct rv = api_->doStuffToMyStruct(cs);
    return toScriptValue(eng_, rv);
}


QScriptValue toScriptValue(QScriptEngine* eng, const CustomStruct& cs)
{
    Q_ASSERT(eng != nullptr);
    QScriptValue val = eng->newObject();
    val.setProperty("foo", cs.foo);
    val.setProperty("bar", cs.bar);
    val.setProperty("baz", cs.baz);
    return val;
}


void fromScriptValue(const QScriptValue& value, CustomStruct& cs)
{
    cs.foo = value.property("foo").toInt32();
    cs.bar = (double)(value.property("bar").toNumber());
    cs.baz = value.property("baz").toString().toStdString().at(0);
}
