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

    Configuration conf(api, interpreters, scripts);
    SerialScriptEmbedder embedder(conf);

    QVERIFY(embedder.isValid() == isValid);
    QCOMPARE(embedder.errorString(), errorStr);

    if (isValid){
        QVERIFY(embedder.configuration().scriptAPI() == api);
        QVERIFY(embedder.configuration().scripts() == scripts);
        QVERIFY(embedder.configuration().interpreters() == interpreters);
    }
    else {
        QVERIFY(embedder.configuration().scriptAPI() == nullptr);
        QVERIFY(embedder.configuration().scripts() == ScriptMap());
        QVERIFY(embedder.configuration().interpreters() == InterpreterMap());
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

    QTest::newRow("valid plugins")
            << std::shared_ptr<ScriptAPI>(new ScriptAPI())
            << InterpreterMap {{"TestLanguage", InterpreterEntry("TestLanguage", PLUGIN_PATH)}}
            << ScriptMap()
            << true << QString();
}


QTEST_APPLESS_MAIN(SerialScriptEmbedderTest)

#include "tst_serialscriptembeddertest.moc"
