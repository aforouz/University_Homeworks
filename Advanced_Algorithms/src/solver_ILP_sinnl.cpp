#include "Solver.hpp"

void solver_ILP_sinnl(Config& _config, Model& _model)
{
    size_t i, j, k, ip, jp;
    size_t nR(_model.matrix().row());
    size_t nC(_model.matrix().column());
    size_t nK(_model.cluster());
    Matrix M(_model.matrix());

    // **************************************************#**************************************************
    // Model Definition
    IloEnv myenv; // environment object
    IloModel mymodel(myenv); // model object

    // **************************************************#**************************************************
    // Variable
    IloNumVarMatrix s(myenv, nR);
    for (i = 0; i < nR; ++i)
    {
        s[i] = IloNumVarArray(myenv, nC, 0, 1, ILOBOOL); // ILOBOOL  ILOFLOAT
    }

    IloNumVarCube f(myenv, nR);
    for (i = 0; i < nR; ++i)
    {
        f[i] = IloNumVarMatrix(myenv, nC);
        for (j = 0; j < nC; ++j)
        {
            if (M[i][j] > 0)
            {
                f[i][j] = IloNumVarArray(myenv, nK, 0, 1, ILOBOOL); // ILOBOOL  ILOFLOAT
            }
        }
    }

    // **************************************************#**************************************************
    // SF-LINK1
    for (i = 0; i < nR; ++i)
    {
        for (j = 0; j < nC; ++j)
        {
            for (ip = 0; ip < nR; ++ip)
            {
                if (ip == i)
                {
                    continue;
                }
                
                for (jp = 0; jp < nC; ++jp)
                {
                    if (jp == j)
                    {
                        continue;
                    }
                    if (M[i][j] > 0 && M[ip][jp] > 0 && M[ip][j] < 0)
                    {
                        for (k = 0; k < nK; ++k)
                        {
                            mymodel.add(s[ip][j] + 1 >= f[i][j][k] + f[ip][jp][k]);
                        }
                    }
                }
            }
        }
    }

    // SF-LINK2
    for (i = 0; i < nR; ++i)
    {
        for (j = 0; j < nC; ++j)
        {
            if (M[i][j] > 0)
            {
                IloExpr myexpr(myenv);
                for (k = 0; k < nK; ++k)
                {
                    myexpr += f[i][j][k];
                }
                myexpr -= s[i][j];
                mymodel.add(IloRange(myenv, 0, myexpr, IloInfinity));
            }
        }
    }

    // **************************************************#**************************************************
    // Subject 
    IloExpr myexpr(myenv);
    for (i = 0; i < nR; ++i)
    {
        for (j = 0; j < nC; ++j)
        {
            myexpr += M[i][j] * s[i][j];
        }
    }
    mymodel.add(IloMaximize(myenv, myexpr)); // objective max 5x + 15y

    // **************************************************#**************************************************
    // Model Solution
    IloCplex mycplex(myenv);
    mycplex.extract(mymodel);
    IloBool feasible = mycplex.solve(); // solves model and stores whether or ...

    // **************************************************#**************************************************
    // Printing the Solution
    if (feasible == IloFalse)
    {
        error(ILP_FEASIBLE);
    }
    _model.get_value(mycplex.getObjValue());

    vector<bool_vec> r(nR, bool_vec(nK, false));
    vector<bool_vec> c(nC, bool_vec(nK, false));
    for (i = 0; i < nR; ++i)
    {
        for (j = 0; j < nC; ++j)
        {
            if (mycplex.getValue(s[i][j]) > EPS && M[i][j] > 0)
            {
                for (k = 0; k < nK; ++k)
                {
                    if (mycplex.getValue(f[i][j][k]) > EPS)
                    {
                        r[i][k] = true;
                        c[j][k] = true;
                    }
                }
            }
        }
    }

    vector<Matrix> result(nK, Matrix(nR, nC));
    for (i = 0; i < nR; ++i)
    {
        for (j = 0; j < nC; ++j)
        {
            if (mycplex.getValue(s[i][j]) > EPS)
            {
                for (k = 0; k < nK; ++k)
                {
                    if (r[i][k] && c[j][k]) // mycplex.getValue(f[i][j][k]) can't be used
                    {
                        result[k][i][j] = 1; // Just for bool
                    }
                }
            }
        }
    }
    _model.get_result(result);

    // **************************************************#**************************************************
    // Closing the Model
    mycplex.clear();
    myenv.end();
}

// **************************************************#**************************************************
// **************************************************#**************************************************
// **************************************************#**************************************************
// **************************************************#**************************************************
// **************************************************#**************************************************

void solver_ILP_sinnl_submtx(Config& _config, Model& _model)
{
    size_t i, j, k, ip, jp;
    size_t nR(_model.matrix().row());
    size_t nC(_model.matrix().column());
    size_t nK(_model.cluster());
    Matrix M(_model.matrix());

    // **************************************************#**************************************************
    // Model Definition
    IloEnv myenv; // environment object
    IloModel mymodel(myenv); // model object

    // **************************************************#**************************************************
    // Variable
    IloNumVarMatrix s(myenv, nR);
    for (i = 0; i < nR; ++i)
    {
        s[i] = IloNumVarArray(myenv, nC, 0, 1, ILOBOOL); // ILOBOOL  ILOFLOAT
    }

    IloNumVarCube f(myenv, nR);
    for (i = 0; i < nR; ++i)
    {
        f[i] = IloNumVarMatrix(myenv, nC);
        for (j = 0; j < nC; ++j)
        {
            if (M[i][j] > 0)
            {
                f[i][j] = IloNumVarArray(myenv, nK, 0, 1, ILOBOOL); // ILOBOOL  ILOFLOAT
            }
        }
    }

    // **************************************************#**************************************************
    // SF-LINK1
    for (i = 0; i < nR; ++i)
    {
        for (j = 0; j < nC; ++j)
        {
            for (ip = 0; ip < nR; ++ip)
            {
                if (ip == i)
                {
                    continue;
                }

                for (jp = 0; jp < nC; ++jp)
                {
                    if (jp == j)
                    {
                        continue;
                    }
                    if (M[i][j] > 0 && M[ip][jp] > 0 && M[ip][j] < 0)
                    {
                        for (k = 0; k < nK; ++k)
                        {
                            mymodel.add(s[ip][j] + 1 >= f[i][j][k] + f[ip][jp][k]);
                        }
                    }
                }
            }
        }
    }

    // SF-LINK2
    for (i = 0; i < nR; ++i)
    {
        for (j = 0; j < nC; ++j)
        {
            if (M[i][j] > 0)
            {
                IloExpr myexpr(myenv);
                for (k = 0; k < nK; ++k)
                {
                    myexpr += f[i][j][k];
                }
                myexpr -= s[i][j];
                mymodel.add(IloRange(myenv, 0, myexpr, IloInfinity));
            }
        }
    }

    // SUBMATX

    // SF-LINKG

    // ILP-SYM
    for (k = 0; k < nK - 1; ++k)
    {
        for (i = 0; i < nR; ++i)
        {
            for (j = 0; j < nC; ++j)
            {
                if (M[i][j] > 0)
                {
                    IloExpr myexpr(myenv);
                    for (ip = 0; ip < i; ++ip)
                    {
                        for (jp = 0; jp < nC; ++jp)
                        {
                            if (M[ip][jp] > 0)
                            {
                                myexpr += f[ip][jp][k];
                            }
                        }
                    }
                    
                    for (jp = 0; jp <= j; ++jp)
                    {
                        if (M[i][jp] > 0)
                        {
                            myexpr += f[i][jp][k];
                        }
                    }

                    myexpr -= f[i][j][k + 1];
                    mymodel.add(IloRange(myenv, 0, myexpr, IloInfinity));
                }
            }
        }
    }

    // **************************************************#**************************************************
    // Subject 
    IloExpr myexpr(myenv);
    for (i = 0; i < nR; ++i)
    {
        for (j = 0; j < nC; ++j)
        {
            myexpr += M[i][j] * s[i][j];
        }
    }
    mymodel.add(IloMaximize(myenv, myexpr)); // objective max 5x + 15y

    // **************************************************#**************************************************
    // Model Solution
    IloCplex mycplex(myenv);
    mycplex.extract(mymodel);
    IloBool feasible = mycplex.solve(); // solves model and stores whether or ...

    // **************************************************#**************************************************
    // Printing the Solution
    if (feasible == IloFalse)
    {
        error(ILP_FEASIBLE);
    }
    _model.get_value(mycplex.getObjValue());

    // check solution is binary or not
#ifdef BINARY
    for (i = 0; i < nR; ++i)
    {
        for (j = 0; j < nC; ++j)
        {
            if (!is_binary(mycplex.getValue(s[i][j])))
            {
                error(NOT_BINARY);
            }
        }
    }

    for (i = 0; i < nR; ++i)
    {
        for (j = 0; j < nC; ++j)
        {
            for (k = 0; k < nK; ++k)
            {
                if (!is_binary(mycplex.getValue(f[i][j][k])))
                {
                    error(NOT_BINARY);
                }
            }
        }
    }
#endif

    // Print result
    vector<bool_vec> r(nR, bool_vec(nK, false));
    vector<bool_vec> c(nC, bool_vec(nK, false));
    for (i = 0; i < nR; ++i)
    {
        for (j = 0; j < nC; ++j)
        {
            if (mycplex.getValue(s[i][j]) > EPS && M[i][j] > 0)
            {
                for (k = 0; k < nK; ++k)
                {
                    if (mycplex.getValue(f[i][j][k]) > EPS)
                    {
                        r[i][k] = true;
                        c[j][k] = true;
                    }
                }
            }
        }
    }

    vector<Matrix> result(nK, Matrix(nR, nC));
    for (i = 0; i < nR; ++i)
    {
        for (j = 0; j < nC; ++j)
        {
            if (mycplex.getValue(s[i][j]) > EPS)
            {
                for (k = 0; k < nK; ++k)
                {
                    if (r[i][k] && c[j][k]) // mycplex.getValue(f[i][j][k]) can't be used
                    {
                        result[k][i][j] = 1; // Just for bool
                    }
                }
            }
        }
    }
    _model.get_result(result);

    // **************************************************#**************************************************
    // Closing the Model
    mycplex.clear();
    myenv.end();
}