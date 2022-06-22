#include <stdio.h>
#include <thread>
#include <atomic>
#include <math.h>
#include <vector>
#include <random>

#define MAX_BUFFER 10
#define ITERATION 10
#define ARRAY_SIZE 1000

class FIFO
{
private:
    float buffer[MAX_BUFFER];
    uint counter = 0;
    uint head = 0;
    uint tail = 0;
public:
    FIFO(/* args */);
    ~FIFO();
    bool isFull();
    bool isEmpty();
    void pushItem(float item);
    float popItem();
};

FIFO::FIFO(/* args */)
{
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
        counter++;
        
        buffer[head] = item;

        head++;
        if (head == MAX_BUFFER)
        {
            head = 0;
        }
    }
}

float FIFO::popItem()
{
    float retVal;
    
    counter--;
    retVal = buffer[tail];


    tail++;
    if (tail == MAX_BUFFER)
    {
        tail = 0;
    }
    
    return retVal;    
}

float calculation(float value)
{
    float retVal = 0;

    for (int i = 0; i < ITERATION; i++)
    {
        retVal = sin(value) * 100;
    }
    
    return retVal;
}

// Thread functions
void stageFirst(FIFO & output, float * value)
{
    auto startTime = std::chrono::steady_clock::now();
    
    //cnd_wait(output.isFull,);
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        output.pushItem(calculation(*(value+i)));
    }

    printf("Thread 1 is done with a duration of %ius!\n", std::chrono::duration_cast<std::chrono::microseconds>(startTime - std::chrono::steady_clock::now()).count());
}

void stageMiddle(FIFO & input, FIFO & output)
{
    float retVal;
    auto startTime = std::chrono::steady_clock::now();
    
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        retVal = calculation(input.popItem());
        output.pushItem(retVal);
    }

    printf("Thread 2 or 3 is done with a duration of %ius!\n", std::chrono::duration_cast<std::chrono::microseconds>(startTime - std::chrono::steady_clock::now()).count());
}
void stageLast(FIFO & input, float & output)
{
    auto startTime = std::chrono::steady_clock::now();

    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        output = calculation(input.popItem());
    }

    printf("Thread 4 is done with a duration of %ius!\n", std::chrono::duration_cast<std::chrono::microseconds>(startTime - std::chrono::steady_clock::now()).count());
}

int main(int argc, char const *argv[])
{
    FIFO fifo0;
    FIFO fifo1;
    FIFO fifo2;

    float startValues[ARRAY_SIZE];
    float finalValues[ARRAY_SIZE];

    // initial fill the start array 
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        startValues[i] = (float)rand();
    }
    
    // Create threads
    std::thread stateOne(stageFirst, fifo0, startValues);
    std::thread stateTwo(stageMiddle, fifo0, fifo1);
    std::thread stateThree(stageMiddle, fifo1, fifo2);
    std::thread stateFour(stageFirst, fifo2, finalValues);

    // Wait for all threads
    stateOne.join();
    stateTwo.join();
    stateThree.join();
    stateFour.join();

    printf("All threads done!");

    return 0;
}
