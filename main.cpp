#include <iostream>
#include <mpi.h>
#include <vector>
#include "headers/config_processing.h"
#include "headers/matrix_manipulations.h"
#include "headers/heat_calculation.h"
#include "headers/visualisation.h"

int main(int argc, char *argv[])
{
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    std::vector<double> partial_matrix;
    int partial_matrix_size, delta_x, delta_y;
    bool continue_signal;

    double alpha, delta_t;

    if (world_rank == 0) {
        auto config_data = read_config("../config.json");

        int w = config_data["width"].asInt();
        int h = config_data["height"].asInt();
        delta_x = config_data["delta_x"].asInt();
        delta_y = config_data["delta_y"].asInt();
        delta_t = config_data["delta_t"].asDouble();

        alpha = calculate_alpha(config_data["conduction"].asDouble(), config_data["capacity"].asDouble(),
                                config_data["density"].asDouble());

        int current_iteration = 0;

        w = w / delta_x;
        h = h / delta_y;

        auto matrix = *initialize_matrix(h, w);
        add_all_temperatures_to_matrix(&matrix, &config_data, h, w);

        for (int i = 1; i < world_size; i++){
            MPI_Send(&delta_x, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&delta_y, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&delta_t, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            MPI_Send(&alpha, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);

            partial_matrix = *cut_matrix_stripe(&matrix, h, w, world_size - 1, i - 1);

            MPI_Send(new int( partial_matrix.size() ) , 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&w, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&partial_matrix[0] , partial_matrix.size(), MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }

//        assert(von_neumann_criterion(delta_x, delta_y, delta_t, alpha));

        for (int i = 0; i < config_data["needed_iterations"].asInt(); i++) {

            continue_signal = true;

            for (int i = 1; i < world_size; i++) {
                MPI_Send(&continue_signal, 1, MPI_C_BOOL, i, 0, MPI_COMM_WORLD);
                auto upper_neighbour = *get_upper_neighbour(&matrix, w, world_size - 1, i - 1);
                auto lower_neighbour = *get_lower_neighbour(&matrix, w, world_size - 1, i - 1);

                MPI_Send(&upper_neighbour[0], w, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
                MPI_Send(&lower_neighbour[0], w, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            }

            for (int i = 1; i < world_size; i++) {
                MPI_Recv(&partial_matrix_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                partial_matrix.resize(partial_matrix_size);

                MPI_Recv(&partial_matrix[0], partial_matrix_size, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                put_into(&matrix, &partial_matrix, h, w, world_size - 1, i - 1);
            }

            if (i % config_data["delta_iterations"].asInt() == 0){
                visualise_matrix(&matrix, h, w, i / config_data["delta_iterations"].asInt() );
            }
        }

        continue_signal = false;
        for (int i = 1; i < world_size; i++) {
            MPI_Send(&continue_signal, 1, MPI_C_BOOL, i, 0, MPI_COMM_WORLD);
            auto upper_neighbour = *get_upper_neighbour(&partial_matrix, w, world_size - 1, i - 1);
            auto lower_neighbour = *get_lower_neighbour(&partial_matrix, w, world_size - 1, i - 1);

            MPI_Send(&upper_neighbour[0], w, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            MPI_Send(&lower_neighbour[0], w, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }

        create_gif( config_data["needed_iterations"].asInt() / config_data["delta_iterations"].asInt() );

    } else {
        int partial_matrix_width;

        MPI_Recv(&delta_x, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&delta_y, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&delta_t, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&alpha, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        MPI_Recv(&partial_matrix_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&partial_matrix_width, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        partial_matrix.resize(partial_matrix_size);

        std::vector<double> upper_neighbour(partial_matrix_width, 0);
        std::vector<double> lower_neighbour(partial_matrix_width, 0);

        MPI_Recv(&partial_matrix[0], partial_matrix_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        while (true) {

            MPI_Recv(&continue_signal, 1, MPI_C_BOOL, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&upper_neighbour[0], partial_matrix_width, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&lower_neighbour[0], partial_matrix_width, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (continue_signal) {

                calculate_heat(&partial_matrix, &upper_neighbour, &lower_neighbour, delta_x, delta_y, delta_t, alpha);

                MPI_Send(&partial_matrix_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                MPI_Send(&partial_matrix[0], partial_matrix_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

            } else {
                break;
            }
        }
    }
    MPI_Finalize();
}


