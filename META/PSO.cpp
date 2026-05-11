//Nota para el lecto: Este codigo se ejecutara 20 veces y se guardaran los resultados en "results_pso.csv". Si solo quieres la informacion de una corrida, la utlima queda almacenada en "pso.csv"
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <fstream>

using namespace std;

const int D = 5;
const int PARTICLES = 30;
const int ITER = 500;
const int RUNS = 20;

/*
double funcion(vector<double> x)
{
    // ACKLEY FUNCTION

    double sum1 = 0;
    double sum2 = 0;

    for(int i=0; i<D; i++)
    {
        sum1 += x[i]*x[i];
        sum2 += cos(2*M_PI*x[i]);
    }

    return -20*exp(-0.2*sqrt(sum1/D))
           -exp(sum2/D)
           +20
           +exp(1);
}
*/
double funcion(vector<double> x)
{
    // Funcion LEVY
    vector<double> w(D);

    for(int i=0; i<D; i++)
    {
        w[i] = 1 + (x[i]-1)/4.0;
    }

    double term1 = pow(sin(M_PI*w[0]),2);

    double sum = 0;

    for(int i=0; i<D-1; i++)
    {
        sum += pow(w[i]-1,2)
               * (1 + 10*pow(sin(M_PI*w[i]+1),2));
    }

    double term3 =
        pow(w[D-1]-1,2)
        * (1 + pow(sin(2*M_PI*w[D-1]),2));

    return term1 + sum + term3;
}

int main()
{
    ofstream results("results_pso.csv");

    results << "run,best\n";
    for(int run=0; run<RUNS; run++)
    {

    random_device rd;
    mt19937 gen(rd());

    uniform_real_distribution<> dist(-5,5);
    uniform_real_distribution<> rand01(0,1);

    vector<vector<double>> pos(PARTICLES, vector<double>(D));
    vector<vector<double>> vel(PARTICLES, vector<double>(D));

    vector<vector<double>> bestPos(PARTICLES, vector<double>(D));

    vector<double> bestFit(PARTICLES);

    vector<double> globalBest(D);

    double globalFit = 999999;

    ofstream file("pso.csv");

    file << "iter,fitness\n";

    for(int i=0; i<PARTICLES; i++)
    {
        for(int j=0; j<D; j++)
        {
            pos[i][j] = dist(gen);
            vel[i][j] = dist(gen)*0.1;
        }

        bestPos[i] = pos[i];

        bestFit[i] = funcion(pos[i]);

        if(bestFit[i] < globalFit)
        {
            globalFit = bestFit[i];
            globalBest = pos[i];
        }
    }

    double w = 0.7;
    double c1 = 1.5;
    double c2 = 1.5;

    for(int t=0; t<ITER; t++)
    {
        for(int i=0; i<PARTICLES; i++)
        {
            for(int j=0; j<D; j++)
            {
                double r1 = rand01(gen);
                double r2 = rand01(gen);

                vel[i][j] =
                    w*vel[i][j]
                    + c1*r1*(bestPos[i][j]-pos[i][j])
                    + c2*r2*(globalBest[j]-pos[i][j]);

                pos[i][j] += vel[i][j];
            }

            double fit = funcion(pos[i]);

            if(fit < bestFit[i])
            {
                bestFit[i] = fit;
                bestPos[i] = pos[i];
            }

            if(fit < globalFit)
            {
                globalFit = fit;
                globalBest = pos[i];
            }
        }

        cout << t << " " << globalFit << endl;

        file << t << "," << globalFit << "\n";
        results << run << "," << globalFit << "\n";
    }

    file.close();
    }
    results.close();
return 0;}