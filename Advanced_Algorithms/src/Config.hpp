#pragma once

#include "Concept.hpp"

class Config
{
private:
	size_t _n_pertubations;
	real_t _acceptance_rate;
	size_t _max_iteration;
	string _directory_address;
	size_t _n_cut_loop_iteration;
	size_t _max_cut;
	real_t _eps;
	Method _method;
	size_t _k;

public:
	Config();
	Config(string _input_direcotry_address);

	/// <summary>
	/// Get acceptance rate between 0 till 100 and it will reduse to 0 to 1 by divided by 100
	/// </summary>
	void get_acceptance_rate(real_t _input_acceptance_rate);
	void get_n_pertubations(size_t _input_pertubations);
	void get_directory_address(string _input_direcotry_address);
	void get_max_iteration(size_t _input_max_iteration);
	void get_max_cut(size_t _input_max_cut);
	void get_n_cut_loop_iteration(size_t _input_n_cut_loop_iteration);
	void get_method(Method _input_method);
	void get_k(size_t _input_k);
	void get_eps(real_t _input_eps);

	size_t k();
	real_t eps();
	Method method();
	size_t n_pertubations();
	string directory_address();
	real_t acceptance_rate();
	size_t max_iteration();
	size_t max_cut();
	size_t n_cut_loop_iteration();
};