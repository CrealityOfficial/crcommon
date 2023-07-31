#include "parametermeta.h"
#include <boost/filesystem.hpp>

#include "crcommon/jsonhelper.h"
#include "ccglobal/log.h"

namespace crcommon
{
	ParameterMetas::ParameterMetas()
	{
	}

	ParameterMetas::~ParameterMetas()
	{
        for (MetasMapIter it = mm.begin();
            it != mm.end(); ++it)
            delete it->second;
        mm.clear();
	}

	void ParameterMetas::initializeBase(const std::string& path)
	{
		std::string baseFile = path + "/base.json";

		rapidjson::Document baseDoc;
		if (!openJson(baseDoc, baseFile))
		{
			LOGE("ParameterMetas::initializeBase error. [%s] not contain base.json", path.c_str());
			return;
		}

        if (baseDoc.HasMember("subs") && baseDoc["subs"].IsArray())
        {
            const rapidjson::Value& value = baseDoc["subs"];
            for (rapidjson::Value::ConstValueIterator it = value.Begin(); it != value.End(); ++it)
            {
                std::string sub = it->GetString();
                std::string json = path + "/" + sub;

                rapidjson::Document subDoc;
                if (!openJson(baseDoc, baseFile))
                {
                    LOGE("ParameterMetas::initializeBase parse sub. [%s] error.", sub.c_str());
                    continue;
                }

                processSub(subDoc, mm);
            }
        }
        else {
            LOGE("ParameterMetas::initializeBase base.json no subs.");
        }
	}

	void ParameterMetas::initializeBaseFile(const std::string& fileName)
	{

	}

	ParameterMetas* ParameterMetas::createInherits(const std::string& fileName)
	{
        return nullptr;
	}
}