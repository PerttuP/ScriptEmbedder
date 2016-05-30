/**
 * @file
 * @brief Implements the ScriptEmbedderBuilder class defined in scripembedderbuilder.hh.
 * @author Perttu Paarlahti 2016.
 */

#include "scriptembedderbuilder.hh"
#include "serialscriptembedder.hh"

namespace ScriptEmbedderNS
{

ScriptEmbedder*ScriptEmbedderBuilder::createSerialEmbedder(const Configuration& conf)
{
    Q_ASSERT(conf.isValid());
    return new SerialScriptEmbedder(conf);
}

}// namespace ScriptEmbedderNS
