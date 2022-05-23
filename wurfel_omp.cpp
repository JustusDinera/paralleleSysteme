#include <math.h>
#include <stdio.h>
#include <random>
#include <omp.h>
#include <chrono>

#define SEITE 100
#define RANDMAX 1000
#define THREADCOUNT 4
#define DIRECTION X
#define COUNTS 2

enum direction{X,Y,Z};

typedef float LINIE[SEITE];
typedef LINIE FLACHE[SEITE];
typedef FLACHE WURFEL[SEITE];

/* 
    Variablen Deklaration 
*/
// 
std::chrono::steady_clock::time_point allStart;
std::chrono::steady_clock::time_point allStop;


// Variables for threads
//pthread_mutex_t mutex[THREADCOUNT]; // Mutexe fuer Threads
pthread_t tId[THREADCOUNT];   // 

struct threadArgs
{
    WURFEL *wuerfel1;
    WURFEL *wuerfel2;
    int x_start; 
    int x_stop;
    int y_start; 
    int y_stop;
    int z_start; 
    int z_stop;
    int number;
};


/* 
    Funktionsdeklaration 
*/
void init_wuerfel(WURFEL * cube){
    for (int i = 0; i < SEITE; i++)
    {
        for (int j = 0; j < SEITE; j++)
        {
            for (int k = 0; k < SEITE; k++)
            {
                (*cube)[i][j][k] = rand()%RANDMAX;
            }
        }
    }
}

// Funktion der Berechnung
float berechnung(float value){
    float returnValue = value;
    for (int i = 0; i < 200; i++)
    {
        returnValue = sin(returnValue)*100;
        returnValue = cos(returnValue)*100;
    }
    return returnValue;
}

// Initial Startparameter
void initParams(threadArgs *args){

        // Start und Stop Varibale
    int start = 0;
    int stop = 0;


    for (int i = 0; i < omp_get_num_threads(); i++)
    {
        // set thread number
        args[i].number = i;
        // set start value
        start = SEITE/omp_get_num_threads() * i;

        // set stop value
        if ((1+i == omp_get_num_threads()) && (SEITE%omp_get_num_threads() > 0))
        {
            stop = SEITE/omp_get_num_threads() * (i + 1) +1;
        }     
        else {
            stop = SEITE/omp_get_num_threads() * (i + 1);
        }
    
        // in depency of direction set thread args and start it 
        if (DIRECTION == X)
        {
            // set thread variable
            args[i].x_start = start;
            args[i].x_stop = stop;
            args[i].y_start = 0;
            args[i].y_stop = SEITE;
            args[i].z_start = 0;
            args[i].z_stop = SEITE;

            // start thread
            //pthread_create((tId+i), NULL, (void *(*)(void*))trans_cube, (void *)(args + i));
        }
        else if (DIRECTION == Y)
        {
            // set thread variable
            args[i].x_start = 0;
            args[i].x_stop = SEITE;
            args[i].y_start = start;
            args[i].y_stop = stop;
            args[i].z_start = 0;
            args[i].z_stop = SEITE;

            // start thread
            //pthread_create((tId+i), NULL, (void *(*)(void*))trans_cube, (void *)(args + i));
        }
        else if (DIRECTION == Z)
        {
            // set thread variable
            args[i].x_start = 0;
            args[i].x_stop = SEITE;
            args[i].y_start = 0;
            args[i].y_stop = SEITE;
            args[i].z_start = start;
            args[i].z_stop = stop;

            // start thread
            //pthread_create((tId+i), NULL, (void *(*)(void*))trans_cube, (void *)(args + i));
        }
    }
}


void waitForThreads(pthread_t * treadId){
    for (int i = 0; i < THREADCOUNT; i++)
    {
        pthread_join(tId[i], NULL);
    }
}

int main(int argc, char const *argv[])
{
    //Wuerfel Arrays
    WURFEL wuerfel1;
    WURFEL wuerfel2;

    // Startzeitpunkt nehmen
    allStart = std::chrono::steady_clock::now();
    

    // Zeitvatiable
    std::chrono::steady_clock::time_point thStart[omp_get_max_threads()]; // nehmen der Startziet
    std::chrono::steady_clock::time_point thStop[omp_get_max_threads()];



    // Wuerfel inital fuellen
    init_wuerfel(&wuerfel1);

    // 
    

    allStart = std::chrono::steady_clock::now();

    //struct threadArgs * stArgs = (struct threadArgs *)args;
    for (int i = 0; i < COUNTS; i++)
    {
#pragma omp parallel
{
    thStart[i] = std::chrono::steady_clock::now();
    for (int x = 0; x < SEITE; x++)
    {
        for (int y = 0; y < SEITE; y++)
        {
            for (int z = 0; z < SEITE; z++)
            {
                if (i%2 == 0)
                {
                    wuerfel2[x][y][z] = berechnung(wuerfel1[x][y][z]);
                }
                else
                {
                    wuerfel1[x][y][z] = berechnung(wuerfel2[x][y][z]);
                }
            }
        }
    }
       
    // nehmen der Endzeit des threads
    thStop[i] = std::chrono::steady_clock::now();
    printf("Der Thread %i hat %ius fuer die Berechnung von Wuerfel %i benoetigt\n",omp_get_thread_num(),std::chrono::duration_cast<std::chrono::microseconds>(thStop[i] - thStart[i]).count(), i);
    #pragma omp barrier
    #pragma omp single
    {
    }
}
    printf("---  Wurfel %i fertig berechnet  ---\n", i);
    }
    // nehmen der Endzeit
    allStop = std::chrono::steady_clock::now();
    printf("Die Wuerfelberechnung hat %ius benoetigt",std::chrono::duration_cast<std::chrono::microseconds>(allStop - allStart).count());

    // Endzeitpunktnehmen
    return 0;
}

