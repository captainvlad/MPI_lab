//
// Created by vlad on 23.06.20.
//

double calculate_alpha( double thermal_conduction, double heat_capacity, double density ){
    return thermal_conduction / (heat_capacity * density);
}

double calculate_heat_equation(double this_prev_temp, double prev_i_temp, double next_i_temp,  double prev_j_temp,  double next_j_temp,
                               double delta_x,        double delta_y,     double delta_t,      double alpha) {

    double this_curr_temp = this_prev_temp;
    double first = (prev_i_temp - 2*this_prev_temp + next_i_temp)/(delta_x*delta_x);
    double second = (prev_j_temp - 2*this_prev_temp + next_j_temp)/(delta_y*delta_y);
    double delta_sums = first + second;
    this_curr_temp += delta_t * alpha * delta_sums;

    return this_curr_temp;
}

bool von_neumann_criterion(double delta_x, double delta_y, double delta_t, double alpha) {
    double val = std::max(delta_x, delta_y);
    double criteria = (val*val)/(4*alpha);

    return criteria >= delta_t;
}

void calculate_heat( std::vector<double>* matrix, std::vector<double>* upper_neighbour, std::vector<double>* lower_neighbour,
                     double delta_x, double delta_y, double delta_t, double alpha){

    int width = (*upper_neighbour).size();
    int height = (*matrix).size() / width;
    double i_1_j_1_plus, i_1_j_1, i_1_minus_j_1, i_1_plus_j_1, i_1_j_1_minus, current_temp, current_row;

    if ( (*upper_neighbour)[0] != INT_MIN ){
        for (int i = 1; i < width - 1; i++){

            i_1_j_1_plus = get_item(matrix, 0, i+1, height, width);
            i_1_j_1 = get_item(matrix, 0, i, height, width);

            i_1_minus_j_1 = (*upper_neighbour)[i];
            i_1_j_1_minus = get_item(matrix, 0, i-1, height, width);

            if (height > 1){
                i_1_plus_j_1 = get_item(matrix, 1, i, height, width);
            } else{
                i_1_plus_j_1 = (*lower_neighbour)[i];
            }

            current_temp = calculate_heat_equation(i_1_j_1, i_1_minus_j_1, i_1_plus_j_1, i_1_j_1_minus, i_1_j_1_plus, delta_x, delta_y, delta_t, alpha);
            set_item(matrix, 0, i, height, width, current_temp);
        }
    }


    if ( (*lower_neighbour)[0] != INT_MIN ){
        current_row = height - 1;

        for (int i = 1; i < width - 1; i++){

            i_1_j_1 = get_item(matrix, current_row, i, height, width);
            i_1_j_1_plus = get_item(matrix, current_row, i + 1, height, width);

            i_1_j_1_minus = get_item(matrix, current_row, i - 1, height, width);
            i_1_plus_j_1 = (*lower_neighbour)[i];

            if (height > 1){
                i_1_minus_j_1 = get_item(matrix, current_row - 1, i, height, width);
            } else{
                i_1_minus_j_1 = (*upper_neighbour)[i];
            }

            current_temp = calculate_heat_equation(i_1_j_1, i_1_minus_j_1, i_1_plus_j_1, i_1_j_1_minus, i_1_j_1_plus, delta_x, delta_y, delta_t, alpha);
            set_item(matrix, current_row, i, height, width, current_temp);
        }
    }

    for (int i = 1; i < height - 1; i++){
        for (int j = 1; j < width - 1; j++){

            i_1_j_1 = get_item(matrix, i, j, height, width);
            i_1_minus_j_1 = get_item(matrix, i - 1, j, height, width);

            i_1_j_1_minus = get_item(matrix, i, j - 1, height, width);
            i_1_j_1_plus = get_item(matrix, i, j + 1, height, width);
            i_1_plus_j_1 = get_item(matrix, i + 1, j, height, width);

            current_temp = calculate_heat_equation(i_1_j_1, i_1_minus_j_1, i_1_plus_j_1, i_1_j_1_minus, i_1_j_1_plus, delta_x, delta_y, delta_t, alpha);
            set_item(matrix, i, j, height, width, current_temp);
        }
    }
}