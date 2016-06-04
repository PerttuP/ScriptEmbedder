/**
 * @file
 * @brief Defines empty default implementation for ScriptAPI
 * and base class for user-defined APIs.
 * @author Perttu Paarlahti 2016.
 */

#ifndef SCRIPTAPI_HH
#define SCRIPTAPI_HH

namespace ScriptEmbedderNS
{

/**
 * @brief This is the default empty implementation and base class
 * for user-defined APIs. Subclass your script api from this class.
 * if scripts are not needed to interact with the host application,
 * use this default implementation in ScriptEmbedder configuration.
 */
class ScriptAPI
{
public:

    /**
     * @brief Default contstructor.
     */
    ScriptAPI() {}

    /**
     * @brief Mandatory virtual destructor.
     */
    virtual ~ScriptAPI() {}
};


} //namespace ScriptEmbedderNS

#endif // SCRIPTAPI_HH

