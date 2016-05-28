#ifndef DIFFERENTPLUGIN_HH
#define DIFFERENTPLUGIN_HH

#include <QObject>
#include "differentplugininterface.hh"


class DifferentPlugin : public QObject, public DifferentPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(DifferentPluginInterface)
    Q_PLUGIN_METADATA(IID DIFFERENT_PLUGIN_IID)

public:

    DifferentPlugin();

    virtual ~DifferentPlugin();

    virtual void sayHello() const;
};

#endif // DIFFERENTPLUGIN_HH
