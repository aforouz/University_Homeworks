#pragma once

#include "Solver.hpp"

Config config_file();
Matrix scan_data(string _directory_address);
Model init_model(Config& _config, Matrix _matrix);
void print_data(Model _model, string _directory_address);
