#include "Model.hpp"

Model::Model()
	:
	_run_time(0),
	_bound(0),
	_value(0),
	_cluster(0),
	_method(OTHER)
{
}

Model::Model(size_t _input_cluster, Method _input_method, Matrix _input_matrix)
	:
	_run_time(0),
	_bound(0),
	_value(0),
	_cluster(_input_cluster),
	_method(_input_method),
	_matrix(_input_matrix)
{
	for (size_t i(0); i < _matrix.row(); ++i)
	{
		for (size_t j(0); j < _matrix.column(); ++j)
		{
			if (_matrix[i][j] > 0)
			{
				_bound += _matrix[i][j];
			}
		}
	}
}

void Model::get_result(vector<Matrix>& _input_result)
{
	_result = _input_result;
}

void Model::get_value(real_t _input_value)
{
	_value = _input_value;
}

void Model::get_heuristic(vector<Submatrix>& _input_heuristic)
{
	_heuristic = _input_heuristic;

	_result = vector<Matrix>(_cluster, Matrix(_matrix.row(), _matrix.column()));
	for (size_t k(0); k < _cluster; ++k)
	{
		for (auto i : _heuristic[k].R())
		{
			for (auto j : _heuristic[k].C())
			{
				_result[k][i][j] = 1; // Just for bool
			}
		}
	}
}

void Model::get_S(Matrix& _input_S)
{
	_S = _input_S;
}

void Model::calculate_value()
{
	calculate_binary_S();
	_value = 0;
	size_t i, j;
	for (i = 0; i < _matrix.row(); ++i)
	{
		for (j = 0; j < _matrix.column(); ++j)
		{
			if (_S[i][j] > EPS)
			{
				_value += _matrix[i][j];
			}
		}
	}
}

void Model::calculate_binary_S()
{
	size_t i, j, k;
	_S = Matrix(_matrix.row(), _matrix.column());
	for (i = 0; i < _matrix.row(); ++i)
	{
		for (j = 0; j < _matrix.column(); ++j)
		{
			for (k = 0; k < _cluster; ++k)
			{
				if (_result[k][i][j] > EPS)
				{
					_S[i][j] = 1;
					break;
				}
			}
		}
	}
}

void Model::set_start_time()
{
	_start_time = chrono::high_resolution_clock::now();
}

void Model::set_end_time()
{
	_run_time = (chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - _start_time).count()) / 1000000000.0;
}

double Model::run_time()
{
	return _run_time;
}

Submatrix& Model::heuristic(size_t _index)
{
	return _heuristic[_index];
}

vector<Submatrix>& Model::heuristic()
{
	return _heuristic;
}

size_t Model::cluster()
{
	return _cluster;
}

Matrix& Model::matrix()
{
	return _matrix;
}

Method Model::method()
{
	return _method;
}

real_t Model::bound()
{
	return _bound;
}

Matrix& Model::result(size_t _index)
{
	return _result[_index];
}

vector<Matrix>& Model::result()
{
	return _result;
}

real_t Model::value()
{
	return _value;
}

Matrix Model::submatrix(size_t _index)
{
	size_t _i, _j;
	Matrix _submatrix(_result[_index]);
	for (_i = 0; _i < _matrix.row(); ++_i)
	{
		for (_j = 0; _j < _matrix.column(); ++_j)
		{
			if (_submatrix[_i][_j] > EPS)
			{
				_submatrix[_i][_j] = _matrix[_i][_j];
			}
		}
	}
	return _submatrix;
}

Matrix& Model::S()
{
	return _S;
}