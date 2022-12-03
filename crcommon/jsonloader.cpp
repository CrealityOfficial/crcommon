#include "jsonloader.h"
#include <unordered_set>
#include <iostream>     // std::cin, std::cout
#include <fstream>      // std::ofstream
#include <sstream>
#include <string>
#include <filesystem>

#include <cstring> //For strtok and strcopy.
#include <errno.h> // error number when trying to read file

#include <rapidjson/document.h> //Loading JSON documents to get settings from them.
#include <rapidjson/error/en.h> //Loading JSON documents to get settings from them.
#include <rapidjson/filereadstream.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettyWriter.h>

#include "ccglobal/log.h"

namespace crcommon
{

    class KeyValueContainer
    {
    public:
        KeyValueContainer() {};
        ~KeyValueContainer() {};
    public:
        std::string jsonFileName;
        Settings m_globalKV;
        std::vector<Settings> m_extruderKVs;
    private:
        std::unordered_set<std::string> defaultSearchDirectories();
        /*
         * \brief Load a JSON file and store the settings inside it.
         * \param json_filename The location of the JSON file to load settings from.
         * \param settings The settings storage to store the settings in.
         * \return Error code. If it's 0, the file was successfully loaded. If it's
         * 1, the file could not be opened. If it's 2, there was a syntax error in
         * the file.
         */
        int loadJSON(const std::string& json_filename, Settings& settings);

        /*
         * \brief Load a JSON document and store the settings inside it.
         * \param document The JSON document to load the settings from.
         * \param settings The settings storage to store the settings in.
         * \return Error code. If it's 0, the document was successfully loaded. If
         * it's 1, some inheriting file could not be opened.
         */
        int loadJSON(const rapidjson::Document& document, const std::unordered_set<std::string>& search_directories, Settings& settings);

        /*
         * \brief Load an element containing a list of settings.
         * \param element The JSON element "settings" or "overrides" that contains
         * settings.
         * \param settings The settings storage to store the new settings in.
         */
        void loadJSONSettings(const rapidjson::Value& element, Settings& settings);

        /*
         * \brief Find a definition file in the search directories.
         * \param definition_id The ID of the definition to look for.
         * \param search_directories The directories to search in.
         * \return The first definition file that matches the definition ID.
         */
        const std::string findDefinitionFile(const std::string& definition_id, const std::unordered_set<std::string>& search_directories);

    public:
        void loadFile(const std::string& filename);
    };
    int KeyValueContainer::loadJSON(const std::string& json_filename, Settings& settings)
    {
        //FILE* file = fopen(json_filename.c_str(), "rb");
        //if (! file)
        //{
        //    LOGE("Couldn't open JSON file: %s", json_filename.c_str());
        //    return 1;
        //}

        //rapidjson::Document json_document;
        //char read_buffer[4096];
        //rapidjson::FileReadStream reader_stream(file, read_buffer, sizeof(read_buffer));
        //json_document.ParseStream(reader_stream);
        //fclose(file);
        //if (json_document.HasParseError())
        //{
        //    LOGE("Error parsing JSON (offset %d): %s", (int)json_document.GetErrorOffset(), GetParseError_En(json_document.GetParseError()));
        //    return 2;
        //}
        LOGE("try to open JSON file: %s", json_filename);

        std::ifstream ifs(json_filename.c_str());
        if (!ifs.is_open())
        {
            LOGE("Couldn't open JSON file: %", json_filename);
            return 1;
        }
        rapidjson::IStreamWrapper isw(ifs);
        rapidjson::Document* json_documentPtr = new rapidjson::Document();
        rapidjson::Document& json_document = *json_documentPtr;
        json_document.ParseStream(isw);
        if (json_document.HasParseError())
        {
            LOGE("Error parsing JSON (offset %d): %s", (int)json_document.GetErrorOffset(), GetParseError_En(json_document.GetParseError()));
            return 2;
        }

        std::unordered_set<std::string> search_directories = defaultSearchDirectories(); // For finding the inheriting JSON files.
        std::string directory = std::filesystem::path(json_filename).parent_path().string();
        search_directories.emplace(directory);
        return loadJSON(json_document, search_directories, settings);
    }

    std::unordered_set<std::string> KeyValueContainer::defaultSearchDirectories()
    {
        std::unordered_set<std::string> result;

        char* search_path_env = getenv("CURA_ENGINE_SEARCH_PATH");
        if (search_path_env)
        {
#if defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
            char delims[] = ":"; // Colon for Unix.
#else
            char delims[] = ";"; // Semicolon for Windows.
#endif
            char paths[128 * 1024]; // Maximum length of environment variable.
            strcpy(paths, search_path_env); // Necessary because strtok actually modifies the original string, and we don't want to modify the environment variable itself.
            char* path = strtok(paths, delims);
            while (path != nullptr)
            {
                result.emplace(path);
                path = strtok(nullptr, ";:,"); // Continue searching in last call to strtok.
            }
        }

        return result;
    }

    int KeyValueContainer::loadJSON(const rapidjson::Document& document, const std::unordered_set<std::string>& search_directories, Settings& settings)
    {
        // Inheritance from other JSON documents.
        if (document.HasMember("inherits") && document["inherits"].IsString())
        {
            std::string parent_file = findDefinitionFile(document["inherits"].GetString(), search_directories);
            if (parent_file == "")
            {
                LOGE("Inherited JSON file: %s not found.", document["inherits"].GetString());
                return 1;
            }
            int error_code = loadJSON(parent_file, settings); // Head-recursively load the settings file that we inherit from.
            if (error_code)
            {
                return error_code;
            }
        }

        // Extruders defined from here, if any.
        // Note that this always puts the extruder settings in the slice of the current extruder. It doesn't keep the nested structure of the JSON files, if extruders would have their own sub-extruders.
        if (document.HasMember("metadata") && document["metadata"].IsObject())
        {
            const rapidjson::Value& metadata = document["metadata"];
            if (metadata.HasMember("machine_extruder_trains") && metadata["machine_extruder_trains"].IsObject())
            {
                const rapidjson::Value& extruder_trains = metadata["machine_extruder_trains"];
                for (rapidjson::Value::ConstMemberIterator extruder_train = extruder_trains.MemberBegin(); extruder_train != extruder_trains.MemberEnd(); extruder_train++)
                {
                    const int extruder_nr = atoi(extruder_train->name.GetString());
                    if (extruder_nr < 0)
                    {
                        continue;
                    }
                    while (m_extruderKVs.size() <= static_cast<size_t>(extruder_nr))
                    {
                        m_extruderKVs.emplace_back();
                    }
                    const rapidjson::Value& extruder_id = extruder_train->value;
                    if (!extruder_id.IsString())
                    {
                        continue;
                    }
                    const std::string extruder_definition_id(extruder_id.GetString());
                    const std::string extruder_file = findDefinitionFile(extruder_definition_id, search_directories);
                    LOGE("start load extruders setting %s", extruder_file.c_str());
                    loadJSON(extruder_file, m_extruderKVs.back());
                }
            }
        }

        if (document.HasMember("settings") && document["settings"].IsObject())
        {
            loadJSONSettings(document["settings"], settings);
        }
        if (document.HasMember("overrides") && document["overrides"].IsObject())
        {
            loadJSONSettings(document["overrides"], settings);
        }
        return 0;
    }

    void KeyValueContainer::loadJSONSettings(const rapidjson::Value& element, Settings& settings)
    {
        for (rapidjson::Value::ConstMemberIterator setting = element.MemberBegin(); setting != element.MemberEnd(); setting++)
        {
            const std::string name = setting->name.GetString();

            const rapidjson::Value& setting_object = setting->value;
            if (!setting_object.IsObject())
            {
                LOGE("JSON setting %s is not an object! %s", name.c_str());
                continue;
            }

            if (setting_object.HasMember("children"))
            {
                if (!setting_object.HasMember("default_value"))
                {
                    LOGE("JSON setting %s has no default_value!,but have children", name);
                    goto CHILDRN;
                }
                else
                {
                    const rapidjson::Value& default_value = setting_object["default_value"];
                    std::string value_string;
                    if (default_value.IsString())
                    {
                        value_string = default_value.GetString();
                    }
                    else if (default_value.IsTrue())
                    {
                        value_string = "true";
                    }
                    else if (default_value.IsFalse())
                    {
                        value_string = "false";
                    }
                    else if (default_value.IsNumber())
                    {
                        std::ostringstream ss;
                        ss << default_value.GetDouble();
                        value_string = ss.str();
                    }
                    else if (default_value.IsArray())
                    {
                        LOGE(" data type is IsArray,but Unrecognized data type in JSON setting %s", name);
                        goto CHILDRN;;
                    }
                    else
                    {
                        LOGE("Unrecognized data type in JSON setting %s", name);
                        goto CHILDRN;;
                    }
                    settings.add(name, value_string);
                }
            CHILDRN:
                loadJSONSettings(setting_object["children"], settings);
            }
            else // Only process leaf settings. We don't process categories or settings that have sub-settings.
            {
                if (!setting_object.HasMember("default_value"))
                {
                    LOGW("JSON setting %s has no default_value!", name.c_str());
                    continue;
                }
                const rapidjson::Value& default_value = setting_object["default_value"];
                std::string value_string;
                if (default_value.IsString())
                {
                    value_string = default_value.GetString();
                }
                else if (default_value.IsTrue())
                {
                    value_string = "true";
                }
                else if (default_value.IsFalse())
                {
                    value_string = "false";
                }
                else if (default_value.IsNumber())
                {
                    std::ostringstream ss;
                    ss << default_value.GetDouble();
                    value_string = ss.str();
                }
                else
                {
                    LOGW("Unrecognized data type in JSON setting %s", name.c_str());
                    continue;
                }
                settings.add(name, value_string);
            }
        }
    }

    const std::string KeyValueContainer::findDefinitionFile(const std::string& definition_id, const std::unordered_set<std::string>& search_directories)
    {
        for (const std::string& search_directory : search_directories)
        {
            const std::string candidate = search_directory + std::string("/") + definition_id + std::string(".def.json");
            const std::ifstream ifile(candidate.c_str()); // Check whether the file exists and is readable by opening it.
            if (ifile)
            {
                return candidate;
            }
        }
        LOGE("Couldn't find definition file with ID: %s", definition_id.c_str());
        return std::string("");
    }
    void KeyValueContainer::loadFile(const std::string& filename)
    {
        loadJSON(filename, m_globalKV);
    }

    int loadGlobalSettingJSON(const std::string& jsonFileName, Settings& KVs, std::vector<Settings>& extruderKVs)
    {
        LOGE("try to open JSON file: %s", jsonFileName);

        std::ifstream ifs(jsonFileName.c_str());
        if (!ifs.is_open())
        {
            LOGE("Couldn't open JSON file: %", jsonFileName);
            return 1;
        }
        KeyValueContainer keyvalueLoader;
        keyvalueLoader.loadFile(jsonFileName);
        KVs = keyvalueLoader.m_globalKV;
        extruderKVs = keyvalueLoader.m_extruderKVs;
        return 0;
    }
}