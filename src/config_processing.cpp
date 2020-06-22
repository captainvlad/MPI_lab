//
// Created by vlad on 19.06.20.
//

#include <jsoncpp/json/json.h>

#include <fstream>
#include <cassert>

inline bool file_exists (std::string name) {
    std::ifstream f(name.c_str());
    return f.good();
}

Json::Value read_config(std::string filename){
    assert(file_exists(filename));
    Json::Value config_data;

    std::ifstream config_file(filename);
    config_file >> config_data;

    return config_data;
}