#include "Main.hpp"
#include "Solver.hpp"

void menu(Config& _config, Model& _model)
{
	switch (_model.method())
	{
	case ILP_DERVAL:
		solver_ILP_derval(_config, _model);
		break;

	case ILP_P_DERVAL:
		solver_ILP_P_derval(_config, _model);
		break;

	case ILP_SINNL:
		solver_ILP_sinnl(_config, _model);
		break;

	case ILP_SINNL_SUBMTX:
		solver_ILP_sinnl_submtx(_config, _model);
		break;

	case BENDERS_DECOMPOSITION:
		solver_Benders_decomposition(_config, _model);
		break;

	case BENDERS_DECOMPOSITION_CUT:
		solver_Benders_decomposition_cut(_config, _model);
		break;

	case ILP_IPCQ:
		solver_ILP_IPCQ(_config, _model);
		break;

	case HEURISTIC:
		solver_heuristic(_config, _model);
		break;

	case OTHER:
		sample_1();
		sample_2();
		break;

	default:
		error(WRONG_METHOD);
		break;
	}
}