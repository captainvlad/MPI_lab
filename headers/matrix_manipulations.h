//
// Created by vlad on 19.06.20.
//

#ifndef MPI_MATRIX_MANIPULATIONS_H
#define MPI_MATRIX_MANIPULATIONS_H

#include "../src/matrix_manipulations.cpp"

void add_point_with_temperature(std::vector<double>* matrix, std::string* str_data);

void add_all_temperatures_to_matrix(std::vector<double>* matrix, Json::Value* data, int height, int width);

int availible_upper_neighbour(std::vector<double>* matrix, int width, int number_of_all_parts, int part_of_interest);

int availible_lower_neighbour(std::vector<double>* matrix, int width, int number_of_all_parts, int part_of_interest);

std::vector<double>* get_upper_neighbour(std::vector<double>* matrix, int width, int number_of_all_parts, int part_of_interest);

#endif //MPI_MATRIX_MANIPULATIONS_H
