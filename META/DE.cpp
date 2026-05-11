//Nota para el lecto: Este codigo se ejecutara 20 veces y se guardaran los resultados en "results_de.csv". Si solo quieres la informacion de una corrida, la utlima queda almacenada en "de.csv"
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
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

int main()
{
    random_device rd;
    mt19937 gen(rd());

    uniform_real_distribution<> dist(-5,5);
    uniform_real_distribution<> rand01(0,1);
    
    ofstream results("results_de.csv");
    results << "run,best\n";

    for(int run = 0; run < RUNS; run++)
    {

    vector<vector<double>> pop(POP, vector<double>(D));

    vector<double> fitness(POP);

    ofstream file("de.csv");

    file << "iter,fitness\n";

    for(int i=0; i<POP; i++)
    {
        for(int j=0; j<D; j++)
        {
            pop[i][j] = dist(gen);
        }

        fitness[i] = funcion(pop[i]);
    }

    double F = 0.8;
    double CR = 0.9;

    for(int t=0; t<ITER; t++)
    {
        for(int i=0; i<POP; i++)
        {
            int r1,r2,r3;

            do{r1=rand()%POP;}while(r1==i);
            do{r2=rand()%POP;}while(r2==i||r2==r1);
            do{r3=rand()%POP;}while(r3==i||r3==r2||r3==r1);

            vector<double> trial(D);

            for(int j=0; j<D; j++)
            {
                double mutant =
                    pop[r1][j]
                    + F*(pop[r2][j]-pop[r3][j]);

                if(rand01(gen)<CR)
                    trial[j]=mutant;
                else
                    trial[j]=pop[i][j];
            }

            double trialFit = funcion(trial);

            if(trialFit < fitness[i])
            {
                pop[i] = trial;
                fitness[i] = trialFit;
            }
        }

        double best = fitness[0];

        for(double f : fitness)
        {
            if(f < best)
                best = f;
        }

        cout << t << " " << best << endl;

        file << t << "," << best << "\n";
    }

    double best = fitness[0];

    for(double f : fitness)
    {
        if(f < best)
            best = f;
    }
    results << run << "," << best << "\n";
    cout << "Run " << run
     << " Best: "
     << best
     << endl;
    file.close();
    }
    results.close();
    return 0;
}
//PSDT: No lo corrigo porque al final cumple con generar la informacion de 1 resultado y 20. Asi que jaja