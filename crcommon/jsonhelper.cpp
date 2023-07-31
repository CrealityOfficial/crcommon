#include "jsonhelper.h"
#include <fstream>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/error/en.h>

#include "ccglobal/log.h"

namespace crcommon
{
    bool openJson(rapidjson::Document& doc, const std::string& fileName)
	{
        LOGI("try to open JSON file: %s", fileName.c_str());

        std::ifstream ifs(fileName.c_str());
        if (!ifs.is_open())
        {
            LOGE("Couldn't open JSON file: %", fileName.c_str());
            return false;
        }

        rapidjson::IStreamWrapper isw(ifs);
        doc.ParseStream(isw);
        if (doc.HasParseError())
        {
            LOGE("Error parsing JSON (offset %d): %s", (int)doc.GetErrorOffset(), GetParseError_En(doc.GetParseError()));
            return false;
        }

        return true;
	}

    void processSub(rapidjson::Document& doc, MetasMap& metas)
    {
        for (rapidjson::Value::ConstMemberIterator child = doc.MemberBegin();
            child != doc.MemberEnd(); child++)
        {
            std::string name = child->name.GetString();
            const rapidjson::Value& childValue = child->value;

            MetasMap::iterator it = metas.find(name);
            if (it != metas.end())
            {
                LOGE("processSub dumplicate key [%s]", name.c_str());
                continue;
            }

            ParameterMeta* meta = new ParameterMeta();
            metas.insert(MetasMap::value_type(name, meta));
        }
    }
}