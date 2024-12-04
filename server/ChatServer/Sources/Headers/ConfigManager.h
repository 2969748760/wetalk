//
// Created by 29697 on 24-10-19.
//

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H
#include <map>
#include <string>


struct SectionInfo {
    SectionInfo() {
    };
    ~SectionInfo() { _section_datas.clear(); }

    SectionInfo(const SectionInfo &src) {
        _section_datas = src._section_datas;
    }

    SectionInfo &operator=(const SectionInfo &src) {
        if (this == &src) {
            return *this;
        }
        this->_section_datas = src._section_datas;
        return *this;
    }

    std::map<std::string, std::string> _section_datas;

    std::string operator[](const std::string &key) {
        if (_section_datas.find(key) == _section_datas.end()) {
            return "";
        }
        return _section_datas[key];
    }

    std::string GetValue(const std::string &key) {
        if (_section_datas.find(key) == _section_datas.end()) {
            return "";
        }
        return _section_datas[key];
    }
};

class ConfigManager {
public:
    ~ConfigManager() {
        _config_map.clear();
    }

    SectionInfo operator[](const std::string &section) {
        if (_config_map.find(section) == _config_map.end()) {
            return SectionInfo();
        }
        return _config_map[section];
    }

    ConfigManager(const ConfigManager &src) {
        this->_config_map = src._config_map;
    }

    ConfigManager &operator=(const ConfigManager &src) {
        if (this == &src) {
            return *this;
        }
        this->_config_map = src._config_map;
        return *this;
    }

    static ConfigManager &GetInstance() {
        static ConfigManager instance;
        return instance;
    }

    std::string GetValue(const std::string &section, const std::string &key);

private:
    std::map<std::string, SectionInfo> _config_map;

    ConfigManager();
};


#endif //CONFIGMANAGER_H
