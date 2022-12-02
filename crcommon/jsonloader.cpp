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
	std::unordered_set<std::string> defaultSearchDirectories()
	{
		return std::unordered_set<std::string>();
	}

	int loadJSON(const std::string& jsonFileName, KValues& KVs, std::vector<KValues>& extruderKVs)
	{
		return 0;
	}
}