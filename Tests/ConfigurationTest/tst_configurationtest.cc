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
Q_DECLARE_METATYPE( ScriptEmbedderNS::InterpreterEntry )
Q_DECLARE_METATYPE( ScriptEmbedderNS::ScriptEntry )


/**
 * @brief ScriptAPI extension for testing purposes.
 */
class CustomAPI : public ScriptEmbedderNS::ScriptAPI
{
public:
    CustomAPI() {}
    virtual ~CustomAPI() {}
    void testMethod() {}
};


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
     * @brief Test equality comparison for InterpreterEntry.
     */
    void interpreterComparisonTest();
    void interpreterComparisonTest_data();

    /**
     * @brief Test equality comparison for ScriptEntry.
     */
    void scriptComparisonTest();
    void scriptComparisonTest_data();

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

    /**
     * @brief Test setting and getting API.
     */
    void apiSetterGetterTest();

    /**
     * @brief Test the addInterpreter method.
     */
    void addRemoveInterpreterTest();

    /**
     * @brief Test the addScript method.
     */
    void addRemoveScriptTest();
};

ConfigurationTest::ConfigurationTest()
{
}


void ConfigurationTest::interpreterComparisonTest()
{
    QFETCH(ScriptEmbedderNS::InterpreterEntry, entry1);
    QFETCH(ScriptEmbedderNS::InterpreterEntry, entry2);
    QFETCH(bool, equals);

    QCOMPARE(entry1 == entry2, equals);
    QCOMPARE(entry2 == entry1, equals);

    if (entry1 == entry2){
        QCOMPARE(entry1.scriptLanguage, entry2.scriptLanguage);
        QCOMPARE(entry1.pluginPath, entry2.pluginPath);
    }
}


void ConfigurationTest::interpreterComparisonTest_data()
{
    QTest::addColumn<ScriptEmbedderNS::InterpreterEntry>("entry1");
    QTest::addColumn<ScriptEmbedderNS::InterpreterEntry>("entry2");
    QTest::addColumn<bool>("equals");

    using namespace ScriptEmbedderNS;

    QTest::newRow("equals")
            << InterpreterEntry {"Python", "path1"}
            << InterpreterEntry {"Python", "path1"}
            << true;

    QTest::newRow("different name and path")
            << InterpreterEntry {"Python", "path1"}
            << InterpreterEntry {"JavaScript", "path2"}
            << false;

    QTest::newRow("same lang, different path")
            << InterpreterEntry {"Python", "path1"}
            << InterpreterEntry {"Python", "path2"}
            << false;

    QTest::newRow("same path, different lang")
            << InterpreterEntry {"Python", "path1"}
            << InterpreterEntry {"JavaScript", "path1"}
            << false;
}


void ConfigurationTest::scriptComparisonTest()
{
    QFETCH(ScriptEmbedderNS::ScriptEntry, entry1);
    QFETCH(ScriptEmbedderNS::ScriptEntry, entry2);
    QFETCH(bool, equals);

    QCOMPARE(entry1 == entry2, equals);
    QCOMPARE(entry2 == entry1, equals);

    if (entry1 == entry2){
        QCOMPARE(entry1.id, entry2.id);
        QCOMPARE(entry1.scriptPath, entry2.scriptPath);
        QCOMPARE(entry1.scriptLanguage, entry2.scriptLanguage);
        QCOMPARE(entry1.readToRAM, entry2.readToRAM);
        QCOMPARE(entry1.priority, entry2.priority);
    }
}


void ConfigurationTest::scriptComparisonTest_data()
{
    using namespace ScriptEmbedderNS;
    QTest::addColumn<ScriptEntry>("entry1");
    QTest::addColumn<ScriptEntry>("entry2");
    QTest::addColumn<bool>("equals");

    QTest::newRow("equals")
            << ScriptEntry {0u, "scriptPath1", "Python", false, 0u}
            << ScriptEntry {0u, "scriptPath1", "Python", false, 0u}
            << true;

    QTest::newRow("different id")
            << ScriptEntry {0u, "scriptPath1", "Python", false, 0u}
            << ScriptEntry {1u, "scriptPath1", "Python", false, 0u}
            << false;

    QTest::newRow("different path")
            << ScriptEntry {0u, "scriptPath1", "Python", false, 0u}
            << ScriptEntry {0u, "scriptPath2", "Python", false, 0u}
            << false;

    QTest::newRow("different language")
            << ScriptEntry {0u, "scriptPath1", "Python", false, 0u}
            << ScriptEntry {0u, "scriptPath1", "JavaScript", false, 0u}
            << false;

    QTest::newRow("different readToRAM")
            << ScriptEntry {0u, "scriptPath1", "Python", false, 0u}
            << ScriptEntry {0u, "scriptPath1", "Python", true, 0u}
            << false;

    QTest::newRow("different priority")
            << ScriptEntry {0u, "scriptPath1", "Python", false, 0u}
            << ScriptEntry {0u, "scriptPath1", "Python", false, 1u}
            << false;

    QTest::newRow("all different")
            << ScriptEntry {0u, "scriptPath1", "Python", false, 0u}
            << ScriptEntry {1u, "scriptPath2", "JavaScript", true, 1u}
            << false;
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


void ConfigurationTest::apiSetterGetterTest()
{
    using namespace ScriptEmbedderNS;
    Configuration c;
    QVERIFY(c.scriptAPI() == nullptr);

    std::shared_ptr<ScriptAPI> defaultApi(new ScriptAPI());
    c.setScriptAPI(defaultApi);
    QVERIFY(c.scriptAPI() == defaultApi);

    std::shared_ptr<ScriptAPI> customApi(new CustomAPI());
    c.setScriptAPI(customApi);
    QVERIFY(c.scriptAPI() == customApi);
}


void ConfigurationTest::addRemoveInterpreterTest()
{
    using namespace ScriptEmbedderNS;
    Configuration c;
    QVERIFY(c.interpreters().empty());

    // Add new interpreters.
    InterpreterEntry i1 {"Python", "sriptpath/pythonplugin"};
    InterpreterEntry i2 {"JavaScript", "scriptpath/javascriptplugin"};
    c.addInterpreter(i1);
    QCOMPARE(c.interpreters().size(), InterpreterMap::size_type(1));
    QCOMPARE(c.interpreters().at("Python"), i1);
    c.addInterpreter(i2);
    QCOMPARE(c.interpreters().size(), InterpreterMap::size_type(2));
    QCOMPARE(c.interpreters().at("Python"), i1);
    QCOMPARE(c.interpreters().at("JavaScript"), i2);

    // Replace existing interpreter
    InterpreterEntry i3 {"Python", "scriptpath/betterpythonplugin"};
    c.addInterpreter(i3);
    QCOMPARE(c.interpreters().size(), InterpreterMap::size_type(2));
    QCOMPARE(c.interpreters().at("Python"), i3);
    QCOMPARE(c.interpreters().at("JavaScript"), i2);

    // Remove interpreter.
    c.removeInterpreter("Python");
    InterpreterMap interpreters = c.interpreters();
    QCOMPARE(interpreters.size(), InterpreterMap::size_type(1));
    QVERIFY(interpreters.find("Python") == interpreters.end() );
    QCOMPARE(interpreters.at("JavaScript"), i2);

    // Remove non-existing interpreter.
    c.removeInterpreter("Python");
    interpreters = c.interpreters();
    QCOMPARE(interpreters.size(), InterpreterMap::size_type(1));
    QVERIFY(interpreters.find("Python") == interpreters.end() );
    QCOMPARE(interpreters.at("JavaScript"), i2);
}


void ConfigurationTest::addRemoveScriptTest()
{
    using namespace ScriptEmbedderNS;
    Configuration c;
    QVERIFY(c.scripts().empty());

    // Add new scripts
    ScriptEntry s1 {0u, "path1", "Python", false, 0u};
    ScriptEntry s2 {1u, "path2", "JavaScript", true, 1u};
    c.addScript(s1);
    QCOMPARE(c.scripts().size(), ScriptMap::size_type(1));
    QCOMPARE(c.scripts().at(0u), s1);
    c.addScript(s2);
    QCOMPARE(c.scripts().size(), ScriptMap::size_type(2));
    QCOMPARE(c.scripts().at(0u), s1);
    QCOMPARE(c.scripts().at(1u), s2);

    // Replace existing script
    ScriptEntry s3 {0u, "path3", "Perl", true, 3u};
    c.addScript(s3);
    QCOMPARE(c.scripts().size(), ScriptMap::size_type(2));
    QCOMPARE(c.scripts().at(0u), s3);
    QCOMPARE(c.scripts().at(1u), s2);

    // Remove script
    c.removeScript(0u);
    ScriptMap scripts = c.scripts();
    QCOMPARE(scripts.size(), ScriptMap::size_type(1));
    QVERIFY(scripts.find(0u) == scripts.end());
    QCOMPARE(scripts.at(1u), s2);

    // Remove non-existing script
    c.removeScript(0u);
    scripts = c.scripts();
    QCOMPARE(scripts.size(), ScriptMap::size_type(1));
    QVERIFY(scripts.find(0u) == scripts.end());
    QCOMPARE(scripts.at(1u), s2);
}


QTEST_APPLESS_MAIN(ConfigurationTest)

#include "tst_configurationtest.moc"
