// Copyright (c) 2022 Ultimaker B.V.
// CuraEngine is released under the terms of the AGPLv3 or higher

#include "Settings.h"
#include "header.h"

namespace crcommon
{

    Settings::Settings()
    {
        parent = nullptr; // Needs to be properly initialised because we check against this if the parent is not set.
    }

    void Settings::add(const std::string& key, const std::string& value)
    {
        if (settings.find(key) != settings.end()) // Already exists.
        {
            settings[key] = value;
        }
        else // New setting.
        {
            settings.emplace(key, value);
        }
    }

    template<>
    std::string Settings::get<std::string>(const std::string& key) const
    {
        // If this settings base has a setting value for it, look that up.
        if (settings.find(key) != settings.end())
        {
            return settings.at(key);
        }


        if (parent)
        {
            return parent->get<std::string>(key);
        }
        std::exit(2);
    }

    template<>
    double Settings::get<double>(const std::string& key) const
    {
        return atof(get<std::string>(key).c_str());
    }

    template<>
    size_t Settings::get<size_t>(const std::string& key) const
    {
        return std::stoul(get<std::string>(key).c_str());
    }

    template<>
    int Settings::get<int>(const std::string& key) const
    {
        return atoi(get<std::string>(key).c_str());
    }

    const std::string Settings::getString(const std::string& key) const
    {
        return get<std::string>(key);
    }

    template<>
    bool Settings::get<bool>(const std::string& key) const
    {
        const std::string& value = get<std::string>(key);
        if (value == "on" || value == "yes" || value == "true" || value == "True")
        {
            return true;
        }
        const int num = atoi(value.c_str());
        return num != 0;
    }

    bool Settings::has(const std::string& key) const
    {
        return settings.find(key) != settings.end();
    }

    void Settings::setParent(Settings* new_parent)
    {
        parent = new_parent;
    }

    void Settings::load(std::ifstream& in)
    {

    }

    void Settings::save(std::ofstream& out)
    {
        int count = (int)settings.size();
        templateSave<int>(count, out);

        for (std::unordered_map<std::string, std::string>::iterator it = settings.begin();
            it != settings.end(); ++it)
        {
            saveStr((*it).first, out);
            saveStr((*it).second, out);
        }
    }

    std::string Settings::getWithoutLimiting(const std::string& key) const
    {
        if (settings.find(key) != settings.end())
        {
            return settings.at(key);
        }
        else if (parent)
        {
            return parent->get<std::string>(key);
        }
        else
        {
            std::exit(2);
        }
    }

    std::string loadStr(std::ifstream& in)
    {
        return std::string();
    }

    void saveStr(const std::string& str, std::ofstream& out)
    {
        int size = (int)str.size();
        templateSave<int>(size, out);
        out.write(str.c_str(), size);
    }
} // namespace cura
