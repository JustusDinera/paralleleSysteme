#include <math.h>
#include <stdio.h>
#include <random>
#include <pthread.h>

#define SEITE 1000
#define RAND_MAX 1000
#define THREADCOUNT 4

enum direction{X,Y,Z};

// Wuerfel Arrays
double wuerfel1[SEITE][SEITE][SEITE];
double wuerfel2[SEITE][SEITE][SEITE];

// Variables for threads
pthread_mutex_t mutex[THREADCOUNT]; // Mutexe fuer Threads
pthread_t threadIds[THREADCOUNT];   // 
int startValues[THREADCOUNT][SEITE][SEITE][SEITE];
int endValues[THREADCOUNT][SEITE][SEITE][SEITE];

void init_wuerfel(double cube[SEITE][SEITE][SEITE]){
    for (int i = 0; i < SEITE; i++)
    {
        for (int j = 0; j < SEITE; j++)
        {
            for (int k = 0; k < SEITE; k++)
            {
                cube[i][j][k] = rand();
            }
        }
    }
}

void setThreadValues(int * start[THREADCOUNT][SEITE][SEITE][SEITE], int * end[THREADCOUNT][SEITE][SEITE][SEITE], int direction, int startValue, int EndValue){
    int bias = 
    for (int i = 0; i < THREADCOUNT; i++)
    {
        if (direction == X)
        {
                        
        } 
        else if (direction == Y)
        {
            
        }
        else if (direction == Z)
        {
            /* code */
        }
        
    }
    
}

double berechnung(double value){
    double returnValue = 0;
    returnValue = sinf(value)*100;
    returnValue = cosf(returnValue)*100;
    return returnValue;
}

void trans_cube(
    double *start_cube[SEITE][SEITE][SEITE], 
    double *end_cube[SEITE][SEITE][SEITE], 
    int x_start, int x_stop,
    int y_start, int y_stop,
    int z_start, int z_stop)
    {
    for (int x = x_start; x < x_stop; x++)
    {
        for (int y = 0; y < y_stop; y++)
        {
            for (int z = 0; z < z_stop; z++)
            {
                *end_cube[x][y][z] = berechnung(*start_cube[x][y][z]);
            }
        }
    }
}

void createThreads(int num, void * funtion){
    for (int i = 0; i < THREADCOUNT; i++)
    {
        pthread_create(threadIds+i, NULL, (void *)trans_cube, )
    }
    
}

int main(int argc, char const *argv[])
{
    init_wuerfel(wuerfel1);
    return 0;
}

