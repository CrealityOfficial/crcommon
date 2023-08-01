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

    void processSub(const rapidjson::Document& doc, MetasMap& metas)
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

            ParameterMeta meta;
            processMeta(childValue, meta);

            metas.insert(MetasMap::value_type(name, new ParameterMeta(meta)));
        }
    }

    void processMeta(const rapidjson::Value& value, ParameterMeta& meta)
    {
        if (!value.IsObject())
            return;

        if (value.HasMember(META_LABEL))
            meta.label = (value[META_LABEL].GetString());

        if (value.HasMember(META_DESCRIPTION))
            meta.description = (value[META_DESCRIPTION].GetString());

        //if (value.HasMember("unit")) itemDef->unit = (value["unit"].GetString());
        
        if (value.HasMember(META_TYPE))
            meta.type = (value[META_TYPE].GetString());

        if (value.HasMember(META_DEFAULT_VALUE))
            meta.default_value = (value[META_DEFAULT_VALUE].GetString());

        //if (value.HasMember("minimum_value")) itemDef->minimum = (value["minimum_value"].GetString());
        //if (value.HasMember("maximum_value")) itemDef->maximum = (value["maximum_value"].GetString());
        //if (value.HasMember("minimum_value_warning")) itemDef->miniwarning = (value["minimum_value_warning"].GetString());
        //if (value.HasMember("maximum_value_warning")) itemDef->maxwarning = (value["maximum_value_warning"].GetString());
        
        if (value.HasMember(META_VALUE))
            meta.value = (value[META_VALUE].GetString());

        if (value.HasMember(META_ENABLED))
        {
            const rapidjson::Value& enabledValue = value[META_ENABLED];
            if (enabledValue.IsBool())
            {
                if (enabledValue.GetBool())
                {
                    meta.enabled = "true";
                }
                else
                {
                    meta.enabled = "false";
                }
            }
            else
            {
                meta.enabled = (enabledValue.GetString());
            }
        }

        //if (value.HasMember("options"))
        //{
        //    const rapidjson::Value& options = value["options"];
        //    for (rapidjson::Value::ConstMemberIterator child = options.MemberBegin();
        //        child != options.MemberEnd(); child++)
        //    {
        //        QString name = child->name.GetString();
        //        QString values = child->value.GetString();
        //
        //        itemDef->options.insert(name, values);
        //    }
        //}
        //if (itemDef->type == "optional_extruder" || itemDef->type == "extruder")
        //{
        //    itemDef->options.insert(QString("-1"), QString("Not overridden"));
        //    itemDef->options.insert(QString("0"), QString("Extruder 1"));
        //    itemDef->options.insert(QString("1"), QString("Extruder 2"));
        //}
    }

    void processInherit(const std::string& fileName, const std::string& directory, ParameterMetas& metas)
    {

    }
}