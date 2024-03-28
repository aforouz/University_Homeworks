#include "Solver.hpp"

void solver_ILP_IPCQ(Config& _config, Model& _model)
{
    size_t i, j, k;
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
    // SRCQ-LINK1
    for (i = 0; i < nR; ++i)
    {
        for (j = 0; j < nC; ++j)
        {
            IloExpr myexpr(myenv);
            for (k = 0; k < nK; ++k)
            {
                myexpr += r[i][k]*c[j][k];
            }
            myexpr = (long long int)nK*s[i][j] - myexpr;
            mymodel.add(IloRange(myenv, 0, myexpr, IloInfinity));

        }
    }

    // SRCQ-LINK2
    for (i = 0; i < nR; ++i)
    {
        for (j = 0; j < nC; ++j)
        {
            if (M[i][j] > 0)
            {
                IloExpr myexpr(myenv);
                for (k = 0; k < nK; ++k)
                {
                    myexpr += r[i][k] * c[j][k];
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
