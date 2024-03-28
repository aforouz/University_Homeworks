#include "Config.hpp"

Config::Config()
	:
	_max_iteration(0),
	_n_pertubations(0),
	_acceptance_rate(0)
{
}

Config::Config(string _input_direcotry_address)
	:
	_max_iteration(0),
	_n_pertubations(0),
	_acceptance_rate(0),
	_directory_address(_input_direcotry_address)
{
}

void Config::get_n_pertubations(size_t _input_pertubations)
{
	_n_pertubations = _input_pertubations;
}

void Config::get_directory_address(string _input_director_address)
{
	_directory_address = _input_director_address;
}

void Config::get_acceptance_rate(real_t _input_acceptance_rate)
{
	_acceptance_rate = _input_acceptance_rate/100;
}

void Config::get_max_iteration(size_t _input_max_iteration)
{
	_max_iteration = _input_max_iteration;
}

void Config::get_max_cut(size_t _input_max_cut)
{
	_max_cut = _input_max_cut;
}

void Config::get_n_cut_loop_iteration(size_t _input_n_cut_loop_iteration)
{
	_n_cut_loop_iteration = _input_n_cut_loop_iteration;
}

void Config::get_method(Method _input_method)
{
	_method = _input_method;
}

void Config::get_k(size_t _input_k)
{
	_k = _input_k;
}

void Config::get_eps(real_t _input_eps)
{
	_eps = _input_eps;
}

size_t Config::k()
{
	return _k;
}

real_t Config::eps()
{
	return _eps;
}

Method Config::method()
{
	return _method;
}

real_t Config::acceptance_rate()
{
	return _acceptance_rate;
}

size_t Config::max_iteration()
{
	return _max_iteration;
}

size_t Config::max_cut()
{
	return _max_cut;
}

size_t Config::n_cut_loop_iteration()
{
	return _n_cut_loop_iteration;
}

size_t Config::n_pertubations()
{
	return _n_pertubations;
}

string Config::directory_address()
{
	return _directory_address;
}