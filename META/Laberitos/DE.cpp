#include <iostream>
#include <vector>
#include <cmath>
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
    ofstream file("de_maze.csv");
    file << "iter,fitness\n";

    srand(time(0));

    vector<vector<int>> pop(POP, vector<int>(MOVES));
    vector<int> fitness(POP);

    for(int i=0; i<POP; i++)
    {
        for(int j=0; j<MOVES; j++)
        {
            pop[i][j] = rand()%4;
        }

        fitness[i] = evaluate(pop[i]);
    }

    for(int t=0; t<ITER; t++)
    {
        for(int i=0; i<POP; i++)
        {
            int r1 = rand()%POP;
            int r2 = rand()%POP;
            int r3 = rand()%POP;

            vector<int> trial(MOVES);

            for(int j=0; j<MOVES; j++)
            {
                int value = pop[r1][j]
                            + (pop[r2][j] - pop[r3][j]);

                value = abs(value)%4;

                if(rand()%100 < 80)
                    trial[j] = value;
                else
                    trial[j] = pop[i][j];
            }

            int trialFitness = evaluate(trial);

            if(trialFitness < fitness[i])
            {
                pop[i] = trial;
                fitness[i] = trialFitness;
            }
        }

        int best = fitness[0];

        for(int f : fitness)
        {
            if(f < best)
                best = f;
        }

        cout << "Iteracion " << t
             << " Fitness: "
             << best
             << endl;
        file << t << "," << best << "\n";
    }
    file.close();
    return 0;
}