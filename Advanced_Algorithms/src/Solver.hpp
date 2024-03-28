#pragma once

#include "Config.hpp"
#include "Model.hpp"

void menu(Config& _config, Model& _model);

void sample_1();
void sample_2();

void solver_ILP_derval(Config& _config, Model& _model);
void solver_ILP_P_derval(Config& _config, Model& _model);
void solver_ILP_sinnl(Config& _config, Model& _model);
void solver_ILP_sinnl_submtx(Config& _config, Model& _model);
void solver_Benders_decomposition(Config& _config, Model& _model);
void solver_Benders_decomposition_cut(Config& _config, Model& _model);
void solver_ILP_IPCQ(Config& _config, Model& _model);
void solver_heuristic(Config& _config, Model& _model);

void solver_branch_and_cut(Config& _config, Model& _model);
void separation_heuristic_submtx(Config& _config, Model& _model);