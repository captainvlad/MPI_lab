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
    std::vector<std::string> words;

    boost::split(words, points_and_temperature, boost::is_any_of(","), boost::token_compress_on);
    assert(words.size() == 3);

    int x_coordinate = stoi(words[0]);
    int y_coordinate = stoi(words[1]);
    double temperature_value = atof(words[2].c_str());

    set_item(matrix, y_coordinate, x_coordinate, height, width, temperature_value);
}

void add_all_temperatures_to_matrix(std::vector<double>* matrix, Json::Value* data, int height, int width){
    for (int i = 0; i < (*data)["temperatures"].size(); i++){
        std::string key = std::to_string(i);
        add_point_with_temperature(matrix, (*data)["temperatures"][key].asString(), height, width);
    }
}

bool all_cells_of_same_temperature(std::vector<double>* matrix){
    bool same = 1;
    double current_value = (*matrix)[0];

    for (auto item: *matrix){
        if (current_value != item){
            return false;
        }
    }

    return true;
}

int availible_upper_neighbour(std::vector<double>* matrix, int width, int number_of_all_parts, int part_of_interest){

    int height = matrix->size() / width;
    int start_row = floor(height / number_of_all_parts) * part_of_interest;

    if (start_row >= height || start_row == 0){
        return -1;
    }

    return start_row - 1;
}

int availible_lower_neighbour(std::vector<double>* matrix, int width, int number_of_all_parts, int part_of_interest){
    bool last = part_of_interest == number_of_all_parts - 1;
    int start_row, finish_row, part_height;
    int height = matrix->size() / width;

    start_row = floor(height / number_of_all_parts) * part_of_interest;

    if (last){
        finish_row = start_row + floor( height / number_of_all_parts ) + height % number_of_all_parts;
    } else {
        finish_row = start_row + floor( height / number_of_all_parts );
    }

    if (finish_row + 1 >= height || finish_row + 1 <= 0){
        return -1;
    }

    return finish_row;
}

std::vector<double>* get_upper_neighbour(std::vector<double>* matrix, int width, int number_of_all_parts, int part_of_interest){
    auto result = new std::vector<double>(width, 0);
    int needed_row = availible_upper_neighbour(matrix, width, number_of_all_parts, part_of_interest);

    if (needed_row == -1){
        (*result)[0] = INT_MIN;
    } else {
        for (int i = 0; i < width; i++){
            (*result)[i] = get_item(matrix, needed_row, i, matrix->size() / width, width);
        }
    }

    return result;
}

std::vector<double>* get_lower_neighbour(std::vector<double>* matrix, int width, int number_of_all_parts, int part_of_interest){
    auto result = new std::vector<double>(width, 0);
    int needed_row = availible_lower_neighbour(matrix, width, number_of_all_parts, part_of_interest);

    if (needed_row == -1){
        (*result)[0] = INT_MIN;
    } else {
        for (int i = 0; i < width; i++){
            (*result)[i] = get_item(matrix, needed_row, i, matrix->size() / width, width);
        }
    }

    return result;
}