#pragma once

#include <json/json.h>

#include <iostream>
#include <fstream>
#include <string>



void load_json(const char *filename, Json::Value &value) {

    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        std::cout << "Failed to open file" << std::endl;
        throw std::exception();
    }

    Json::CharReaderBuilder builder;
    builder["collectComments"] = true;
    std::string errors;

    if (!parseFromStream(builder, ifs, &value, &errors)) {
        std::cout << "Failed to parse json" << std::endl;
        throw std::exception();
    }
}