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

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

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

        int back_buffer;
        std::vector<double> c;

        for (int i = 1; i < world_size; i++){
            auto matrix_1 = *cut_matrix_stripe(&matrix, h, w, world_size - 1, i - 1);
//            represent_matrix(matrix_1, 50, 100);
//            std::cout << "\n\n";
            MPI_Send(new int( matrix_1.size() ) , 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&matrix_1[0] , matrix_1.size(), MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }

        for (int i = 1; i < world_size; i++){
            MPI_Recv(&back_buffer, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            c.resize(back_buffer);
            MPI_Recv(&c[0], back_buffer, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            put_into(&matrix, &c, 100, 100, world_size - 1, i - 1);
        }

        represent_matrix(&matrix, 100, 100);

    }
    else {
        int size;
        std::vector<double> b;

        MPI_Recv(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        b.resize(size);

        MPI_Recv(&b[0], size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int i = 0; i < b.size(); i++){
            if (b[i] == 0){
                b[i] = world_rank;
            }
        }

        MPI_Send(&size ,1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&b[0] , b.size(), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}