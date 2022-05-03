#include <math.h>
#include <stdio.h>
#include <random>
#include <pthread.h>
#include <chrono>

#define SEITE 10
#define RANDMAX 1000
#define THREADCOUNT 4
#define CYCLE 2

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
pthread_barrier_t wait;
pthread_t tId[THREADCOUNT];   // 
int startValues[THREADCOUNT][SEITE][SEITE][SEITE];
int endValues[THREADCOUNT][SEITE][SEITE][SEITE];

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
    for (int i = 0; i < 2000; i++)
    {
        returnValue = sin(returnValue)*100;
        returnValue = cos(returnValue)*100;
    }
    return returnValue;
}

// Berechung des Wertes einer Zelle und speichern im anderen Wuerfel
void trans_cube(void * args)
{
    // Varibalendeklaration
    struct threadArgs * stArgs = (struct threadArgs *)args;
    std::chrono::steady_clock::time_point thStart = std::chrono::steady_clock::now(); // nehmen der Startziet
    std::chrono::steady_clock::time_point thCycle = thStart;
    std::chrono::steady_clock::time_point thStop;

    for (int i = 0; i < CYCLE; i++)
    {
        for (int x = stArgs->x_start; x < stArgs->x_stop; x++)
        {
            for (int y = stArgs->y_start; y < stArgs->y_stop; y++)
            {
                for (int z = stArgs->z_start; z < stArgs->z_stop; z++)
                {
                    if (i%2 == 0)
                    {
                        (* stArgs->wuerfel2)[x][y][z] = berechnung((*stArgs->wuerfel1)[x][y][z]);
                    }
                    else 
                    {
                        (* stArgs->wuerfel1)[x][y][z] = berechnung((*stArgs->wuerfel2)[x][y][z]);
                    }
                }
            }
        }
        
        thStop = std::chrono::steady_clock::now();
        printf("Der Thread %i hat %ius fuer die Berechnung des %i. Wuerfel benoetigt\n",stArgs->number,std::chrono::duration_cast<std::chrono::microseconds>(thStop - thCycle).count(), (i+1));
        thCycle = thStop;

        // threads synchronisieren ausser beim letzten Durchlauf
        if (i < CYCLE - 1)
        {
            pthread_barrier_wait(&wait);
        }
    }

    // nehmen der Endzeit des threads
    thStop = std::chrono::steady_clock::now();
    printf("Der Thread %i hat %ius fuer die Berechnung des gesamten Threads benoetigt\n",stArgs->number,std::chrono::duration_cast<std::chrono::microseconds>(thStop - thStart).count());

}

// Thread anlegen und Grenzen festlegen
void createThreads(int num, int direction, WURFEL * wuerfel1, WURFEL * wuerfel2){
    int start = 0;
    int stop = 0; 

    // Argumentvariable anlegen
    struct threadArgs *args = (struct threadArgs*)malloc(sizeof(struct threadArgs)*THREADCOUNT);

    for (int i = 0; i < num; i++)
    {
        // set cube adresses 
        (args+i)->wuerfel1 = wuerfel1;
        (args+i)->wuerfel2 = wuerfel2;
        // set thread number
        (args+i)->number = i;
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
    WURFEL wuerfel1;
    WURFEL wuerfel2;

    // initialisiere Barriere
    pthread_barrier_init(&wait,NULL,4);

    // Startzeitpunkt nehmen
    allStart = std::chrono::steady_clock::now();
    
    // Wuerfel inital fuellen
    init_wuerfel(&wuerfel1);
    createThreads(THREADCOUNT, X, &wuerfel1, &wuerfel2);   
    waitForThreads(tId);

    // nehmen der Endzeit
    allStop = std::chrono::steady_clock::now();
    printf("Die Wuerfelberechnung hat %ius benoetigt",std::chrono::duration_cast<std::chrono::microseconds>(allStop - allStart).count());

    // Endzeitpunktnehmen
    return 0;
}

