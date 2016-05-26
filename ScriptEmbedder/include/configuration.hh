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

    /**
     * @brief Constructor. Sets default values for fields:
     * id = 0, scriptPath = "", scriptLanguage = "", readToRAM = false, priority = 0.
     */
    ScriptEntry();

    /**
     * @brief Constructor. Set given parametes for fields.
     * @param id Script's unique id number.
     * @param path Path to script's source code file.
     * @param language Script's language.
     * @param toRAM Is script read to RAM at configuration.
     * @param priority Script' priority (has effect only in asynchronous mode).
     * @pre Path and language are not empty strings.
     * @post New entry has given values as its attributes.
     */
    ScriptEntry(unsigned scriptId,
                const QString& path,
                const QString& language,
                bool toRAM = false,
                unsigned priority = 0);

    /**
     * @brief Comparison for equality is impemented for convenience.
     * @param rhs Compared object.
     * @return True if objects are equal (identical attributes).
     */
    bool operator==(const ScriptEntry& rhs) const;
};


/**
 * @brief Represents configuration for single sctipt interpreter.
 */
struct InterpreterEntry
{
    /**
     * @brief Name of interpreted language.
     */
    QString scriptLanguage;

    /**
     * @brief Path to interpreter's plugin binary file.
     */
    QString pluginPath;

    /**
     * @brief Costructor. Sets default values for fields:
     * scriptLanguage = "", pluginPath = "".
     *
     */
    InterpreterEntry();

    /**
     * @brief Constructor. Set given values for attributes.
     * @param language Supported scripting language.
     * @param path Path to interpreter plugin library file.
     * @pre Language and path are non-empty strings.
     */
    InterpreterEntry(const QString& language, const QString& path);

    /**
     * @brief Comparison for equality is impemented for convenience.
     * @param rhs Compared object.
     * @return True if objects are equal (identical attributes).
     */
    bool operator==(const InterpreterEntry& rhs) const;
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
     * @brief Remove interpreter for given language.
     * @param lang Language to be removed.
     * @pre -
     * @post Interpreter has been removed.
     * If no such interpreter exist, does nothing.
     */
    void removeInterpreter(const QString& lang);

    /**
     * @brief Check if there is an interpreter for given language set.
     * @param language Interpreted language.
     * @return True, if such interpreter exists.
     * @pre -
     */
    bool hasInterpreter(const QString& language) const;

    /**
     * @brief Get interpreter information for given language.
     * @param language Interpreted language.
     * @return Designated interpreter for given language. If no such
     * interpreter exists, returns default-constructed InterpreterEntry.
     * @pre -
     */
    InterpreterEntry getInterpteter(const QString& language) const;

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
     * @brief Remove script having given id.
     * @param id Id of script to be removed.
     * @pre -
     * @post Script has been removed. If no such script exists, does nothing.
     */
    void removeScript(unsigned id);

    /**
     * @brief Check if script with given id exists.
     * @param id Id number of the script.
     * @return True, if script does exist.
     * @pre -
     */
    bool hasScript(unsigned id) const;

    /**
     * @brief Get script entry wit given id.
     * @param id Id number of the script.
     * @return Script matching the id. If no such id exists, returns default
     * ScriptEntry (id=0, path="", language="", readToRam=false, priority=0).
     * @pre -
     */
    ScriptEntry getScript(unsigned id) const;

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
     * 5) All plugin paths has an appropriate postfix. Existence is not checked at this point.
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
