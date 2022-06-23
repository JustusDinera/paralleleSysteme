#include <stdio.h>
#include <fstream>
#include <iomanip>
#include <thread>
#include <atomic>
#include <math.h>
#include <random>

#define MAX_BUFFER 10
#define ITERATION 10
#define ARRAY_SIZE 1000
#define MAX_DELAY 5

/* class declaration */
class FIFO
{
private:
    float buffer[MAX_BUFFER];
    std::atomic<uint> counter;
    uint head;
    uint tail;
public:
    FIFO();
    ~FIFO();
    bool isFull();
    bool isEmpty();
    void pushItem(float item);
    float popItem();
};

FIFO::FIFO()
{
    counter = 0;
    head = 0;
    tail = 0;
}

FIFO::~FIFO()
{
}

bool FIFO::isFull()
{
    if (counter == MAX_BUFFER)
    {
        return true;
    }
    else
    {
        return false;
    }    
}

bool FIFO::isEmpty()
{
    if (counter == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void FIFO::pushItem(float item)
{
    if (!isFull())
    {
        buffer[head] = item;

        head++;
        // handle an overflow
        if (head == MAX_BUFFER)
        {
            head = 0;
        }

        counter++;
    }
}

float FIFO::popItem()
{
    float retVal;
    
    retVal = buffer[tail];

    tail++;
    //handle an overflow
    if (tail == MAX_BUFFER)
    {
        tail = 0;
    }
    
    counter--;
    return retVal;    
}

/* Variable declaration */
FIFO fifo0;
FIFO fifo1;
FIFO fifo2;
float startValues[ARRAY_SIZE];
float finalValues[ARRAY_SIZE];

/* function declaration */
float calculation1(float value)
{
    float retVal = 0;

    for (int i = 0; i < ITERATION; i++)
    {
        retVal = sin(value) * 100;
    }
    
    return retVal;
}

float calculation2(float value)
{
    float retVal = 0;

    for (int i = 0; i < ITERATION; i++)
    {
        retVal = sin(value) * cos(value) * 100;
    }
    
    return retVal;
}

float calculation3(float value)
{
    float retVal = 0;

    for (int i = 0; i < ITERATION; i++)
    {
        retVal = cos(value) * 100;
    }
    
    return retVal;
}

// Thread functions
void stageFirst()
{
    auto startTime = std::chrono::steady_clock::now();
    
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        // wait in case fo a full output buffer
        while (fifo0.isFull());        
        // push value to the output buffer
        fifo0.pushItem(calculation1(startValues[i]));

        if (i < ARRAY_SIZE -1)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(rand()%MAX_DELAY));
        }
        
    }

    printf("Thread 1 is done with a duration of %ius!\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - startTime).count());
}

void stageMiddle1()
{
    float retVal;
    auto startTime = std::chrono::steady_clock::now();
    
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        // wait in case of empty input buffer
        while (fifo0.isEmpty());
        // pop out the current value
        retVal = calculation2(fifo0.popItem());

        // wait in case fo a full output buffer
        while (fifo1.isFull());
        // push value to the output buffer
        fifo1.pushItem(retVal);

        if (i < ARRAY_SIZE -1)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(rand()%MAX_DELAY));
        }
    }

    printf("Thread 2 is done with a duration of %ius!\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - startTime).count());
}

void stageMiddle2()
{
    float retVal;
    auto startTime = std::chrono::steady_clock::now();
    
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        // wait in case of empty input buffer
        while (fifo1.isEmpty());
        retVal = calculation3(fifo1.popItem());

        // wait in case fo a full output buffer
        while (fifo2.isFull());
        // push value to the output buffer
        fifo2.pushItem(retVal);
        
        if (i < ARRAY_SIZE -1)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(rand()%MAX_DELAY));
        }
    }

    printf("Thread 3 is done with a duration of %ius!\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - startTime).count());
}

void stageLast()
{
    auto startTime = std::chrono::steady_clock::now();

    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        // wait in case of empty buffer
        while (fifo2.isEmpty());
        // write final values in the output array
        finalValues[i] = calculation1(fifo2.popItem());

        // delay the thread to provocate runtime issues
        if (i < ARRAY_SIZE -1)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(rand()%MAX_DELAY));
        }
    }

    printf("Thread 4 is done with a duration of %ius!\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - startTime).count());
}

int main(int argc, char const *argv[])
{
    std::ofstream outputFile;
    outputFile.open("Arrays.txt");

    outputFile << "--- Input Array ---" << std::endl;

    // initial fill the start array and print the array to file
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        startValues[i] = (float)rand();

        outputFile << startValues[i] << '\t';
        if ((i%5 == 0)&&(i != 0))
            outputFile << std::endl;        
    }
    


    // Create threads
    std::thread stateOne(stageFirst);
    std::thread stateTwo(stageMiddle1);
    std::thread stateThree(stageMiddle2);
    std::thread stateFour(stageLast);

    // Wait for all threads
    stateOne.join();
    stateTwo.join();
    stateThree.join();
    stateFour.join();

    outputFile << "\n--- Output Array ---" << std::endl;
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        outputFile << std::setprecision(5) << finalValues[i] << '\t';
        if ((i%5 == 0)&&(i != 0))
            outputFile << std::endl;        
    }

    printf("All threads done!");

    return 0;
}
