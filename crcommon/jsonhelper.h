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
}

#endif // CRCOMMON_PARAMETERGENERATOR_1690769853658_H