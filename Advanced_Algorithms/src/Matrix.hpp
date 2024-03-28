#pragma once

#include "Concept.hpp"

class Submatrix
{
private:
	size_set _R;
	size_set _C;

public:
	Submatrix();
	Submatrix(size_t _max_row, size_t _max_column);

	void get_row(size_set _row);
	void get_column(size_set _column);

	void insert_row(size_t _row);
	void insert_column(size_t _column);
	void insert_node(size_t _row, size_t _column);

	void erase_row(size_t _row);

	size_vec R();
	size_vec C();

	bool R(size_t _row);
	bool C(size_t _column);
};

// **************************************************#**************************************************
// **************************************************#**************************************************
// **************************************************#**************************************************
// **************************************************#**************************************************
// **************************************************#**************************************************

class Matrix
{
private:
	vector<real_vec> _data;
	size_t _row;
	size_t _column;

public:
	Matrix();
	Matrix(size_t _input_row, size_t _input_column);

	void allocate(size_t _input_row, size_t _input_column);
	void delete_negative();

	void transpose();

	real_vec& operator[](size_t _index);

	size_t row();

	size_t column();

	friend ostream& operator<<(ostream& _out, Matrix _obj);
};