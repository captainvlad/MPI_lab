//
// Created by vlad on 19.06.20.
//

#ifndef MPI_MATRIX_H
#define MPI_MATRIX_H

#include "../wrapper/matrix.cpp"

std::vector<double>* initialize_matrix(int height, int width);
double get_item(std::vector<double>* matrix, int row, int column, int height, int width);

void represent_matrix(std::vector<double>* matrix, int height, int width);
void set_item(std::vector<double>* matrix, int row, int column, int height, int width, double value);

std::vector<double>* cut_matrix_stripe(std::vector<double>* matrix, int height,
                                       int width, int number_of_rows, int row_number);

void put_into( std::vector<double>* matrix, std::vector<double>* part_matrix, int height,
        int width, int total_parts, int part_index );

#endif //MPI_MATRIX_H
