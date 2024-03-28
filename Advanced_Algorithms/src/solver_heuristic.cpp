#include "Solver.hpp"

Matrix M;

real_t calculate(vector<Submatrix>& _RC)
{
	real_t value(0);
	size_t i, j, k;
	size_vec R, C;
	for (i = 0; i < M.row(); ++i)
	{
		for (j = 0; j < M.column(); ++j)
		{
			for (k = 0; k < _RC.size(); ++k)
			{
				if (_RC[k].R(i) && _RC[k].C(j)) // duplicate
				{
					value += M[i][j];
					break;
				}
			}
		}
	}
	return value;
}

bool row_removal(vector<Submatrix>& _RC, size_t _kp, size_t _i, bool _flag = false)
{
	if (_flag) // change solution
	{
		_RC[_kp].erase_row(_i);
		return false;
	}
	vector<Submatrix> RC(_RC);
	RC[_kp].erase_row(_i);
	return calculate(RC) > calculate(_RC);
}

bool row_addition(vector<Submatrix>& _RC, size_t _kp, size_t _i, bool _flag = false)
{
	if (_flag) // change solution
	{
		_RC[_kp].insert_row(_i);
		return false;
	}
	vector<Submatrix> RC(_RC);
	RC[_kp].insert_row(_i);
	return calculate(RC) > calculate(_RC);
}

bool row_move(vector<Submatrix>& _RC, size_t _kp, size_t _kpp, size_t _i, bool _flag = false)
{
	if (_flag) // change solution
	{
		_RC[_kp].insert_row(_i);
		_RC[_kpp].erase_row(_i);
		return false;
	}
	vector<Submatrix> RC(_RC);
	RC[_kp].insert_row(_i);
	RC[_kpp].erase_row(_i);
	return calculate(RC) > calculate(_RC);
}

void row_exchange(vector<Submatrix>& _RC, size_t _kp, size_t _kpp, size_t _ip, size_t _ipp)
{
	_RC[_kp].erase_row(_ip);
	_RC[_kp].insert_row(_ipp);
	_RC[_kpp].erase_row(_ipp);
	_RC[_kpp].insert_column(_ip);
}

void local_search(Config& _config, Model& _model)
{
	M = _model.matrix();
	bool improved(true);
	size_t i;
	size_t pertubations;
	size_t nR(_model.matrix().row());
	size_t nK(_model.cluster());
	size_t ip, ipp, kp, kpp;
	size_vec ind(nK);
	size_set Rkp, Rkpp;
	vector<Submatrix> RC(_model.heuristic());

	for (pertubations = 0; pertubations < _config.n_pertubations() && improved; ++pertubations)
	{
		LINE2:
		improved = false;

		for (kp = 0; kp < nK; ++kp)
		{
			for (auto iR : RC[kp].R())
			{
				// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// rowRemoval
				improved = row_removal(RC, kp, iR);

				if (improved)
				{
					// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
					// rowRemovalSolution
					row_removal(RC, kp, iR, true);
					goto LINE2;
				}
			}
		}

		for (kp = 0; kp < nK; ++kp)
		{
			size_vec R(RC[kp].R());
			for (i = 0; i < nR; ++i)
			{
				if (find(R.begin(), R.end(), i) == R.end())
				{
					// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
					// rowAddition
					improved = row_addition(RC, kp, i);

					if (improved)
					{
						// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
						// rowAdditionSolution
						row_addition(RC, kp, i, true);
						goto LINE2;
					}
				}
			}
		}

		for (kp = 0; kp < nK; ++kp)
		{
			for (auto iR : RC[kp].R())
			{
				for (kpp = 0; kpp < nK; ++kpp)
				{
					size_vec R(RC[kpp].R());
					if ((kpp != kp) && (find(R.begin(), R.end(), iR) == R.end()))
					{
						// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
						// rowMove
						improved = row_move(RC, kp, kpp, iR);

						if (improved)
						{
							// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
							// rowMoveSolution
							row_move(RC, kp, kpp, iR, true);
							goto LINE2;
						}
					}
				}
			}
		}

		LINE22:
		// perform a random rowExchange as the other operators were not successfull
		for (i = 0; i < nK; ++i)
		{
			ind[i] = i;
		}

		kp = random_number(nK);
		ip = ind[kp];
		ind.erase(ind.begin() + kp);
		kpp = random_number(nK - 1);
		ipp = ind[kpp];

		if ((Rkp.find(ipp) != Rkp.end()) || (Rkpp.find(ip) != Rkpp.end()))
		{
			goto LINE22;
		}
		
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// rowExchangeSolution
		row_exchange(RC, kp, kpp, ip, ipp);
	}

	_model.get_heuristic(RC);
	_model.calculate_value();
}

void solver_heuristic(Config& _config, Model& _model)
{
	bool found;
	size_t i, j, k;
	size_t is;
	size_t nR(_model.matrix().row());
	size_t nC(_model.matrix().column());
	size_t nK(_model.cluster());
	Matrix M(_model.matrix());
	real_t zG(0), rowValue(0);
	real_t currentBestObjSubmatrix(0);
	real_t colValueTesteRatej(0);
	
	real_vec colSum;

	size_set R, Rk, Cis;
	size_set::iterator iR;

	vector<size_set> C;
	Submatrix RCk;
	vector<Submatrix> RC(nK);

	for (k = 0; k < nK; ++k)
	{
		for (i = 0; i < nR; ++i)
		{
			R.insert(i);
		}

		Rk.clear(); // rows of the currently constructed submatrix
		colSum = real_vec(nC, 0); // column-sum for the currently considered rows
		found = true;
		currentBestObjSubmatrix = 0;

		for (size_t it(0); it < _config.max_iteration() && found; ++it) // set max iter
		{
			found = false;
			is = 0; // chosen row to add
			Cis.clear(); // best columns for the chose row to add

			C = vector<size_set>(R.size());
			for (i = 0, iR = R.begin(); iR != R.end(); ++iR, ++i)
			{
				rowValue = 0; // contains the objective of the current submatrix when row i is included
				C[i].clear();
				// check for all columns if they should be included when selecting rows R k ∪ { i }
				for (j = 0; j < nC; ++j)
				{
					colValueTesteRatej = colSum[j] + M[*iR][j];
					// colum-sum when including row i
					if (colValueTesteRatej > 0)
					{
						C[i].insert(j);
						rowValue += colValueTesteRatej;
					}
				}

				// adding row i would improve the current submatrix
				if (rowValue > currentBestObjSubmatrix)
				{
					if (random_number() >= _config.acceptance_rate())
					{
						found = true;
						Cis = C[i];
						currentBestObjSubmatrix = rowValue;
						is = i;
					}
				}
			}

			// Update the selected rows, columns and colSum j for the next iteration
			if (found)
			{
				RCk.get_column(Cis);
				RCk.insert_row(is);
				R.erase(is);

				for (j = 0; j < nC; ++j)
				{
					colSum[j] += M[is][j];
				}
			}
		}

		// Update the object value and Mij
		// R = RCk.R();
		// C = RCk.C();
		for (auto ik : RCk.R())
		{
			for (auto jk : RCk.C())
			{
				if (abs(M[ik][jk]) > EPS)
				{
					zG += M[ik][jk];
					M[ik][jk] = 0;
				}
			}
		}
		RC[k] = RCk;
	}

	_model.get_value(zG);
	_model.get_heuristic(RC);

	local_search(_config, _model);
}