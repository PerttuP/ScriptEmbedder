#include <QString>
#include <QtTest>
#include "serialscriptembedder.hh"

#ifdef Q_OS_WIN
const QString PLUGIN_PATH = "../InterpreterTestPlugin/debug/InterpreterTestPlugin.dll";
const QString DIFF_PLUGIN_PATH = "../../InterpreterLoaderTest/DifferentPlugin/debug/DifferentPlugin.dll";
const QString LIB_POSTFIX = ".dll";
#else
const QString PLUGIN_PATH = "../InterpreterTestPlugin/InterpreterTestPlugin.so";
const QString DIFF_PLUGIN_PATH = "../../InterpreterLoaderTest/DifferentPlugin/DifferentPlugin.dll";
const QString LIB_POSTFIX = ".so";
#endif


typedef std::map<QString, ScriptEmbedderNS::InterpreterEntry> InterpreterMap;
typedef std::map<unsigned, ScriptEmbedderNS::ScriptEntry> ScriptMap;
Q_DECLARE_METATYPE(InterpreterMap)
Q_DECLARE_METATYPE(ScriptMap)
Q_DECLARE_METATYPE(std::shared_ptr<ScriptEmbedderNS::ScriptAPI>)


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
        for (auto it = interpreters.begin(); it != interpreters.end(); ++it){
            QPluginLoader loader(it->second.pluginPath);
            QVERIFY(loader.isLoaded());
        }
    }
    else {
        // Conf is invalid.
        QVERIFY(embedder.configuration().scriptAPI() == nullptr);
        QVERIFY(embedder.configuration().scripts() == ScriptMap());
        QVERIFY(embedder.configuration().interpreters() == InterpreterMap());
        for (auto it = interpreters.begin(); it != interpreters.end(); ++it){
            QPluginLoader loader(it->second.pluginPath);
            QVERIFY(!loader.isLoaded());
        }
    }
}


void SerialScriptEmbedderTest::resetTest_data()
{
    constructorTest_data();
}


QTEST_APPLESS_MAIN(SerialScriptEmbedderTest)

#include "tst_serialscriptembeddertest.moc"
