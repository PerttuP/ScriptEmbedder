/**
 * @file
 * @brief Unit tests for the InterpreterLoader test.
 * @author Perttu Paarlahti 2016.
 */

#include <QString>
#include <QtTest>

#include "configuration.hh"
#include "interpreterloader.hh"

Q_DECLARE_METATYPE(ScriptEmbedderNS::InterpreterEntry)


/**
 * @brief Unit tests for the InterpreterLoader class.
 */
class InterpreterLoaderTest : public QObject
{
    Q_OBJECT

public:
    InterpreterLoaderTest();

private Q_SLOTS:

    /**
     * @brief Tests the loader with valid entry.
     */
    void interpreterLoaderSuccess();

    /**
     * @brief Tests the loader with invalid entries.
     */
    void interpreterLoaderFailure();
    void interpreterLoaderFailure_data();
};


InterpreterLoaderTest::InterpreterLoaderTest()
{
}


void InterpreterLoaderTest::interpreterLoaderSuccess()
{
    using namespace ScriptEmbedderNS;
#ifdef Q_OS_WIN
    InterpreterEntry entry("TestLanguage", "../InterpreterPluginStub/debug/InterpreterPluginStub.dll");
#else
    InterpreterEntry entry("TestLanguage", "../InterpreterPluginStub/InterpreterPluginStub.so");
#endif

    // Load interpreter.
    InterpreterLoader loader(entry);
    InterpreterPlugin* plugin = loader.instance();
    QVERIFY(plugin != nullptr);
    QCOMPARE(loader.errorString(), QString());
    QCOMPARE(plugin->language(), entry.scriptLanguage);

    // Other instance
    InterpreterPlugin* plugin2 = loader.instance();
    QVERIFY(plugin2 != nullptr);
    QCOMPARE(loader.errorString(), QString());
    QCOMPARE(plugin2->language(), entry.scriptLanguage);
    QVERIFY(plugin == plugin2);

    // Verify that plugin is loaded.
    QPluginLoader pluginLoader(entry.pluginPath);
    QVERIFY(pluginLoader.isLoaded());

    // Unload plugin.
    loader.unloadPlugin();
    QVERIFY(!pluginLoader.isLoaded());

    // Reload plugin.
    plugin = loader.instance();
    QVERIFY(plugin != nullptr);
    QCOMPARE(loader.errorString(), QString());
    QCOMPARE(plugin->language(), entry.scriptLanguage);

    // Cleanup
    loader.unloadPlugin();
}


void InterpreterLoaderTest::interpreterLoaderFailure()
{
    QFETCH(ScriptEmbedderNS::InterpreterEntry, entry);
    QFETCH(QString, errorString);

    // Try loading plugin.
    ScriptEmbedderNS::InterpreterLoader loader(entry);
    ScriptEmbedderNS::InterpreterPlugin* interpreter = loader.instance();
    QVERIFY(interpreter == nullptr);
    QCOMPARE(loader.errorString(), errorString);

    // Plugin should not be loaded.
    QPluginLoader pluginLoader(entry.pluginPath);
    QVERIFY(!pluginLoader.isLoaded());
}


void InterpreterLoaderTest::interpreterLoaderFailure_data()
{
    using namespace ScriptEmbedderNS;
    QTest::addColumn<InterpreterEntry>("entry");
    QTest::addColumn<QString>("errorString");

#ifdef Q_OS_WIN
    QString PLUGIN_PATH = "../InterpreterPluginStub/debug/InterpreterPluginStub.dll";
    QString DIFFERENT_PLUGIN_PATH = "../DifferentPlugin/debug/DifferentPlugin.dll";
    QString LIB_POSTFIX = ".dll";
#else
    QString PLUGIN_PATH = "../InterpreterPluginStub/InterpreterPluginStub.so";
    QString DIFFERENT_PLUGIN_PATH = "../DifferentPlugin/DifferentPlugin.so";
    QString LIB_POSTFIX = ".so";
#endif

    QTest::newRow("invalid path")
            << InterpreterEntry("TestLanguage", "does_not_exist"+LIB_POSTFIX)
            << QString("Failed to load %1 plugin: %2.")
               .arg("TestLanguage").arg("does_not_exist"+LIB_POSTFIX);

    QTest::newRow("Language mismatch")
            << InterpreterEntry("DifferentLanguage", PLUGIN_PATH)
            << QString("Plugin '%1' is not for desired language '%2'.")
               .arg(PLUGIN_PATH).arg("DifferentLanguage");

    QTest::newRow("different plugin")
            << InterpreterEntry("TestLanguage", DIFFERENT_PLUGIN_PATH)
            << QString("Plugin '%1' is not an interpreter plugin.")
               .arg(DIFFERENT_PLUGIN_PATH);
}


QTEST_APPLESS_MAIN(InterpreterLoaderTest)

#include "tst_interpreterloadertest.moc"
