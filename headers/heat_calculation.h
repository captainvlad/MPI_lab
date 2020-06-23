//
// Created by vlad on 23.06.20.
//

#ifndef MPI_HEAT_CALCULATION_H
#define MPI_HEAT_CALCULATION_H

#include "../src/heat_calculation.cpp"

double calculate_alpha( double thermal_conduction, double heat_capacity, double density );
double calculate_heat_equation(double this_prev_temp, double prev_i_temp, double next_i_temp,  double prev_j_temp,  double next_j_temp,
                               double delta_x,        double delta_y,     double delta_t,      double alpha);

bool von_neumann_criterion(double delta_x, double delta_y, double delta_t, double alpha);
void calculate_heat( std::vector<double>* matrix, std::vector<double>* upper_neighbour, std::vector<double>* lower_neighbour,
                     double delta_x, double delta_y, double delta_t, double alpha);

#endif //MPI_HEAT_CALCULATION_H
