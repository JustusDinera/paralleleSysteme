#include <math.h>
#include <stdio.h>
#include <random>
#include <pthread.h>
#include <chrono>

#define SEITE 1000
#define RAND_MAX 1000
#define THREADCOUNT 1

enum direction{X,Y,Z};

/* 
    Variablen Deklaration 
*/
// 
std::chrono::steady_clock::time_point allStart;
std::chrono::steady_clock::time_point allStop;


// Variables for threads
//pthread_mutex_t mutex[THREADCOUNT]; // Mutexe fuer Threads
pthread_t tId[THREADCOUNT];   // 
int startValues[THREADCOUNT][SEITE][SEITE][SEITE];
int endValues[THREADCOUNT][SEITE][SEITE][SEITE];

struct threadArgs
{
    double * wuerfel1[SEITE][SEITE][SEITE];
    double * wuerfel2[SEITE][SEITE][SEITE];
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

// Funktion der Berechnung
double berechnung(double value){
    double returnValue = 0;
    returnValue = sin(value)*100;
    returnValue = cos(returnValue)*100;
    return returnValue;
}

// Berechung des Wertes einer Zelle und speichern im anderen Wuerfel
void trans_cube(struct threadArgs args)
    {
        // Varibalendeklaration

    std::chrono::steady_clock::time_point thStart = std::chrono::steady_clock::now(); // nehmen der Startziet
    std::chrono::steady_clock::time_point thStop;

    for (int x = args.x_start; x < args.x_stop; x++)
    {
        for (int y = args.y_start; y < args.y_stop; y++)
        {
            for (int z = args.z_start; z < args.z_stop; z++)
            {
                *(args.wuerfel1)[x][y][z] = berechnung(*(args.wuerfel2)[x][y][z]);
            }
        }
    }

    // nehmen der Endzeit des threads
    thStop = std::chrono::steady_clock::now();
    printf("Der Thread %i hat %ius fuer die Berechnung benoetigt\n",args.number,std::chrono::duration_cast<std::chrono::microseconds>(thStop - thStart).count());
}

// Thread anlegen und Grenzen festlegen
void createThreads(int num, int direction){
    int start = 0;
    int stop = 0; 

    // Argumentvariable anlegen
    struct threadArgs *args = (struct threadArgs*)malloc(sizeof(struct threadArgs)*THREADCOUNT);

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
        
        // in depency of direction set thread args and start it 
        if (direction == X)
        {
            // set thread variable
            (args+i)->x_start = start;
            (args+i)->x_stop = stop;
            (args+i)->y_start = 0;
            (args+i)->y_stop = SEITE;
            (args+i)->z_start = 0;
            (args+i)->z_stop = SEITE;

            // start thread
            pthread_create((tId+i), NULL, (void *(*)(void*))trans_cube, (void *)(args + i));
        }
        else if (direction == Y)
        {
            // set thread variable
            (args+i)->x_start = 0;
            (args+i)->x_stop = SEITE;
            (args+i)->y_start = start;
            (args+i)->y_stop = stop;
            (args+i)->z_start = 0;
            (args+i)->z_stop = SEITE;

            // start thread
            pthread_create((tId+i), NULL, (void *(*)(void*))trans_cube, (void *)(args + i));
        }
        else if (direction == Z)
        {
            // set thread variable
            (args+i)->x_start = 0;
            (args+i)->x_stop = SEITE;
            (args+i)->y_start = 0;
            (args+i)->y_stop = SEITE;
            (args+i)->z_start = start;
            (args+i)->z_stop = stop;

            // start thread
            pthread_create((tId+i), NULL, (void *(*)(void*))trans_cube, (void *)(args + i));
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
    double wuerfel1[SEITE][SEITE][SEITE];
    double wuerfel2[SEITE][SEITE][SEITE];

    // Startzeitpunkt nehmen
    allStart = std::chrono::steady_clock::now();
    
    // Wuerfel inital fuellen
    init_wuerfel(wuerfel1);
    createThreads(THREADCOUNT, X);   
    waitForThreads(tId);

    // nehmen der Endzeit
    allStop = std::chrono::steady_clock::now();
    printf("Die Wuerfelberechnung hat %ius benoetigt",std::chrono::duration_cast<std::chrono::microseconds>(allStop - allStart).count());

    // Endzeitpunktnehmen
    return 0;
}

