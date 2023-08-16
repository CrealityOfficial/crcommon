#ifndef CRCOMMON_PARAMETERGENERATOR_1690769853658_H
#define CRCOMMON_PARAMETERGENERATOR_1690769853658_H
#include <rapidjson/document.h>
#include "crcommon/base/parametermeta.h"
#include <string>

namespace crcommon
{
	bool openJson(rapidjson::Document& doc, const std::string& fileName);

	void processSub(const rapidjson::Document& doc, MetasMap& metas);
	void processMeta(const rapidjson::Value& value, ParameterMeta& meta);

	void processInherit(const std::string& fileName, const std::string& directory, ParameterMetas& metas);

	void saveJson(const std::string& fileName, const std::string& content);
	std::string createKeysContent(const std::vector<std::string>& keys);
}

#endif // CRCOMMON_PARAMETERGENERATOR_1690769853658_H