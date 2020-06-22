#include <iostream>
#include <mpi.h>
#include <vector>
#include "headers/config_processing.h"
#include "headers/matrix_manipulations.h"

int main(int argc, char *argv[])
{

    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

//    char processor_name[MPI_MAX_PROCESSOR_NAME];
//    int name_len;
//    MPI_Get_processor_name(processor_name, &name_len);

    std::vector<double> partial_matrix;
    int partial_matrix_size;

    if (world_rank == 0) {
        auto config_data = read_config("../config.json");

        int w = config_data["width"].asInt();
        int h = config_data["height"].asInt();
        int delta_x = config_data["delta_x"].asInt();
        int delta_y = config_data["delta_y"].asInt();
        int delta_t = config_data["delta_t"].asInt();

        w = w / delta_x;
        h = h / delta_y;

        auto matrix = *initialize_matrix(h, w);
        add_all_temperatures_to_matrix(&matrix, &config_data, h, w);

        for (int i = 1; i < world_size; i++){
            partial_matrix = *cut_matrix_stripe(&matrix, h, w, world_size - 1, i - 1);
            MPI_Send(new int( partial_matrix.size() ) , 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&partial_matrix[0] , partial_matrix.size(), MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }

        for (int i = 1; i < world_size; i++){
            MPI_Recv(&partial_matrix_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            partial_matrix.resize(partial_matrix_size);

            MPI_Recv(&partial_matrix[0], partial_matrix_size, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            put_into(&matrix, &partial_matrix, 100, 100, world_size - 1, i - 1);
        }

        represent_matrix(&matrix, 100, 100);

    } else {

        MPI_Recv(&partial_matrix_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        partial_matrix.resize(partial_matrix_size);

        MPI_Recv(&partial_matrix[0], partial_matrix_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int i = 0; i < partial_matrix_size; i++){
            if (partial_matrix[i] == 0){
                partial_matrix[i] = world_rank;
            }
        }

        MPI_Send(&partial_matrix_size ,1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&partial_matrix[0] , partial_matrix_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}