/**
 * @file
 * @brief Defines the Configuration class and auxiliary structs required
 * for setting up the ScriptEmbedder component.
 * @author Perttu Paarlahti 2016
 */

#ifndef CONFIGURATION_HH
#define CONFIGURATION_HH

#include <QString>
#include <map>
#include <memory>
#include "scriptapi.hh"

namespace ScriptEmbedderNS
{

// Forward declaration for user-defined scipt API.
class ScriptAPI;


/**
 * @brief Represents configuration for single script entity.
 */
struct ScriptEntry
{
    /**
     * @brief Unique id for script entity.
     */
    unsigned id;

    /**
     * @brief Path to script source file.
     */
    QString scriptPath;

    /**
     * @brief Script language (must match some of interpreter script languages).
     */
    QString scriptLanguage;

    /**
     * @brief If true, script read to RAM in configuration. Else it is read
     * from disk on demand. Reading scripts to RAM makes execution faster, but
     * requires constantly more memory.
     */
    bool readToRAM;

    /**
     * @brief Sctipt's priority. This has effect only in asynchronous mode.
     * 0 is the highest priority.
     */
    unsigned priority;
};


/**
 * @brief Represents configuration for single sctipt interpreter.
 */
struct InterpreterEntry
{
    /**
     * @brief Name of interpreted language.
     */
    QString ScriptLanguage;

    /**
     * @brief Path to interpreter's plugin binary file.
     */
    QString pluginPath;

};


/**
 * @brief Represents the ScriptEmbedder configutation.
 * Contains neccessary information about interpreters, predefined scripts
 * and the ScriptAPI object.
 */
class Configuration
{
public:

    /**
     * @brief Default constructor. Creates Configuration with no api,
     * interpreters or scripts set. This configuration is not valid.
     * @pre -
     * @post Configuration object is invalid, and has no api, interpreters or
     * scripts set.
     */
    Configuration();

    /**
     * @brief Constructor. Creates Configuration from given parameters.
     * @param api User defined ScriptAPI.
     * @param interpreters Required interpreters in map. Scrip language as key.
     * @param scripts Predefined scripts in map. Script id as key.
     * @pre -
     * @post given parameters have been set as api, interpreters and scripts.
     * Configuration may not be valid.
     */
    Configuration(std::shared_ptr<ScriptAPI> api,
                  std::map<QString, InterpreterEntry> interpreters,
                  std::map<unsigned, ScriptEntry> scripts);

    /**
     * @brief Assign script api.
     * @param api Used script api object.
     * @pre api != nullptr.
     * @post ScriptAPI has been set for this configuration.
     * If api was already set, it is replaced with the new one.
     */
    void setScriptAPI(std::shared_ptr<ScriptAPI> api);

    /**
     * @brief Get currently assigned ScriptAPI.
     * @return ScriptAPI object used in this configuration.
     * @pre -
     */
    std::shared_ptr<ScriptAPI> scriptAPI() const;

    /**
     * @brief Add new interpreter.
     * @param interpreter New interpreter.
     * @pre Iterpreter's scriptLanguate or pluginPath is not an empty string.
     * @post If there already is an interpreter for same language, it will be
     *  replaced with the new one.
     */
    void addInterpreter(const InterpreterEntry& interpreter);

    /**
     * @brief Get currently assigned interpreters.
     * @return Current interpreters in map. Script language is the key.
     * @pre -
     */
    std::map<QString, InterpreterEntry> interpreters() const;

    /**
     * @brief Add new script entry in configuration.
     * @param script New entry.
     * @pre -
     * @post If an entry with same id already exists, it will be replaced.
     */
    void addScript(const ScriptEntry& script);

    /**
     * @brief Get assigned scripts.
     * @return Currently set scripts in map. Script id is the key.
     * @pre -
     */
    std::map<unsigned, ScriptEntry> scripts() const;

    /**
     * @brief Check if current configuration is valid.
     * Configuration is considered valid if:
     * 1) ScriptApi has been set.
     * 2) There is at liest one interpreter set.
     * 3) Each script has a suitable interpreter set (languages match).
     * 4) All script paths point to an existing file.
     * 5) All plugin paths point to an existing library file.
     * No other validation is made at this point.
     * @return True, if configuration is valid.
     * @pre -
     */
    bool isValid() const;

    /**
     * @brief Get more information on why configuration is invalid.
     * @return If configuration is invalid, this returns more detailed
     * description. Else returns empty string.
     */
    QString errorString() const;


private:

    std::shared_ptr<ScriptAPI> api_;
    std::map<QString, InterpreterEntry> interpreters_;
    std::map<unsigned, ScriptEntry> scripts_;
};

} // namespace ScriptEmbedderNS

#endif // CONFIGURATION_HH
