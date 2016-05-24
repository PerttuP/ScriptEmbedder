/**
 * @file
 * @brief Unit tests for the ScriptEmbedder::Configuration class.
 * @author Perttu Paarlahti 2016.
 */

#include <QString>
#include <QtTest>
#include "configuration.hh"

// Required to trick the Q_DECLARE_METATYPE and QFETCH
// to work with types having ',' in them.
typedef std::map<QString,ScriptEmbedderNS::InterpreterEntry> InterpreterMap;
typedef std::map<unsigned,ScriptEmbedderNS::ScriptEntry> ScriptMap;

Q_DECLARE_METATYPE( std::shared_ptr<ScriptEmbedderNS::ScriptAPI> )
Q_DECLARE_METATYPE( InterpreterMap )
Q_DECLARE_METATYPE( ScriptMap )

/**
 * @brief Unit tests for the ScriptEmbedder::Configuration class.
 */
class ConfigurationTest : public QObject
{
    Q_OBJECT

public:
    ConfigurationTest();

private Q_SLOTS:

    /**
     * @brief Test the default constructor.
     */
    void defaultConstructorTest();

    /**
     * @brief Test parametrized constructor and the
     * isValid- and errorString methods.
     */
    void constructorAndIsValidTest();
    void constructorAndIsValidTest_data();
};

ConfigurationTest::ConfigurationTest()
{
}


void ConfigurationTest::defaultConstructorTest()
{
    ScriptEmbedderNS::Configuration c;
    QVERIFY(c.scriptAPI() == nullptr);
    QVERIFY(c.interpreters().empty());
    QVERIFY(c.scripts().empty());
}


void ConfigurationTest::constructorAndIsValidTest()
{
    QFETCH(std::shared_ptr<ScriptEmbedderNS::ScriptAPI>, api);
    QFETCH(InterpreterMap, interpreters);
    QFETCH(ScriptMap, scripts);
    QFETCH(bool, isValid);
    QFETCH(QString, errorString);

    ScriptEmbedderNS::Configuration c(api, interpreters, scripts);
    QCOMPARE(c.scriptAPI(), api);
    QCOMPARE(c.interpreters(), interpreters);
    QCOMPARE(c.scripts(), scripts);
    QVERIFY(c.isValid() == isValid);
    QCOMPARE(c.errorString(), errorString);
}


void ConfigurationTest::constructorAndIsValidTest_data()
{
    QTest::addColumn<std::shared_ptr<ScriptEmbedderNS::ScriptAPI> >("api");
    QTest::addColumn<std::map<QString, ScriptEmbedderNS::InterpreterEntry> >("interpreters");
    QTest::addColumn<std::map<unsigned, ScriptEmbedderNS::ScriptEntry> >("scripts");
    QTest::addColumn<bool>("isValid");
    QTest::addColumn<QString>("errorString");

    QTest::newRow("defaults") << std::shared_ptr<ScriptEmbedderNS::ScriptAPI>(nullptr)
                              << InterpreterMap()
                              << ScriptMap()
                              << false
                              << QString("ScriptAPI has not been set.");
}

QTEST_APPLESS_MAIN(ConfigurationTest)

#include "tst_configurationtest.moc"
