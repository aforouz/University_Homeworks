#include "Main.hpp"

void sample_1()
{
    // Model Definition
    IloEnv myenv; // environment object
    IloModel mymodel(myenv); // model object
    IloNumVar x(myenv, 0, IloInfinity, ILOFLOAT); // variable x on [0,infinity)
    IloNumVar y(myenv, 0, IloInfinity, ILOFLOAT); // variable y on [0,infinity)
    mymodel.add(3 * x + y <= 3); // constraint x+y <= 200
    mymodel.add(x + 3 * y <= 3); // constraint 5x + 15y <= 400
    mymodel.add(IloMaximize(myenv, x + y)); // objective max 5x + 15y
    // Model Solution
    IloCplex mycplex(myenv);
    mycplex.extract(mymodel);
    IloBool feasible = mycplex.solve(); // solves model and stores whether or ...
    // not it is feasible in an IloBool variable called "feasible"

    // Printing the Solution
    if (feasible == IloTrue)
    {
        cout << endl << "Problem feasible." << endl;
        cout << "x = " << mycplex.getValue(x) << endl; // value of x
        cout << "y = " << mycplex.getValue(y) << endl; // value of y
        cout << "cost = " << mycplex.getObjValue() << endl; // objective
    }
    else
        cout << endl << "Problem infeasible." << endl;
    // Closing the Model
    mycplex.clear();
    myenv.end();
    // wait for user to press a key, otherwise the readout will immediately ...
    // disappear from the command line
}

void sample_2()
{
    Matrix A(2, 2);
    A[0][0] = 3;
    A[0][1] = 1;
    A[1][0] = 1;
    A[1][1] = 3;
    real_vec b(2, 3);
    real_vec c(2, 1);

    // Model Definition
    IloEnv myenv; // environment object
    IloModel mymodel(myenv); // model object
    IloNumVarArray x(myenv, 2, 0, IloInfinity, ILOFLOAT);
    for (size_t i(0); i < 2; ++i)
    {
        IloExpr myexpr(myenv);
        for (size_t j(0); j < 2; ++j)
        {
            myexpr += A[i][j] * x[j];
        }
        mymodel.add(IloRange(myenv, 0, myexpr, b[i]));
    }
    IloExpr myexpr(myenv);
    for (size_t i(0); i < 2; ++i)
    {
        myexpr += c[i] * x[i];
    }
    mymodel.add(IloMaximize(myenv, myexpr)); // objective max 5x + 15y

    // Model Solution
    IloCplex mycplex(myenv);
    mycplex.extract(mymodel);
    IloBool feasible = mycplex.solve(); // solves model and stores whether or ...
    // not it is feasible in an IloBool variable called "feasible"

    // Printing the Solution
    if (feasible == IloTrue)
    {
        cout << endl << "Problem feasible." << endl;
        cout << "x = " << mycplex.getValue(x[0]) << endl; // value of x
        cout << "y = " << mycplex.getValue(x[1]) << endl; // value of y
        cout << "cost = " << mycplex.getObjValue() << endl; // objective
    }
    else
        cout << endl << "Problem infeasible." << endl;
    // Closing the Model
    mycplex.clear();
    myenv.end();
    // wait for user to press a key, otherwise the readout will immediately ...
    // disappear from the command line
}