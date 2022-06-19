#include <threads.h>
#include <stdio.h>
#include <random>
#include <vector>

class FIFO
{
private:
    int fuellstandMax = 10; 
    std::vector<int> puffer;

public:
    FIFO();
    ~FIFO();
    // returns the max value of the puffer
    int FIFO::getMaxFuellstand();
    // returns the size of the puffer
    int getFuellstand();
    /*
    * @brief set a new value if the puffer is not full 
    * @param int value new value for the puffer
    * @return 0 if the push was successfull, 1 if the push didn't work (puffer full)
    */
    int push(int value);
    int pop();

};

FIFO::FIFO()
{
}

FIFO::~FIFO()
{
}

int FIFO::getFuellstand()
{
    return puffer.size();
}

int FIFO::getMaxFuellstand()
{
    return fuellstandMax;
}

int FIFO::push(int value)
{
    if (puffer.size() < fuellstandMax)
    {
        puffer.push_back(value);
        return 0;
    }
    else
    {
        return 1;
    }
}

int FIFO::pop()
{
    int retVal;
    if (puffer.size() > 0)
    {
        retVal = puffer[0];
        puffer.erase(puffer.begin());
        return retVal;
    }
    
}

void stage1(FIFO & outputFifo)
{
    if (outputFifo.getMaxFuellstand() > outputFifo.getFuellstand() )
    {
        /* code */
    }
    
}

void stage2(FIFO & inputFifo, FIFO & outputFifo)
{
    
}

void stage3(FIFO & inputFifo, FIFO & outputFifo)
{
    
}

void stage4(FIFO & inputFifo)
{
    
}

int main(int argc, char const *argv[])
{
    FIFO fifo1;
    FIFO fifo2;
    FIFO fifo3;



    return 0;
}
