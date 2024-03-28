#include "Concept.hpp"

void error(Error _error_code)
{
	switch (_error_code)
	{
	case DATA_FILE:
		cerr << "No access to data file." << endl;
		break;
	case RESULT_FILE:
		cerr << "No access to result file." << endl;
		break;
	case CONFIG_FILE:
		cerr << "No access to config file." << endl;
		break;
	case WRONG_METHOD:
		cerr << "Wrong method code." << endl;
		break;
	case ILP_FEASIBLE:
		cerr << "Solution is not feasible" << endl;
		break;
	case NOT_BINARY:
		cerr << "Solution is not binary" << endl;
		break;
	default:
		cerr << "An unknown error." << endl;
		break;
	}
	system("pause");
	exit(EXIT_FAILURE);
}

// **************************************************^**************************************************
// Binary

bool is_binary(real_t _value)
{
	if (abs(_value - 1) < EPS || abs(_value) < EPS)
	{
		return true;
	}
	return false;
}

// **************************************************^**************************************************
// RandomGenerator

RandomGenerator::RandomGenerator()
	:
	_random_engine(_random_device()), _distribution(0, 1)
{}

double RandomGenerator::generate()
{
	return _distribution(_random_engine);
}

RandomGenerator _random_generator;

real_t random_number()
{
	return _random_generator.generate();
}

size_t random_number(size_t _max_number)
{
	return size_t(floor(_random_generator.generate() * _max_number));
}

void random_permutation(size_t _random_count, vector<size_t>& _random_vector)
{
	// TODO: insert return statement here
	_random_vector = vector<size_t>(_random_count);
	vector<pair<double, size_t>> _rpv(_random_count); // random permutation vector

	for (size_t _i(0); _i < _random_count; ++_i)
	{
		_rpv[_i] = make_pair(_random_generator.generate(), _i);
	}

	sort(_rpv.begin(), _rpv.end());

	for (size_t _i(0); _i < _random_count; ++_i)
	{
		_random_vector[_i] = _rpv[_i].second;
	}
}
