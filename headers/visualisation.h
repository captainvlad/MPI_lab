//
// Created by vlad on 23.06.20.
//

#ifndef MPI_VISUALISATION_H
#define MPI_VISUALISATION_H

#include "../visual/visualisation.cpp"
#include <Magick++.h>

void create_gif();
Magick::Color* define_color(double value);

void visualise_matrix(std::vector<double>* matrix, int height, int width, int number_of_image);

#endif //MPI_VISUALISATION_H
