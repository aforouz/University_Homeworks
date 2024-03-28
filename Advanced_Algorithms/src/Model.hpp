#pragma once

#include "Matrix.hpp"

class Model
{
private:

	real_t _bound;
	size_t _cluster;
	real_t _value;
	Method _method;
	Matrix _matrix;
	Matrix _S;
	vector<Matrix> _result;
	vector<Submatrix> _heuristic;

	chrono::time_point<chrono::high_resolution_clock> _start_time;
	double _run_time;

public:
	Model();
	Model(size_t _input_cluster, Method _input_method, Matrix _input_matrix);

	void get_result(vector<Matrix>& _input_result);
	void get_value(real_t _input_value);
	void get_heuristic(vector<Submatrix>& _input_heuristic);
	void get_S(Matrix& _input_S);

	void calculate_value();
	void calculate_binary_S();

	void set_start_time();
	void set_end_time();

	Matrix& S();
	size_t cluster();
	Matrix& matrix();
	Method method();
	real_t bound();
	real_t value();

	double run_time();

	Submatrix& heuristic(size_t _index);
	vector<Submatrix>& heuristic();

	Matrix& result(size_t _index);
	Matrix submatrix(size_t _index);
	vector<Matrix>& result();
};