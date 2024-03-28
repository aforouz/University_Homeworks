#include "Solver.hpp"

// ILP branch and cut solution pseudocode, assuming objective is to bemaximized
void separation_heuristic_submtx(Config& _config, Model& _model)
{
	real_t score_cut_off(_model.value());
	real_t score(0), j_star_score(0);
	Matrix M(_model.matrix());
	size_t i, j, k, ip, jp;
	int64_t j_star;
	size_t nR(M.row()), nC(M.column());
	size_t nK(_model.cluster());
	size_t n_violated_ineqs(0);
	vector<bool_vec> usedInS(nR, bool_vec(nC, false));
	Matrix s_star(_model.S());
	size_vec R;
	set<size_pair> S, TS, TS_j_star, TS_j;
	bool_vec usedRowS, usedColS;
	vector<Matrix> Database(_model.result());

	/*
		// if usedInS was a real matrix
		for (i = 0; i < nR; ++i)
		{
			for (j = 0; j < nC; ++j)
			{
				if (M[i][j] <= 0)
				{
					usedInS[i][j] = -1;
				}
			}
		}
	*/

	for (i = 0; i < nR; ++i)
	{
		for (j = 0; j < nC; ++j)
		{
			// pick the next available cell to start building a new set S
			if ((s_star[i][j] >= 1 - _config.eps()) && (M[i][j] > 0) && (usedInS[i][j] == 0))
			{
				S.insert(size_pair(i, j));
				TS.clear();

				usedRowS = bool_vec(nR, false);
				usedRowS[i] = true;

				usedColS = bool_vec(nC, false);
				usedColS[j] = true;

				// build the set S row-wise, randomly shuffle the rows to obtain a more diverse set of inequalities
				random_permutation(nR, R);
				for (ip = 0; ip < nR; ++ip)
				{
					if ((ip == i) || (usedRowS[ip] > 0))
					{
						continue;
					}

					// 11
					j_star = -1;
					j_star_score = score_cut_off;
					TS_j_star.clear();

					for (jp = 0; jp < nC; ++jp)
					{
						// try to find cell in row i to grow S
						if ((s_star[ip][jp] >= 1 - _config.eps()) && (M[ip][jp] > 0) && (usedInS[ip][jp] == 0) && (usedColS[jp] == 0))
						{
							score = 0;
							// 14
							TS_j.clear();

							// calculate the score for adding (i,j) to S and also keep track of the set T(S, j) which would be obtained by adding(i, j) to T(S)
							for (auto ijpp : S)
							{

								if (s_star[ijpp.first][jp] < s_star[ip][ijpp.second])
								{
									score += s_star[ijpp.first][jp];
									// 17
									TS_j.insert(size_pair(ijpp.first, jp));
								}
								else
								{
									score += s_star[ip][ijpp.second];
									// 19
									TS_j.insert(size_pair(ip, ijpp.second));
								}
							}
							if (score < j_star_score)
							{
								j_star = jp;
								j_star_score = score;
								// 21
								TS_j_star = TS_j;
							}
						}
					}
					// grow the set S if a suitable cell (i, j ∗) was found and update T(S) accordingly
					if ((j_star > -1) && (j_star_score < 1))
					{
						S.insert(size_pair(ip, j_star));
						TS.merge(TS_j_star);
						usedRowS[ip] = true;
						usedColS[jp] = true;
					}
				}
				// 25
//				if (violated())
				{
					for (auto ijp : S)
					{
						usedInS[ijp.first][ijp.second] = true;
					}
					// 27
					++n_violated_ineqs;
					if (n_violated_ineqs >= _config.max_cut())
					{
						// 30
						return;
					}
				}
			}
		}
	}
//	if ()
	{
		score_cut_off += 0.1;
	}
}