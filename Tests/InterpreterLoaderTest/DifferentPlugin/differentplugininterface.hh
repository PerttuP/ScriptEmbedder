#ifndef DIFFERENTPLUGININTERFACE_HH
#define DIFFERENTPLUGININTERFACE_HH

#include <QtPlugin>

#define DIFFERENT_PLUGIN_IID "PerttuP.ScriptInterpreter.DifferentPlugin"

class DifferentPluginInterface
{
public:

    virtual ~DifferentPluginInterface() {}

    virtual void sayHello() const = 0;
};

Q_DECLARE_INTERFACE(DifferentPluginInterface, DIFFERENT_PLUGIN_IID)

#endif // DIFFERENTPLUGININTERFACE_HH

