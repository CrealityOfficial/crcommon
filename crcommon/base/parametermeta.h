#ifndef CRCOMMON_PARAMETERMETA_1690769853657_H
#define CRCOMMON_PARAMETERMETA_1690769853657_H
#include "crcommon/interface.h"

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

#define META_LABEL "label"
#define META_DESCRIPTION "description"
#define META_TYPE "type"
#define META_DEFAULT_VALUE "default_value"
#define META_VALUE "value"
#define META_ENABLED "enabled"

namespace crcommon
{
	struct ParameterMeta
	{
		std::string name;
		std::string label;
		std::string description;
		std::string type;
		std::string default_value;
		std::string value;
		std::string enabled;
	};

	typedef std::unordered_map<std::string, ParameterMeta*> MetasMap;
	typedef MetasMap::iterator MetasMapIter;

	class CRCOMMON_API ParameterMetas
	{
	public:
		ParameterMetas();
		~ParameterMetas();

		void initializeBase(const std::string& path);
		void initializeBaseFile(const std::string& fileName);

		ParameterMetas* createInherits(const std::string& fileName);
	public:
		MetasMap mm;
	};
}

#endif // CRCOMMON_PARAMETERMETA_1690769853657_H