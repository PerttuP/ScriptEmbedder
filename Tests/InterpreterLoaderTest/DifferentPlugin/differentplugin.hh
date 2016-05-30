/**
 * @file
 * @brief Defines a plugin stub for testing InterpreterLoader in erroneous cases.
 * @author Perttu Paarlahti 2016.
 */

#ifndef DIFFERENTPLUGIN_HH
#define DIFFERENTPLUGIN_HH

#include <QObject>
#include <cstdio>
#include "differentplugininterface.hh"


class DifferentPlugin : public QObject, public DifferentPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(DifferentPluginInterface)
    Q_PLUGIN_METADATA(IID DIFFERENT_PLUGIN_IID)

public:

    DifferentPlugin() {}

    virtual ~DifferentPlugin() {}

    virtual void sayHello() const
    {
        printf("Hello world!");
    }
};

#endif // DIFFERENTPLUGIN_HH
