#include <sstream>
#include <iostream>
#include <ilcplex/ilocplex.h>

#include "facilities_file.hpp"
#include "facilities_problem.hpp"

ILOSTLBEGIN
int main()
{
    FacilitiesFile file("instance.fac");
    FacilitiesProblem facilities_problem = file.read_file();

    IloEnv env;          // declara Variável de ambiente do CPLEX
    IloModel model(env); // declara Variavel do modelo do CPLEX (que existe dentro do ambiente criado)

    IloBoolVarArray y(env); // declara Vetor de variáveis numéricas (que existe dentro do ambiente criado)
                            // Vetor inicialmente vazio

    IloArray<IloBoolVarArray> x(env, facilities_problem.number_of_facilities);

    for (int i = 0; i < facilities_problem.number_of_facilities; i++)
    {
        std::ostringstream oss;
        oss << "y_" << i + 1;

        y.add(IloBoolVar(env, oss.str().c_str()));
    }

    for (int i = 0; i < facilities_problem.number_of_facilities; i++)
    {
        x[i] = IloBoolVarArray(env, facilities_problem.number_of_clients);

        for (int j = 0; j < facilities_problem.number_of_clients; j++)
        {
            std::ostringstream oss;
            oss << "x_" << i + 1 << "_" << j + 1;

            x[i][j] = IloBoolVar(env, oss.str().c_str());
            model.add(x[i][j]);
        }
    }

    // Todo cliente deve ser atendido
    IloConstraintArray client_constraints(env);
    for (int j = 0; j < facilities_problem.number_of_clients; j++)
    {
        IloExpr constraint(env);

        for (int i = 0; i < facilities_problem.number_of_facilities; i++)
        {
            constraint += x[i][j];
        }

        client_constraints.add(constraint == 1);
        constraint.end();
    }

    // Restrições disjuntas
    IloConstraintArray disjoint_constraints(env);
    for (Edge edge : facilities_problem.edges)
    {
        int i = edge.first;
        int j = edge.second;

        disjoint_constraints.add(x[i][j] <= y[i]);
    }

    // Capacidade
    IloConstraintArray capacity_constraints(env);
    for (int i = 0; i < facilities_problem.number_of_facilities; i++)
    {
        IloExpr constraint(env);

        for (auto MatrixEntry : facilities_problem.capacity_usages)
        {
            Edge edge = MatrixEntry.first;
            int capacity_usage = MatrixEntry.second;

            if (edge.first == i)
            {
                int j = edge.second;
                constraint += capacity_usage * x[i][j];
            }
        }

        client_constraints.add(constraint <= facilities_problem.capacity * y[i]);
        constraint.end();
    }

    // Derruba variáveis sobressalentes
    IloConstraintArray unused_variables_constraint(env);
    for (int i = 0; i < facilities_problem.number_of_facilities; i++)
    {
        for (int j = 0; j < facilities_problem.number_of_clients; j++)
        {
            if (facilities_problem.capacity_usages.find({i, j}) == facilities_problem.capacity_usages.end()) {
                unused_variables_constraint.add(x[i][j] == 0);
            }
        }
    }

    model.add(client_constraints);
    model.add(disjoint_constraints);
    model.add(capacity_constraints);
    model.add(unused_variables_constraint);

    // Função Objetivo
    IloExpr fo(env);
    for (int i = 0; i < facilities_problem.number_of_facilities; i++)
    {
        fo += facilities_problem.opening_cost * y[i];
    }

    for (Edge edge : facilities_problem.edges)
    {
        int i = edge.first;
        int j = edge.second;

        fo += facilities_problem.transfer_costs[{i, j}] * x[i][j];
    }

    model.add(IloMinimize(env, fo, "FO"));

    IloCplex solver(model); // declara variável "solver" sobre o modelo a ser solucionado
    solver.exportModel("model.lp");
    solver.solve(); // chama o "solver"

    cout << "Max=" << solver.getObjValue() << endl;    //  imprime solução do problema
    cout << "LB=" << solver.getBestObjValue() << endl; //  imprime solução do problema
    env.end();

    std::cout << "OK!";
}
