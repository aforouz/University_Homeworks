#pragma once

#include "Header.hpp"

// Namespace
using namespace std;

// Type
typedef double real_t;
typedef vector<real_t> real_vec;
typedef vector<bool> bool_vec;
typedef vector<size_t> size_vec;
typedef set<size_t> size_set;
typedef pair<size_t, size_t> size_pair;

typedef IloArray<IloNumVarArray> IloNumVarMatrix;
typedef IloArray<IloNumVarMatrix> IloNumVarCube;

// Concept
constexpr auto EPS = 1e-15;

// ENUM
typedef enum { OTHER, ILP_DERVAL, ILP_P_DERVAL, ILP_SINNL, ILP_SINNL_SUBMTX, BENDERS_DECOMPOSITION, BENDERS_DECOMPOSITION_CUT, ILP_IPCQ, HEURISTIC } Method;
typedef enum { DATA_FILE, CONFIG_FILE, WRONG_METHOD, RESULT_FILE, ILP_FEASIBLE, NOT_BINARY } Error;

// Error function
void error(Error _error_code);

// Binary
bool is_binary(real_t _value);

// Random
class RandomGenerator
{
public:
	RandomGenerator();

	/// <summary>
	/// Generate a real number between 0 and 1
	/// </summary>
	real_t generate();

private:
	random_device _random_device;
	mt19937 _random_engine;
	uniform_real_distribution<real_t> _distribution;
};

/// <summary>
/// Generate a real number between 0 and 1
/// </summary>
real_t random_number();

/// <summary>
/// Give a random number between 1 and input number.
/// </summary>
/// <param name = "_max_number">
/// Maximum number.
/// </param>
size_t random_number(size_t _max_number);

/// <summary>
/// Make a vector of random numbers.
/// </summary>
/// <param name = "_random_count">
/// Maximum random number and count of them.
/// </param>
/// <param name = "_random_vector">
/// Vector of random numbers. It would allocate in function.
/// </param>
void random_permutation(size_t _random_count, vector<size_t>& _random_vector);