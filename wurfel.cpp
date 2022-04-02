#include <math.h>
#include <stdio.h>
#include <random>
#include <pthread.h>

#define SEITE 1000
#define RAND_MAX 1000
#define THREADCOUNT 1

enum direction{X,Y,Z};

/* 
    Variablen Deklaration 
*/
// Wuerfel Arrays
double wuerfel1[SEITE][SEITE][SEITE];
double wuerfel2[SEITE][SEITE][SEITE];

// Variables for threads
pthread_mutex_t mutex[THREADCOUNT]; // Mutexe fuer Threads
pthread_t threadIds[THREADCOUNT];   // 
int startValues[THREADCOUNT][SEITE][SEITE][SEITE];
int endValues[THREADCOUNT][SEITE][SEITE][SEITE];

/* 
    Funktionsdeklaration 
*/
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
        for (int y = y_start; y < y_stop; y++)
        {
            for (int z = z_start; z < z_stop; z++)
            {
                *end_cube[x][y][z] = berechnung(*start_cube[x][y][z]);
            }
        }
    }
}

void createThreads(int num, int direction){
    int start = 0;
    int stop = 0; 
    for (int i = 0; i < num; i++)
    {
        // set start value
        start = SEITE/num * i;

        // set stop value
        if ((1+i == num) && (SEITE%num > 0))
        {
            stop = SEITE/num * (i + 1) +1;
        }     
        else {
            stop = SEITE/num * (i + 1);
        }
        
        // create threads in depend of direction
        if (direction == X)
        {
            pthread_create(threadIds+i, NULL, (void *)trans_cube(&wuerfel1, &wuerfel2, start, stop, 0, SEITE, 0, SEITE), )
        }
        else if (direction == Y)
        {
            pthread_create(threadIds+i, NULL, (void *)trans_cube(&wuerfel1, &wuerfel2, 0, SEITE, start, stop, 0, SEITE), )
        }
        else if (direction == Z)
        {
            pthread_create(threadIds+i, NULL, (void *)trans_cube(&wuerfel1, &wuerfel2, 0, SEITE, 0, SEITE, start, stop), )
        }
    }
    
}

int main(int argc, char const *argv[])
{
    init_wuerfel(wuerfel1);
    createThreads(THREADCOUNT, X);   
    return 0;
}

