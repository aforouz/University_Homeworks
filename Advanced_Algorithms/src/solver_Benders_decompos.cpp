#include "Solver.hpp"

void solver_Benders_decomposition(Config& _config, Model& _model)
{
    size_t i, j, k, Sk;
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

    IloNumVarMatrix r(myenv, nR);
    for (i = 0; i < nR; ++i)
    {
        r[i] = IloNumVarArray(myenv, nK, 0, 1, ILOBOOL); // ILOBOOL  ILOFLOAT
    }

    IloNumVarMatrix c(myenv, nC);
    for (j = 0; j < nC; ++j)
    {
        c[j] = IloNumVarArray(myenv, nK, 0, 1, ILOBOOL); // ILOBOOL  ILOFLOAT
    }

    // **************************************************#**************************************************
    // SRC-LINK1
    for (i = 0; i < nR; ++i)
    {
        for (j = 0; j < nC; ++j)
        {
            if (M[i][j] < 0)
            {
                for (k = 0; k < nK; ++k)
                {
                    mymodel.add(s[i][j] + 1 >= r[i][k] + c[j][k]);
                }
            }
        }
    }

    // SRC-LINK2
    for (Sk = 0; Sk < (static_cast<unsigned long long>(1) << nK); ++Sk)
    {
        for (i = 0; i < nR; ++i)
        {
            for (j = 0; j < nC; ++j)
            {
                IloExpr myexpr(myenv);
                for (k = 0; k < nK; ++k)
                {
                    if ((Sk & (static_cast<unsigned long long>(1) << k)) != 0)
                    {
                        myexpr += r[i][k];
                    }
                    else
                    {
                        myexpr += c[j][k];
                    }
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

    vector<Matrix> result(nK, Matrix(nR, nC));
    for (i = 0; i < nR; ++i)
    {
        for (j = 0; j < nC; ++j)
        {
            if (mycplex.getValue(s[i][j]) > EPS)
            {
                for (k = 0; k < nK; ++k)
                {
                    if (mycplex.getValue(r[i][k]) > EPS && mycplex.getValue(c[j][k]) > EPS)
                    {
                        result[k][i][j] = 1;
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

void solver_Benders_decomposition_cut(Config& _config, Model& _model)
{
    size_t i, j, k, Sk;
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

    IloNumVarMatrix r(myenv, nR);
    for (i = 0; i < nR; ++i)
    {
        r[i] = IloNumVarArray(myenv, nK, 0, 1, ILOBOOL); // ILOBOOL  ILOFLOAT
    }

    IloNumVarMatrix c(myenv, nC);
    for (j = 0; j < nC; ++j)
    {
        c[j] = IloNumVarArray(myenv, nK, 0, 1, ILOBOOL); // ILOBOOL  ILOFLOAT
    }

    // **************************************************#**************************************************
    // SRC-LINK1
    for (i = 0; i < nR; ++i)
    {
        for (j = 0; j < nC; ++j)
        {
            if (M[i][j] < 0)
            {
                for (k = 0; k < nK; ++k)
                {
                    mymodel.add(s[i][j] + 1 >= r[i][k] + c[j][k]);
                }
            }
        }
    }

    // SRC-LINK2
    for (Sk = 0; Sk < (static_cast<unsigned long long>(1) << nK); ++Sk)
    {
        for (i = 0; i < nR; ++i)
        {
            for (j = 0; j < nC; ++j)
            {
                IloExpr myexpr(myenv);
                for (k = 0; k < nK; ++k)
                {
                    if ((Sk & (static_cast<unsigned long long>(1) << k)) != 0)
                    {
                        myexpr += r[i][k];
                    }
                    else
                    {
                        myexpr += c[j][k];
                    }
                }
                myexpr -= s[i][j];
                mymodel.add(IloRange(myenv, 0, myexpr, IloInfinity));
            }
        }
    }

    // NPOS
    for (i = 0; i < nR; ++i)
    {
        Sk = 0;
        for (j = 0; j < nC; ++j)
        {
            if (M[i][j] > 0)
            {
                ++Sk;
            }
        }

        IloExpr myexpr(myenv);
        for (k = 0; k < nK; ++k)
        {
            myexpr += r[i][k];
        }
        mymodel.add(IloRange(myenv, 0, myexpr, Sk));
    }

    for (j = 0; j < nC; ++j)
    {
        Sk = 0;
        for (i = 0; i < nR; ++i)
        {
            if (M[i][j] > 0)
            {
                ++Sk;
            }
        }

        IloExpr myexpr(myenv);
        for (k = 0; k < nK; ++k)
        {
            myexpr += c[j][k];
        }
        mymodel.add(IloRange(myenv, 0, myexpr, Sk));
    }

    // B-SYM
    for (k = 0; k < nK - 1; ++k)
    {
        for (i = 0; i < nR; ++i)
        {
            IloExpr myexpr(myenv);
            for (j = 0; j <= i; ++j)
            {
                myexpr += r[j][k];
            }
            myexpr -= r[i][k + 1];
            mymodel.add(IloRange(myenv, 0, myexpr, IloInfinity));
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
        for (k = 0; k < nK; ++k)
        {
            if (!is_binary(mycplex.getValue(r[i][k])))
            {
                error(NOT_BINARY);
            }
        }
    }

    for (j = 0; j < nC; ++j)
    {
        for (k = 0; k < nK; ++k)
        {
            if (!is_binary(mycplex.getValue(c[j][k])))
            {
                error(NOT_BINARY);
            }
        }
    }
#endif

    // Print result
    vector<Matrix> result(nK, Matrix(nR, nC));
    for (i = 0; i < nR; ++i)
    {
        for (j = 0; j < nC; ++j)
        {
            if (mycplex.getValue(s[i][j]) > EPS)
            {
                for (k = 0; k < nK; ++k)
                {
                    if (mycplex.getValue(r[i][k]) > EPS && mycplex.getValue(c[j][k]) > EPS)
                    {
                        result[k][i][j] = 1;
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
