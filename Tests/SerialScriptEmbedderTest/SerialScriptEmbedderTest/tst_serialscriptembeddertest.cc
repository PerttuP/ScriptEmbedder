/**
 * @file
 * @brief Unit tests for the SerialScriptEmbedder class, implementation dor ScriptEmbedder interface.
 * @author Perttu Paarlahti 2016.
 */

#include <QString>
#include <QtTest>
#include "serialscriptembedder.hh"
#include "interpretertestplugin.hh"


#ifdef Q_OS_WIN
const QString PLUGIN_PATH = "../InterpreterTestPlugin/debug/InterpreterTestPlugin.dll";
const QString PLUGIN_PATH2 = "../../InterpreterLoaderTest/InterpreterPluginStub/debug/InterpreterPluginStub.dll";
const QString DIFF_PLUGIN_PATH = "../../InterpreterLoaderTest/DifferentPlugin/debug/DifferentPlugin.dll";
const QString LIB_POSTFIX = ".dll";
#else
const QString PLUGIN_PATH = "../InterpreterTestPlugin/InterpreterTestPlugin.so";
const QString PLUGIN_PATH2 = "../../InterpreterLoaderTest/InterpreterPluginStub/InterpreterPluginStub.so";
const QString DIFF_PLUGIN_PATH = "../../InterpreterLoaderTest/DifferentPlugin/DifferentPlugin.so";
const QString LIB_POSTFIX = ".so";
#endif


// Meta type declarations.
typedef std::map<QString, ScriptEmbedderNS::InterpreterEntry> InterpreterMap;
typedef std::map<unsigned, ScriptEmbedderNS::ScriptEntry> ScriptMap;
Q_DECLARE_METATYPE(InterpreterMap)
Q_DECLARE_METATYPE(ScriptMap)
Q_DECLARE_METATYPE(std::shared_ptr<ScriptEmbedderNS::ScriptAPI>)
Q_DECLARE_METATYPE(ScriptEmbedderNS::ScriptEntry)
Q_DECLARE_METATYPE(ScriptEmbedderNS::InterpreterEntry)


/**
 * @brief Stub implementation for the Logger interface.
 */
class LoggerStub : public ScriptEmbedderNS::Logger
{
public:

    // Members for verifying tests.
    QStringList logMessages;
    std::vector<std::tuple<ScriptEmbedderNS::ScriptEntry, QStringList, int> > successes;
    std::vector<std::tuple<ScriptEmbedderNS::ScriptEntry, QStringList, QString> > failures;

    LoggerStub() :
        ScriptEmbedderNS::Logger(), logMessages(), successes(), failures() {}

    virtual ~LoggerStub() {}

    void logMessage(const QString& msg)
    {
        logMessages.push_back(msg);
    }

    void scriptExecuted(const ScriptEmbedderNS::ScriptEntry& script,
                        const QStringList& params, int returnValue)
    {
        successes.push_back( std::make_tuple(script, params, returnValue) );
    }

    void scriptFailed(const ScriptEmbedderNS::ScriptEntry& script,
                      const QStringList& params, const QString& errorMsg)
    {
        failures.push_back( std::make_tuple(script, params, errorMsg));
    }
};


/**
 * @brief Unit tests for SerialScriptEmbedderTest.
 */
class SerialScriptEmbedderTest : public QObject
{
    Q_OBJECT

public:
    SerialScriptEmbedderTest();

private Q_SLOTS:

    /**
     * @brief constructorTest
     */
    void constructorTest();
    void constructorTest_data();

    /**
     * @brief Test reseting configuration.
     */
    void resetTest();
    void resetTest_data();

    /**
     * @brief Test the addScript method.
     */
    void addScriptTest();
    void addScriptTest_data();

    /**
     * @brief Test the removeScript method.
     */
    void removeScriptTest();

    /**
     * @brief Test the addInterpreter method.
     */
    void addInterpreterTest();
    void addInterpreterTest_data();

    /**
     * @brief Test the execute method.
     */
    void runScriptTest();
    void runScriptTest_data();
};


SerialScriptEmbedderTest::SerialScriptEmbedderTest()
{
}


void SerialScriptEmbedderTest::constructorTest()
{
    using namespace ScriptEmbedderNS;
    QFETCH(std::shared_ptr<ScriptAPI>, api);
    QFETCH(InterpreterMap, interpreters);
    QFETCH(ScriptMap, scripts);
    QFETCH(bool, isValid);
    QFETCH(QString, errorStr);

    // Create embedder.
    Configuration conf(api, interpreters, scripts);
    SerialScriptEmbedder embedder(conf);

    // Verify results
    QVERIFY(embedder.isValid() == isValid);
    QCOMPARE(embedder.errorString(), errorStr);

    if (isValid){
        // New conf is set.
        QVERIFY(embedder.configuration().isValid());
        QVERIFY(embedder.configuration().scriptAPI() == api);
        QVERIFY(embedder.configuration().scripts() == scripts);
        QVERIFY(embedder.configuration().interpreters() == interpreters);
        for (auto it = interpreters.begin(); it != interpreters.end(); ++it){
            QPluginLoader loader(it->second.pluginPath);
            QVERIFY(loader.isLoaded());
            QVERIFY(((InterpreterTestPlugin*)(loader.instance()))->api == api);
        }
    }
    else {
        // Conf is invalid.
        QVERIFY(!embedder.configuration().isValid());
        QVERIFY(embedder.configuration().scriptAPI() == nullptr);
        QVERIFY(embedder.configuration().scripts() == ScriptMap());
        QVERIFY(embedder.configuration().interpreters() == InterpreterMap());
        for (auto it = interpreters.begin(); it != interpreters.end(); ++it){
            QPluginLoader loader(it->second.pluginPath);
            QVERIFY(!loader.isLoaded());
        }
    }
}


void SerialScriptEmbedderTest::constructorTest_data()
{
    using namespace ScriptEmbedderNS;
    QTest::addColumn<std::shared_ptr<ScriptAPI> >("api");
    QTest::addColumn<InterpreterMap>("interpreters");
    QTest::addColumn<ScriptMap>("scripts");
    QTest::addColumn<bool>("isValid");
    QTest::addColumn<QString>("errorStr");

    const QString TEST_PATH = "testfiles/";

    QTest::newRow("valid plugins, no scripts")
            << std::shared_ptr<ScriptAPI>(new ScriptAPI())
            << InterpreterMap {{"TestLanguage", InterpreterEntry("TestLanguage", PLUGIN_PATH)}}
            << ScriptMap()
            << true << QString();

    QTest::newRow("valid plugins, one RAM script")
            << std::shared_ptr<ScriptAPI>(new ScriptAPI())
            << InterpreterMap {{"TestLanguage", InterpreterEntry("TestLanguage", PLUGIN_PATH)}}
            << ScriptMap {{0u, ScriptEntry(0u, TEST_PATH+"testscript.txt", "TestLanguage", true, 0u)}}
            << true << QString();

    QTest::newRow("valid plugins, two scripts")
            << std::shared_ptr<ScriptAPI>(new ScriptAPI())
            << InterpreterMap {{"TestLanguage", InterpreterEntry("TestLanguage", PLUGIN_PATH)}}
            << ScriptMap {{0u, ScriptEntry(0u, TEST_PATH+"testscript.txt", "TestLanguage", true, 0u)},
                          {0u, ScriptEntry(1u, TEST_PATH+"empty.txt", "TestLanguage", true, 0u)} }
            << true << QString();

    QTest::newRow("not a interpreter plugin")
            << std::shared_ptr<ScriptAPI>(new ScriptAPI())
            << InterpreterMap {{"TestLanguage", InterpreterEntry("TestLanguage", DIFF_PLUGIN_PATH)}}
            << ScriptMap()
            << false
            << QString("Configuration failed: Plugin '%1' is not an interpreter plugin.")
               .arg(DIFF_PLUGIN_PATH);

    QTest::newRow("Language mismatch")
            << std::shared_ptr<ScriptAPI>(new ScriptAPI())
            << InterpreterMap {{"OtherLanguage", InterpreterEntry("OtherLanguage", PLUGIN_PATH)}}
            << ScriptMap()
            << false
            << QString("Configuration failed: Plugin '%1' is not for desired language '%2'.")
               .arg(PLUGIN_PATH).arg("OtherLanguage");

    QTest::newRow("Loading plugin fails")
            << std::shared_ptr<ScriptAPI>(new ScriptAPI())
            << InterpreterMap {{"TestLanguage", InterpreterEntry("TestLanguage", "does_not_exist" + LIB_POSTFIX)}}
            << ScriptMap()
            << false
            << QString("Configuration failed: Failed to load %1 plugin: %2.")
               .arg("TestLanguage").arg("does_not_exist" + LIB_POSTFIX);

    QTest::newRow("Empty file to RAM")
            << std::shared_ptr<ScriptAPI>(new ScriptAPI())
            << InterpreterMap {{"TestLanguage", InterpreterEntry("TestLanguage", PLUGIN_PATH)}}
            << ScriptMap {{0u, ScriptEntry(0u, TEST_PATH+"empty.txt", "TestLanguage", true, 0u)}}
            << false
            << QString("Configuration failed: source file '%1' for script '%2' does not open or is empty.")
               .arg(TEST_PATH + "empty.txt").arg(0u);
}


void SerialScriptEmbedderTest::resetTest()
{
    using namespace ScriptEmbedderNS;
    QFETCH(std::shared_ptr<ScriptAPI>, api);
    QFETCH(InterpreterMap, interpreters);
    QFETCH(ScriptMap, scripts);
    QFETCH(bool, isValid);
    QFETCH(QString, errorStr);

    // Set up originsl embedder configuration.
    Configuration original;
    original.setScriptAPI(std::shared_ptr<ScriptAPI>(new ScriptAPI));
    original.addInterpreter(InterpreterEntry("TestLanguage", PLUGIN_PATH));
    original.addScript(ScriptEntry(0u, "testfiles/testscript.txt", "TestLanguage", true, 0u));
    original.addScript(ScriptEntry(1u, "testfiles/empty.txt", "TestLanguage", false, 1u));
    QVERIFY(original.isValid());
    SerialScriptEmbedder embedder(original);
    QVERIFY(embedder.isValid());
    QCOMPARE(embedder.errorString(), QString());
    LoggerStub logger;
    embedder.setLogger(&logger);

    // Reset
    Configuration conf(api, interpreters, scripts);
    embedder.reset(conf);

    // Verify results
    QVERIFY(embedder.isValid() == isValid);
    QCOMPARE(embedder.errorString(), errorStr);

    if (isValid){
        // New conf is set
        QVERIFY(embedder.configuration().scriptAPI() == api);
        QVERIFY(embedder.configuration().scripts() == scripts);
        QVERIFY(embedder.configuration().interpreters() == interpreters);

        // New plugins are loaded.
        for (auto it = interpreters.begin(); it != interpreters.end(); ++it){
            QPluginLoader loader(it->second.pluginPath);
            QVERIFY(loader.isLoaded());
            QVERIFY(((InterpreterTestPlugin*)(loader.instance()))->api == api);
        }
        // Old, unused plugins are unloaded
        InterpreterMap originalInterpreter = original.interpreters();
        for (auto it=originalInterpreter.begin(); it!=originalInterpreter.end(); ++it){
            QPluginLoader loader(it->second.pluginPath);
            auto it2 = interpreters.find(it->first);
            bool loaded = it2==interpreters.end() ? false : it2->second == it->second;
            QCOMPARE(loader.isLoaded(), loaded);
        }
        // Check log message.
        QCOMPARE(logger.logMessages.size(), QStringList::size_type(1));
        QCOMPARE(logger.logMessages.at(0), QString("Configuration set successfully."));

    }
    else {
        // Conf is invalid.
        QVERIFY(embedder.configuration().scriptAPI() == nullptr);
        QVERIFY(embedder.configuration().scripts() == ScriptMap());

        // New plugins are not loaded
        QVERIFY(embedder.configuration().interpreters() == InterpreterMap());
        for (auto it = interpreters.begin(); it != interpreters.end(); ++it){
            QPluginLoader loader(it->second.pluginPath);
            QVERIFY(!loader.isLoaded());
        }
        // Old plugins are unloaded.
        InterpreterMap originalInterpreter = original.interpreters();
        for (auto it=originalInterpreter.begin(); it!=originalInterpreter.end(); ++it){
            QPluginLoader loader(it->second.pluginPath);
            QVERIFY(!loader.isLoaded());
        }
        // Check log message.
        QCOMPARE(logger.logMessages.size(), QStringList::size_type(1));
        QCOMPARE(logger.logMessages.at(0), errorStr);
    }
}


void SerialScriptEmbedderTest::resetTest_data()
{
    constructorTest_data();
}


void SerialScriptEmbedderTest::addScriptTest()
{
    using namespace ScriptEmbedderNS;
    QFETCH(InterpreterMap, interpreters);
    QFETCH(ScriptMap, registeredScripts);
    QFETCH(ScriptEntry, script);
    QFETCH(bool, success);
    QFETCH(QString, errorStr);
    QFETCH(QString, logMsg);

    // Initialize embedder
    std::shared_ptr<ScriptAPI> api(new ScriptAPI());
    Configuration conf(api, interpreters, registeredScripts);
    SerialScriptEmbedder embedder(conf);
    QVERIFY(embedder.isValid());
    LoggerStub logger;
    embedder.setLogger(&logger);

    // Add script.
    bool ok = embedder.addScript(script);
    QVERIFY(ok == success);
    QCOMPARE(embedder.errorString(), errorStr);
    QCOMPARE(logger.logMessages.size(), QStringList::size_type(1));
    QCOMPARE(logger.logMessages.at(0), logMsg);

    // Check current scripts
    ScriptMap scriptsNow = embedder.configuration().scripts();
    for (auto it = registeredScripts.begin(); it != registeredScripts.end(); ++it){
        auto it2 = scriptsNow.find(it->first);
        QVERIFY(it2 != scriptsNow.end());
        if (it->second.id != script.id && success){
            QCOMPARE(it->second, it2->second);
        }
    }
    QCOMPARE(scriptsNow.find(script.id) != scriptsNow.end(),
             success || registeredScripts.find(script.id) != registeredScripts.end());
    if (success){
        QCOMPARE(script, scriptsNow.at(script.id));
    }
}


void SerialScriptEmbedderTest::addScriptTest_data()
{
    using namespace ScriptEmbedderNS;
    QTest::addColumn<InterpreterMap>("interpreters");
    QTest::addColumn<ScriptMap>("registeredScripts");
    QTest::addColumn<ScriptEntry>("script");
    QTest::addColumn<bool>("success");
    QTest::addColumn<QString>("errorStr");
    QTest::addColumn<QString>("logMsg");

    const QString TEST_PATH = "testfiles/";

    QTest::newRow("valid to RAM")
            << InterpreterMap {{"TestLanguage", InterpreterEntry("TestLanguage", PLUGIN_PATH)}}
            << ScriptMap()
            << ScriptEntry(0u, TEST_PATH+"testscript.txt", "TestLanguage", true, 1u)
            << true
            << QString() << QString("Script '%1' added.").arg(0u);

    QTest::newRow("valid not to RAM")
            << InterpreterMap {{"TestLanguage", InterpreterEntry("TestLanguage", PLUGIN_PATH)}}
            << ScriptMap()
            << ScriptEntry(1u, TEST_PATH+"empty.txt", "TestLanguage", false, 1u)
            << true
            << QString() << QString("Script '%1' added.").arg(1u);

    QTest::newRow("Already exists")
            << InterpreterMap {{"TestLanguage", InterpreterEntry("TestLanguage", PLUGIN_PATH)}}
            << ScriptMap {{1u, ScriptEntry(1u, TEST_PATH+"testscript.txt", "TestLanguage", true, 1u)}}
            << ScriptEntry(1u, TEST_PATH+"testscript.txt", "TestLanguage", true, 1u)
            << true
            << QString() << QString("Script '%1' already exists.").arg(1u);

    QTest::newRow("Replace")
            << InterpreterMap {{"TestLanguage", InterpreterEntry("TestLanguage", PLUGIN_PATH)}}
            << ScriptMap {{1u, ScriptEntry(1u, TEST_PATH+"testscript.txt", "TestLanguage", true, 1u)}}
            << ScriptEntry(1u, TEST_PATH+"empty.txt", "TestLanguage", false, 1u)
            << true
            << QString() << QString("Script '%1' replaced.").arg(1u);

    QTest::newRow("File does not exist")
            << InterpreterMap {{"TestLanguage", InterpreterEntry("TestLanguage", PLUGIN_PATH)}}
            << ScriptMap()
            << ScriptEntry(1u, "nofile.txt", "TestLanguage", false, 1u)
            << false
            << QString("Could not add script: file '%1' does not exist.").arg("nofile.txt")
            << QString("Could not add script: file '%1' does not exist.").arg("nofile.txt");

    QTest::newRow("Empty script to RAM")
            << InterpreterMap {{"TestLanguage", InterpreterEntry("TestLanguage", PLUGIN_PATH)}}
            << ScriptMap {{1u, ScriptEntry(1u, TEST_PATH+"empty.txt", "TestLanguage", false, 1u)}}
            << ScriptEntry(1u, TEST_PATH+"empty.txt", "TestLanguage", true, 1u)
            << false
            << QString("Could not add script %1: File '%2' does not open or is empty.").arg(1u).arg(TEST_PATH+"empty.txt")
            << QString("Could not add script %1: File '%2' does not open or is empty.").arg(1u).arg(TEST_PATH+"empty.txt");

    QTest::newRow("No interpreter")
            << InterpreterMap {{"TestLanguage", InterpreterEntry("TestLanguage", PLUGIN_PATH)}}
            << ScriptMap()
            << ScriptEntry(0u, TEST_PATH+"testscript.txt", "DiffLang", true, 1u)
            << false
            << QString("Could not add script '%1': No suitable interpreter for language '%2'.").arg(0u).arg("DiffLang")
            << QString("Could not add script '%1': No suitable interpreter for language '%2'.").arg(0u).arg("DiffLang");
}


void SerialScriptEmbedderTest::removeScriptTest()
{
    using namespace ScriptEmbedderNS;
    // Initialize embedder.
    Configuration conf;
    conf.setScriptAPI(std::shared_ptr<ScriptAPI>(new ScriptAPI()));
    conf.addInterpreter(InterpreterEntry("TestLanguage", PLUGIN_PATH));
    SerialScriptEmbedder embedder(conf);
    QVERIFY(embedder.isValid());
    ScriptEntry remainingScript(1u, "testfiles/empty.txt", "TestLanguage", false, 0u);
    QVERIFY(embedder.addScript(remainingScript));
    QVERIFY(embedder.addScript(ScriptEntry(0u, "testfiles/testscript.txt", "TestLanguage", true, 0u)) );
    LoggerStub logger;
    embedder.setLogger(&logger);

    // Remove existing.
    embedder.removeScript(0u);
    QVERIFY(embedder.isValid());
    QCOMPARE(embedder.errorString(), QString());
    QCOMPARE(logger.logMessages.size(), QStringList::size_type(1));
    QCOMPARE(logger.logMessages.at(0), QString("Script '%1' removed.").arg(0u));
    QCOMPARE(embedder.configuration().scripts().size(), ScriptMap::size_type(1));
    QCOMPARE(embedder.configuration().scripts().at(1u), remainingScript);


    // Remove non-existing
    embedder.removeScript(0u);
    QVERIFY(embedder.isValid());
    QCOMPARE(embedder.errorString(), QString());
    QCOMPARE(logger.logMessages.size(), QStringList::size_type(2));
    QCOMPARE(logger.logMessages.at(1), QString("Could not remove script '%1': No such script.").arg(0u));
    QCOMPARE(embedder.configuration().scripts().size(), ScriptMap::size_type(1));
    QCOMPARE(embedder.configuration().scripts().at(1u), remainingScript);
}


void SerialScriptEmbedderTest::addInterpreterTest()
{
    using namespace ScriptEmbedderNS;
    QFETCH(bool, success);
    QFETCH(InterpreterEntry, newInterpreter);
    QFETCH(QString, errorStr);
    QFETCH(QString, logMsg);

    // Initialize embedder.
    Configuration conf;
    conf.setScriptAPI(std::shared_ptr<ScriptAPI>(new ScriptAPI()));
    conf.addInterpreter(InterpreterEntry("TestLanguage", PLUGIN_PATH));
    SerialScriptEmbedder embedder(conf);
    LoggerStub logger;
    embedder.setLogger(&logger);

    // Add interpreter
    bool ok = embedder.addInterpreter(newInterpreter);
    QVERIFY(ok == success);
    QCOMPARE(embedder.errorString(), errorStr);
    QCOMPARE(logger.logMessages.size(), QStringList::size_type(1));
    QCOMPARE(logger.logMessages.at(0), logMsg);
    if (success){
        QCOMPARE(embedder.configuration().interpreters().at(newInterpreter.scriptLanguage), newInterpreter);
    }
    else {
        InterpreterMap interpreters = embedder.configuration().interpreters();
        InterpreterMap originals = conf.interpreters();
        QCOMPARE(interpreters.find(newInterpreter.scriptLanguage) != interpreters.end(),
                 originals.find(newInterpreter.scriptLanguage) != originals.end());
    }

    QPluginLoader loader(newInterpreter.pluginPath);
    QVERIFY(loader.isLoaded() == success);
}


void SerialScriptEmbedderTest::addInterpreterTest_data()
{
    using namespace ScriptEmbedderNS;
    QTest::addColumn<bool>("success");
    QTest::addColumn<InterpreterEntry>("newInterpreter");
    QTest::addColumn<QString>("errorStr");
    QTest::addColumn<QString>("logMsg");

    QTest::newRow("already exists")
            << true
            << InterpreterEntry("TestLanguage", PLUGIN_PATH)
            << QString()
            << QString("Interpreter for '%1' already exists.").arg("TestLanguage");

    QTest::newRow("not an interpreter")
            << false
            << InterpreterEntry("TestLanguage", DIFF_PLUGIN_PATH)
            << QString("Could not add interpreter: Plugin '%1' is not an interpreter plugin.").arg(DIFF_PLUGIN_PATH)
            << QString("Could not add interpreter: Plugin '%1' is not an interpreter plugin.").arg(DIFF_PLUGIN_PATH);

    QTest::newRow("invalid plugin path")
            << false
            << InterpreterEntry("TestLanguage", "notPlugin"+LIB_POSTFIX)
            << QString("Could not add interpreter: Failed to load %1 plugin: %2.").arg("TestLanguage").arg("notPlugin"+LIB_POSTFIX)
            << QString("Could not add interpreter: Failed to load %1 plugin: %2.").arg("TestLanguage").arg("notPlugin"+LIB_POSTFIX);

    QTest::newRow("replace with valid")
            << true
            << InterpreterEntry("TestLanguage", PLUGIN_PATH2)
            << QString()
            << QString("Interpreter for '%1' replaced.").arg("TestLanguage");
}


void SerialScriptEmbedderTest::runScriptTest()
{
    using namespace ScriptEmbedderNS;
    QFETCH(ScriptMap, scripts);
    QFETCH(unsigned, runId);
    QFETCH(QStringList, runParams);
    QFETCH(int, returnValue);
    QFETCH(QString, errorStr);

    // Initialize embedder
    std::shared_ptr<ScriptAPI> api(new ScriptAPI());
    InterpreterMap interpreters {{"TestLanguage", InterpreterEntry("TestLanguage", PLUGIN_PATH)}};
    Configuration conf(api, interpreters, scripts);
    SerialScriptEmbedder embedder(conf);
    LoggerStub logger;
    embedder.setLogger(&logger);

    // Initialize plugin
    QPluginLoader loader(PLUGIN_PATH);
    InterpreterTestPlugin* plugin = ((InterpreterTestPlugin*)loader.instance());
    loader.unload();
    QVERIFY(loader.isLoaded());
    QVERIFY(plugin->api == api);
    plugin->result.returnValue = returnValue;
    plugin->result.result = errorStr.isEmpty() ? ScriptInterpreter::SUCCESS : ScriptInterpreter::FAILURE;
    plugin->result.errorString = errorStr;

    // Run script and verify results.
    embedder.execute(runId, runParams);
    if (plugin->result.result == ScriptInterpreter::SUCCESS){
        // Successful case.
        QVERIFY(logger.successes.size() == 1);
        QVERIFY(logger.failures.size() == 0);
        QCOMPARE(std::get<0>(logger.successes.at(0)), scripts[runId]);
        QCOMPARE(std::get<1>(logger.successes.at(0)), runParams);
        QCOMPARE(std::get<2>(logger.successes.at(0)), returnValue);
    }
    else
    {
        // Unsuccessful case.
        QVERIFY(logger.failures.size() == 1);
        QVERIFY(logger.successes.size() == 0);
        QCOMPARE(std::get<0>(logger.failures.at(0)), scripts[runId]);
        QCOMPARE(std::get<1>(logger.failures.at(0)), runParams);
        QCOMPARE(std::get<2>(logger.failures.at(0)), errorStr);
    }

}


void SerialScriptEmbedderTest::runScriptTest_data()
{
    using namespace ScriptEmbedderNS;
    QTest::addColumn<ScriptMap>("scripts");
    QTest::addColumn<unsigned>("runId");
    QTest::addColumn<QStringList>("runParams");
    QTest::addColumn<int>("returnValue");
    QTest::addColumn<QString>("errorStr");

    QTest::newRow("success")
            << ScriptMap {{0u, ScriptEntry(0u, "testfiles/testscript.txt", "TestLanguage", true, 1u)}}
            << 0u << QStringList{"1", "2", "abc"} << 1 << QString();

    QTest::newRow("failure")
            << ScriptMap{{10u, ScriptEntry(10u, "testfiles/testscript.txt", "TestLanguage", false, 4u)}}
            << 10u << QStringList{"3,14", "asd", "0"} << 0 << QString("Syntax error");

    QTest::newRow("no such script")
            << ScriptMap() << 0u << QStringList{"1", "2", "abc"} << 0
            << QString("Script '%1' does not exist.").arg(0u);

    QTest::newRow("does not open")
            << ScriptMap{{1u, ScriptEntry(1u, "testfiles/empty.txt", "TestLanguage", false, 4u)}}
            << 1u << QStringList{"a", "b", "124"} << 0
            << QString("File '%1' does not open or is empty.").arg("testfiles/empty.txt");
}


QTEST_APPLESS_MAIN(SerialScriptEmbedderTest)

#include "tst_serialscriptembeddertest.moc"
