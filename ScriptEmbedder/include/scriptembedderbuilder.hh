/**
 * @file
 * @brief Defines the ScriptEmbedderBuilder class, which is responsible for
 * instantiating different implementations for the ScriptEmbedder interface.
 * @author Perttu Paarlahti 2016.
 */

#ifndef SCRIPTEMBEDDERBUILDER_HH
#define SCRIPTEMBEDDERBUILDER_HH

#include "scriptembedder.hh"
#include "configuration.hh"

namespace ScriptEmbedderNS
{

/**
 * @brief Builder class for different ScriptEmbedder implementations.
 */
class ScriptEmbedderBuilder
{
public:

    /**
     * @brief Instantiate non-asynchronous ScriptEmbedder.
     * @param conf Configuration.
     * @return New instance of ScriptEmbedder, that does not execute scripts
     * asynchronously with the calling thread. Ownership is passed to the caller.
     * @pre Configuration is valid.
     */
    static ScriptEmbedder* createSerialEmbedder(const Configuration& conf);
};

} // namespace ScriptEmbedderNS

#endif // SCRIPTEMBEDDERBUILDER_HH
