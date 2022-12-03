#ifndef CRCOMMON_JSONLOADER_1669972280254_H
#define CRCOMMON_JSONLOADER_1669972280254_H
#include "crcommon/interface.h"
#include "crcommon/Settings.h"
#include <unordered_map>
#include <vector>

namespace crcommon
{
    /*
 * \brief Load a JSON file and store the settings inside it.
 * \param json_filename The location of the JSON file to load settings from.
 * \param settings The settings storage to store the settings in.
 * \return Error code. If it's 0, the file was successfully loaded. If it's
 * 1, the file could not be opened. If it's 2, there was a syntax error in
 * the file.
 */
    CRCOMMON_API int loadGlobalSettingJSON(const std::string& jsonFileName, Settings& KVs, std::vector<Settings>& extruderKVs);
}

#endif // CRCOMMON_JSONLOADER_1669972280254_H