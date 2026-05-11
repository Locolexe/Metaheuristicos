//Nota para el lecto: Este codigo se ejecutara 20 veces y se guardaran los resultados en "results_ga.csv". Si solo quieres la informacion de una corrida, la utlima queda almacenada en "ga.csv"
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <fstream>

using namespace std;

const int D = 5;
const int POP = 30;
const int ITER = 500;
const int RUNS = 20;
/*
double funcion(vector<double> x)
{
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

struct Individual
{
    vector<double> x;
    double fitness;
};

bool compare(Individual a, Individual b)
{
    return a.fitness < b.fitness;
}

int main()
{
    

    random_device rd;
    mt19937 gen(rd());

    uniform_real_distribution<> dist(-5,5);
    uniform_real_distribution<> rand01(0,1);
    
    vector<Individual> pop(POP);

    ofstream results("results_ga.csv");

    results << "run,best\n";
    for(int run=0; run<RUNS; run++)
    {
    ofstream file("ga.csv");

    file << "iter,fitness\n";

    for(int i=0; i<POP; i++)
    {
        pop[i].x.resize(D);

        for(int j=0; j<D; j++)
        {
            pop[i].x[j] = dist(gen);
        }

        pop[i].fitness = funcion(pop[i].x);
    }

    for(int t=0; t<ITER; t++)
    {
        sort(pop.begin(), pop.end(), compare);

        vector<Individual> newPop;

        newPop.push_back(pop[0]);
        newPop.push_back(pop[1]);

        while(newPop.size() < POP)
        {
            Individual p1 = pop[rand()%10];
            Individual p2 = pop[rand()%10];

            Individual child;

            child.x.resize(D);

            for(int j=0; j<D; j++)
            {
                if(rand01(gen) < 0.5)
                    child.x[j] = p1.x[j];
                else
                    child.x[j] = p2.x[j];

                if(rand01(gen) < 0.1)
                {
                    child.x[j] += dist(gen)*0.1;
                }
            }

            child.fitness = funcion(child.x);

            newPop.push_back(child);
        }

        pop = newPop;

        sort(pop.begin(), pop.end(), compare);

        cout << t << " " << pop[0].fitness << endl;

        file << t << "," << pop[0].fitness << "\n";
    }
    results << run << "," << pop[0].fitness << "\n";
    cout << "Run " << run
     << " Best: "
     << pop[0].fitness
     << endl;
    file.close();
    }
    results.close();
return 0;
}