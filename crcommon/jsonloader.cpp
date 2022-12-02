#include "jsonloader.h"
#include <unordered_set>

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
	void _add_item(const std::string& key, const std::string value, KValues& KVs)
	{
		if (KVs.find(key) != KVs.end()) // Already exists.
		{
			KVs[key] = value;
		}
		else // New setting.
		{
			KVs.emplace(key, value);
		}
	}

	std::unordered_set<std::string> defaultSearchDirectories()
	{
		return std::unordered_set<std::string>();
	}

	int loadJSON(const std::string& jsonFileName, KValues& KVs, std::vector<KValues>& extruderKVs)
	{
		return 0;
	}
}