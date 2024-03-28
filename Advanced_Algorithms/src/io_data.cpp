#include "Main.hpp"

Config config_file()
{
	size_t temp_index;
	real_t temp_number;
	string temp_string;
	fstream fin;

	fin.open("config.txt", ios::in);

	if (fin.fail())
	{
		error(CONFIG_FILE);
	}

	getline(fin, temp_string);
	string directory_address(temp_string.begin() + 9, temp_string.end());

	Config config(directory_address);

	fin >> temp_string >> temp_index;
	config.get_k(temp_index);

	fin >> temp_string >> temp_index;

	switch (temp_index)
	{
	case 0:
		config.get_method(OTHER);
		break;
	case 1:
		config.get_method(ILP_DERVAL);
		break;
	case 2:
		config.get_method(ILP_P_DERVAL);
		break;
	case 3:
		config.get_method(ILP_SINNL);
		break;
	case 4:
		config.get_method(ILP_SINNL_SUBMTX);
		break;
	case 5:
		config.get_method(BENDERS_DECOMPOSITION);
		break;
	case 6:
		config.get_method(BENDERS_DECOMPOSITION_CUT);
		break;
	case 7:
		config.get_method(ILP_IPCQ);
		break;
	case 8:
		config.get_method(HEURISTIC);
		break;
	default:
		error(WRONG_METHOD);
		break;
	}

	fin >> temp_string >> temp_index;
	config.get_n_pertubations(temp_index);

	fin >> temp_string >> temp_number;
	config.get_acceptance_rate(temp_number);

	fin >> temp_string >> temp_index;
	config.get_max_iteration(temp_index);

	fin >> temp_string >> temp_number;
	config.get_eps(temp_number);

	fin >> temp_string >> temp_index;
	config.get_max_cut(temp_index);

	fin >> temp_string >> temp_index;
	config.get_n_cut_loop_iteration(temp_index);

	fin.close();

	return config;
}

Matrix scan_data(string _directory_address)
{
	fstream fin;
	Matrix data;
	size_t temp_row, temp_column;
	string address(_directory_address);

	if (*(address.end() - 1) != '/')
	{
		address += "/";
	}
	address += "input.txt";

	fin.open(address, ios::in);
	
	if (fin.fail())
	{
		error(DATA_FILE);
	}

	fin >> temp_row >> temp_column;

	data.allocate(temp_row, temp_column);
	for (size_t i(0); i < data.row(); ++i)
	{
		for (size_t j(0); j < data.column(); ++j)
		{
			fin >> data[i][j];
		}
	}

	fin.close();
	return data;
}

void print_data(Model _model, string _directory_address)
{
	fstream fout;
	string address(_directory_address);
	if (*(address.end() - 1) != '/')
	{
		address += "/";
	}
	address += "output.txt";

	fout.open(address, ios::out);

	if (fout.fail())
	{
		error(RESULT_FILE);
	}

	fout << "Time: "
		<< fixed
		<< setprecision(4)
		<< _model.run_time()
		<< "s"
		<< endl
		<< "--------------------------------------------------"
		<< endl;

	fout << "Value: "
		<< fixed
		<< setprecision(6)
		<< _model.value()
		<< endl
		<< "--------------------------------------------------"
		<< endl;

	fout << "Main matrix: "
		<< endl
		<< _model.matrix()
		<< endl
		<< "--------------------------------------------------"
		<< endl;

	for (size_t i(0); i < _model.cluster(); ++i)
	{
		fout << "Submatrix: "
			<< i + 1
			<< endl
			<< _model.submatrix(i)
			<< endl
			<< "--------------------------------------------------"
			<< endl;
	}

	fout.close();
}