#include <stdio.h>
#include <threads.h>
#include <atomic>
#include <math.h>
#include <vector>

#define MAX_BUFFER 10


class FIFO
{
private:
    std::atomic<std::vector<float>> buffer;
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
    return (sin(value) * sin(value) * 100);
}

void stageFirst(FIFO & output, float value)
{
    //cnd_wait(output.isFull,);

    output.pushItem(calculation(value));
}

void stageMiddle(FIFO & input, FIFO & output)
{
    float retVal; 

    retVal = calculation(input.popItem());
    output.pushItem(retVal);
}
void stageLast()
{

}

int main(int argc, char const *argv[])
{
    FIFO fifo0;
    FIFO fifo1;
    FIFO fifo2;


    return 0;
}
