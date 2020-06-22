//
// Created by vlad on 19.06.20.
//

#ifndef MPI_CONFIG_PROCESSING_H
#define MPI_CONFIG_PROCESSING_H

#include "../src/config_processing.cpp"

inline bool file_exists (std::string name);

Json::Value read_config(std::string filename);

#endif //MPI_CONFIG_PROCESSING_H
