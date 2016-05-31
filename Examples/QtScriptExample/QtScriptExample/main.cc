/**
 * @file
 * @brief Short example program on using ScriptEmbedder.
 * @author Perttu Paarlahti 2016.
 */

#include <QCoreApplication>
#include <QString>
#include <QDebug>
#include "scriptembedderbuilder.hh"
#include "myscriptapiimplementation.hh"


/**
 * @brief Custom implementation for the Logger interface.
 */
class MyLogger : public ScriptEmbedderNS::Logger
{
public:

    void logMessage(const QString& msg)
    {
        qDebug() << msg;
    }

    void scriptExecuted(const ScriptEmbedderNS::ScriptEntry& script, const QStringList& params, int returnValue)
    {
        qDebug() << "Success:" << script.id << "params:" << params.join(", ") << " return value:" << returnValue;
    }

    void scriptFailed(const ScriptEmbedderNS::ScriptEntry& script, const QStringList& params, const QString& errorMsg)
    {
        qDebug() << "Failure:" << script.id << "params:" << params.join(", ") << " error:" << errorMsg;
    }
};


/**
 * @brief Set configuration and instantiate embedder.
 * @return New ScriptEmbedder.
 */
std::shared_ptr<ScriptEmbedderNS::ScriptEmbedder> createEmbedder()
{
#ifdef Q_OS_WIN
    const QString PLUGIN_PATH = "../QtScriptPluginExample/debug/QtScriptPluginExample.dll";
#else
    const QString PLUGIN_PATH = "../QtScriptPluginExample/QtScriptPluginExample.so";
#endif

    using namespace ScriptEmbedderNS;
    Configuration conf;
    conf.setScriptAPI(std::shared_ptr<ScriptAPI>(new MyScriptApiImplementation()));
    conf.addInterpreter(InterpreterEntry("QtScript", PLUGIN_PATH));
    conf.addScript(ScriptEntry(0u, "scripts/hello.qs", "QtScript"));
    conf.addScript(ScriptEntry(1u, "scripts/printTime.qs", "QtScript", true));
    conf.addScript(ScriptEntry(2u, "scripts/printMyStruct.qs", "QtScript"));
    if (!conf.isValid()){
        qDebug() << conf.errorString();
        return std::shared_ptr<ScriptEmbedder>(nullptr);
    }

    return std::shared_ptr<ScriptEmbedder>(ScriptEmbedderBuilder::createSerialEmbedder(conf));
}


/**
 * @brief Example program.
 */
int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    // Instantiate ScriptEmbedder
    using namespace ScriptEmbedderNS;
    std::shared_ptr<ScriptEmbedder> embedder = createEmbedder();
    if (embedder == nullptr){
        return -2;
    }
    else if (!embedder->isValid()){
        qDebug() << embedder->errorString();
        return -1;
    }
    // Set logger
    MyLogger logger;
    embedder->setLogger(&logger);

    // Run scipts.
    embedder->execute(0);
    embedder->execute(1);
    embedder->execute(2, QStringList{"12", "3.14", "a"});
    embedder->execute(3); // No such script.

    qDebug() << "Press ^C to exit.";
    return a.exec();
}
