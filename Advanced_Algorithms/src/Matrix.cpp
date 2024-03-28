#include "Matrix.hpp"

Submatrix::Submatrix()
{
}

Submatrix::Submatrix(size_t _max_row, size_t _max_column)
{
}

void Submatrix::get_row(size_set _row)
{
	_R = _row;
}

void Submatrix::get_column(size_set _column)
{
	_C = _column;
}

void Submatrix::insert_row(size_t _row)
{
	_R.insert(_row);
}

void Submatrix::insert_column(size_t _column)
{
	_C.insert(_column);
}

void Submatrix::insert_node(size_t _row, size_t _column)
{
	_R.insert(_row);
	_C.insert(_column);
}

void Submatrix::erase_row(size_t _row)
{
	_R.erase(_row);
}

size_vec Submatrix::R()
{
	return size_vec(_R.begin(), _R.end());
}

size_vec Submatrix::C()
{
	return size_vec(_C.begin(), _C.end());
}

bool Submatrix::R(size_t _row)
{
	return !(_R.find(_row) == _R.end());
}

bool Submatrix::C(size_t _column)
{
	return !(_C.find(_column) == _C.end());
}

// **************************************************#**************************************************
// **************************************************#**************************************************
// **************************************************#**************************************************
// **************************************************#**************************************************
// **************************************************#**************************************************

Matrix::Matrix()
	:
	_row(0),
	_column(0)
{
}

Matrix::Matrix(size_t _input_row, size_t _input_column)
	:
	_row(_input_row),
	_column(_input_column),
	_data(_input_row, real_vec(_input_column, 0))
{
}

void Matrix::allocate(size_t _input_row, size_t _input_column)
{
	_row = _input_row;
	_column = _input_column;
	_data = vector<real_vec>(_row, real_vec(_column, 0));
}

void Matrix::delete_negative()
{
	bool flag;
	size_t i, j;

	// delete row
	for (i = 0; i < _row; ++i)
	{
		flag = true;
		for (j = 0; j < _column; ++j)
		{
			if (_data[i][j] > 0)
			{
				flag = false;
				break;
			}
		}
		if (flag)
		{
			_data.erase(_data.begin() + i);
			--i;
			--_row;
		}
	}

	// delete column
	for (j = 0; j < _column; ++j)
	{
		flag = true;
		for (i = 0; i < _row; ++i)
		{
			if (_data[i][j] > 0)
			{
				flag = false;
				break;
			}
		}
		if (flag)
		{
			for (i = 0; i < _row; ++i)
			{
				_data[i].erase(_data[i].begin() + j);
			}
			--j;
			--_column;
		}
	}
}

void Matrix::transpose()
{
	vector<real_vec> temp(_column, real_vec(_row, 0));
	for (size_t i(0); i < _row; ++i)
	{
		for (size_t j(0); j < _column; ++j)
		{
			temp[j][i] = _data[i][j];
		}
	}
	_data = temp;
	swap(_row, _column);
}

real_vec& Matrix::operator[](size_t _index)
{
	return _data[_index];
}

size_t Matrix::row()
{
	return _row;
}

size_t Matrix::column() 
{
	return _column;
}

ostream& operator<<(ostream& _out, Matrix _obj)
{
	_out << resetiosflags(ios::floatfield);
	for (size_t i(0); i < _obj.row(); ++i)
	{
		for (size_t j(0); j < _obj.column(); ++j)
		{
			_out << setw(8) << setprecision(6) << _obj[i][j] << '\t';
		}
		_out << endl;
	}
	return _out;
}

