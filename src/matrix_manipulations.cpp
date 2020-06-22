//
// Created by vlad on 19.06.20.
//

#include <iostream>
#include <cassert>

#include "../headers/config_processing.h"
#include "../headers/matrix.h"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

void add_point_with_temperature(std::vector<double>* matrix, std::string points_and_temperature, int height, int width){
//    std::string points_and_temperature = (*str_data);
    std::vector<std::string> words;

    boost::split(words, points_and_temperature, boost::is_any_of(","), boost::token_compress_on);
    assert(words.size() == 3);

    int x_coordinate = stoi(words[0]);
    int y_coordinate = stoi(words[1]);
    double temperature_value = atof(words[2].c_str());

//    std::cout << x_coordinate << " " << " " << y_coordinate << " " << temperature_value << "\n";

    set_item(matrix, y_coordinate, x_coordinate, height, width, temperature_value);
}

void add_all_temperatures_to_matrix(std::vector<double>* matrix, Json::Value* data, int height, int width){
    for (int i = 0; i < (*data)["temperatures"].size(); i++){
        std::string key = std::to_string(i);
        add_point_with_temperature(matrix, (*data)["temperatures"][key].asString(), height, width);
    }
}