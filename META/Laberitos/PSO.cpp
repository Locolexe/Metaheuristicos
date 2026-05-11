#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

const int SIZE = 15;
const int PARTICLES = 20;
const int ITER = 200;
const int MOVES = 75;

vector<vector<int>> maze =
{
{0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
{0,1,1,1,0,1,0,1,0,1,0,1,0,1,1},
{0,0,0,0,0,1,0,1,0,1,0,1,0,0,0},
{0,1,1,1,1,1,1,1,1,1,0,1,1,1,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
{0,1,1,1,1,1,0,1,1,1,0,1,0,1,0},
{0,0,0,0,0,0,0,0,0,0,0,1,0,1,0},
{0,1,1,1,0,1,1,1,1,1,0,1,1,1,1},
{0,1,0,1,0,0,0,0,0,0,0,0,0,1,0},
{0,1,0,1,1,1,1,1,0,1,1,1,0,1,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,1,0,1,0,1,1,1,0,1,1,1,0,1,0},
{0,0,0,1,0,0,0,0,0,0,0,0,0,1,0},
{0,1,1,1,0,1,1,1,0,1,0,1,1,1,1},
{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0}
};

struct Particle
{
    vector<int> route;
    vector<int> bestRoute;

    int fitness;
    int bestFitness;
};

int evaluate(vector<int> route)
{
    int x = 0;
    int y = 0;

    int penalty = 0;

    for(int m : route)
    {
        int nx = x;
        int ny = y;

        if(m == 0) nx--;
        if(m == 1) nx++;
        if(m == 2) ny--;
        if(m == 3) ny++;

        if(nx >= 0 && nx < SIZE && ny >= 0 && ny < SIZE
           && maze[nx][ny] == 0)
        {
            x = nx;
            y = ny;
        }
        else
        {
            penalty += 10;
        }
    }

    int distance = abs(14 - x) + abs(0 - y);

    return distance + penalty + route.size();
}

int main()
{
    ofstream file("pso_maze.csv");
    file << "iter,fitness\n";

    srand(time(0));

    vector<Particle> swarm(PARTICLES);

    vector<int> globalBest;
    int globalFitness = 999999;

    for(int i=0; i<PARTICLES; i++)
    {
        for(int j=0; j<MOVES; j++)
        {
            swarm[i].route.push_back(rand()%4);
        }

        swarm[i].fitness = evaluate(swarm[i].route);

        swarm[i].bestRoute = swarm[i].route;
        swarm[i].bestFitness = swarm[i].fitness;

        if(swarm[i].fitness < globalFitness)
        {
            globalFitness = swarm[i].fitness;
            globalBest = swarm[i].route;
        }
    }

    for(int t=0; t<ITER; t++)
    {
        for(int i=0; i<PARTICLES; i++)
        {
            for(int j=0; j<MOVES; j++)
            {
                if(rand()%100 < 40)
                {
                    swarm[i].route[j] = globalBest[j];
                }

                if(rand()%100 < 10)
                {
                    swarm[i].route[j] = rand()%4;
                }
            }

            swarm[i].fitness = evaluate(swarm[i].route);

            if(swarm[i].fitness < swarm[i].bestFitness)
            {
                swarm[i].bestFitness = swarm[i].fitness;
                swarm[i].bestRoute = swarm[i].route;
            }

            if(swarm[i].fitness < globalFitness)
            {
                globalFitness = swarm[i].fitness;
                globalBest = swarm[i].route;
            }
        }

        cout << "Iteracion " << t
             << " Fitness: "
             << globalFitness
             << endl;
        file << t << "," << globalFitness << "\n";
    }
    file.close();
    return 0;
}