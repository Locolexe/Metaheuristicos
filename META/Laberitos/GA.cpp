#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

const int SIZE = 15;
const int POP = 20;
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

struct Individual
{
    vector<int> route;
    int fitness;
};

int evaluate(vector<int> route)
{
    int x = 0;
    int y = 14;

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

bool compare(Individual a, Individual b)
{
    return a.fitness < b.fitness;
}

int main()
{
    ofstream file("ga_maze.csv");
    file << "iter,fitness\n";

    srand(time(0));

    vector<Individual> pop(POP);

    for(int i=0; i<POP; i++)
    {
        for(int j=0; j<MOVES; j++)
        {
            pop[i].route.push_back(rand()%4);
        }

        pop[i].fitness = evaluate(pop[i].route);
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

            for(int i=0; i<MOVES; i++)
            {
                if(rand()%2)
                    child.route.push_back(p1.route[i]);
                else
                    child.route.push_back(p2.route[i]);

                if(rand()%100 < 10)
                {
                    child.route[i] = rand()%4;
                }
            }

            child.fitness = evaluate(child.route);

            newPop.push_back(child);
        }

        pop = newPop;

        sort(pop.begin(), pop.end(), compare);

        cout << "Iteracion " << t
             << " Fitness: "
             << pop[0].fitness
             << endl;
        file << t << "," << pop[0].fitness << "\n";
    }
    file.close();
    return 0;
}