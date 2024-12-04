//
// Created by 29697 on 24-10-19.
//

#include "ConfigManager.h"
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ini_parser.hpp>

std::string ConfigManager::GetValue(const std::string &section, const std::string &key) {
    if (_config_map.find(section) == _config_map.end()) {
        return "";
    }
    return _config_map[section].GetValue(key);
}

ConfigManager::ConfigManager() {
    boost::filesystem::path current_path = boost::filesystem::current_path().parent_path();
    // 构建config.ini文件的完整路径
    boost::filesystem::path config_path = current_path / "config\\config.ini";
    std::cout << "config path is " << config_path << std::endl;
    boost::property_tree::ptree pt;
    boost::property_tree::read_ini(config_path.string(), pt);

    for (const auto &section_pair: pt) {
        const ::std::string &section_name = section_pair.first;
        const boost::property_tree::ptree &section_tree = section_pair.second;
        std::map<std::string, std::string> section_config;
        for (const auto &ket_value_pair: section_tree) {
            const ::std::string &key = ket_value_pair.first;
            const ::std::string &value = ket_value_pair.second.get_value<std::string>();
            section_config[key] = value;
        }
        SectionInfo section_info;
        section_info._section_datas = section_config;
        _config_map[section_name] = section_info;
    }

    // 输出所有的section和key value对
    for (const auto &section_pair: _config_map) {
        const std::string &section_name = section_pair.first;
        const SectionInfo &section_info = section_pair.second;
        std::cout << "[" << section_name << "]" << std::endl;
        for (const auto &key_value_pair: section_info._section_datas) {
            std::cout << key_value_pair.first << "=" << key_value_pair.second << std::endl;
        }
    }
}