//
// Created by vlad on 19.06.20.
//
#include <vector>
#include <cassert>
#include <cmath>

std::vector<double>* initialize_matrix(int height, int width){
    return new std::vector<double>(height * width, 0);
}

double get_item(std::vector<double>* matrix, int row, int column, int height , int width){
    return (*matrix)[row * width + column];
}

void set_item(std::vector<double>* matrix, int row, int column, int height, int width, double value){
    (*matrix)[row * width + column] = value;
}

void represent_matrix(std::vector<double>* matrix, int height, int width){
    for (int i = 0; i < height; i++){
        std::cout << "|";
        for (int j = 0; j < width; j++){
            std::cout << get_item(matrix, i, j, height, width) << " ";
        }
        std::cout << "|\n";
    }
}

std::vector<double>* cut_matrix_stripe(std::vector<double>* matrix, int height,
        int width, int number_of_rows, int row_number){

    bool last = row_number == number_of_rows - 1;
    int start_row, finish_row, part_height;

    start_row = floor(height / number_of_rows) * row_number;

    if (last){
        finish_row = start_row + floor( height / number_of_rows ) + height % number_of_rows;
    } else {
        finish_row = start_row + floor( height / number_of_rows );
    }

    part_height = finish_row - start_row;

    auto result = initialize_matrix(part_height, width);

    for (int i = 0; i < part_height; i++){
        for (int j = 0; j < width; j++){
            double value = get_item(matrix, i + start_row, j, height, width);
            set_item(result, i, j, part_height, width, value);
        }
    }

    return result;
}

void put_into( std::vector<double>* matrix, std::vector<double>* part_matrix, int height,
        int width, int total_parts, int part_index ){

    bool last = part_index == total_parts - 1;
    int start_row, finish_row, part_height;
    double value;

    start_row = floor(height / total_parts) * part_index;

    if (last){
        finish_row = start_row + floor( height / total_parts ) + height % total_parts;
    } else {
        finish_row = start_row + floor( height / total_parts );
    }

    part_height = finish_row - start_row;

    for (int i = 0; i < part_height; i++){
        for (int j = 0; j < width; j++){
            value = get_item(part_matrix, i, j, part_height, width);
            set_item(matrix, start_row + i, j, height, width, value);
        }
    }
}