/**
 * @file
 * @brief Defines the adapter for MyScriptAPI.
 * @author Perttu Paarlahti 2016.
 */

#ifndef QTSCRIPTAPIADAPTER_HH
#define QTSCRIPTAPIADAPTER_HH

#include <QObject>
#include <QDateTime>
#include <QScriptValue>
#include <QScriptEngine>
#include "myscriptapi.hh"

/**
 * @brief Adapts the MyScriptAPI interface suitable for QtScript.
 */
class QtScriptApiAdapter : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief Constructor.
     * @param api API object.
     * @param engine QScriptEngine.
     * @pre api != nullptr, engine != nullptr.
     */
    QtScriptApiAdapter(MyScriptAPI* api,
                       QScriptEngine* engine,
                       QObject* parent = 0);

    ~QtScriptApiAdapter();

public slots:

    // Adapter methods for MyAPI
    void sayHello();
    void saySomething(const QString& msg, int repeat);
    QDateTime timeNow();
    QScriptValue doStuffToMyStruct(const QScriptValue& myStruct);


private:

    MyScriptAPI* api_;
    QScriptEngine* eng_;
};


/**
 * @brief Converts CustomStruct to QScriptValue.
 * @param eng QScriptEngine required for creating QScriptValue.
 * @param cs Source object
 * @return QScriptValue representing CustomStruct.
 */
QScriptValue toScriptValue(QScriptEngine* eng, const CustomStruct& cs);

/**
 * @brief Convert QScriptValue to CustomStruct.
 * @param value Source value.
 * @param cs Target object,
 */
void fromScriptValue(const QScriptValue& value, CustomStruct& cs);

#endif // QTSCRIPTAPIADAPTER_HH
