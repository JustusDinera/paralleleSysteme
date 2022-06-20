#include <threads.h>
#include <stdio.h>
#include <random>
#include <vector>

#define BUFFER_MAX 10

/**
 * Used as a modulo operator
 * as <tt> a % b = (a & (b − 1)) </tt>
 * where \c a is a positive index in the buffer and
 * \c b is the (power of two) size of the buffer.
 */
#define BUFFER_MASK (BUFFER_MAX-1)

/**
 * Simplifies the use of <tt>struct BUFFER</tt>.
 */
//typedef struct BUFFER BUFFER;

/**
 * Structure which holds a ring buffer.
 * The buffer contains a buffer array
 * as well as metadata for the ring buffer.
 */
typedef struct BUFFER {
    /** Buffer memory. */
    char buffer[BUFFER_MAX];
    /** Index of tail. */
    uint tailIndex;
    /** Index of head. */
    uint headIndex;
};

class FIFO
{
private:
    BUFFER buffer;
public:
    FIFO(/* args */);
    
    ~FIFO();
    
    void bufferInit();
    
    void bufferQueue(char data);
    
    void bufferQueueArr(const char *data, uint size);
    
    uint8_t bufferDequeue(char *data);
    
    uint bufferDequeueArr(char *data, uint len);
    
    uint8_t bufferPeek(char *data, uint index);

    uint8_t bufferIsEmpty();
    
    uint8_t bufferIsFull();
    
    uint bufferNumItems();

    uint8_t FIFO::buffer_peek(char *data, uint index);

};

FIFO::FIFO()
{
}

FIFO::~FIFO()
{
}


void FIFO::bufferInit() {
  buffer.tailIndex = 0;
  buffer.headIndex = 0;
}

void FIFO::bufferQueue(char data) {
  /* Is buffer full? */
  if(bufferIsFull()) {
    /* Is going to overwrite the oldest byte */
    /* Increase tail index */
    buffer.tailIndex = ((buffer.tailIndex + 1) & BUFFER_MASK);
  }

  /* Place data in buffer */
  buffer.buffer[buffer.headIndex] = data;
  buffer.headIndex = ((buffer.headIndex + 1) & BUFFER_MASK);
}

void FIFO::bufferQueueArr(const char *data, uint size) {
  /* Add bytes; one by one */
  uint i;
  
  for(i = 0; i < size; i++) {
    bufferQueue(data[i]);
  }
}

uint8_t FIFO::bufferDequeue(char *data) {
  if(bufferIsEmpty()) {
    /* No items */
    return 0;
  }
  
  *data = buffer.buffer[buffer.tailIndex];
  buffer.tailIndex = ((buffer.tailIndex + 1) & BUFFER_MASK);
  return 1;
}

uint FIFO::bufferDequeueArr(char *data, uint len) {
  char *data_ptr = data;
  uint cnt = 0;

  if(bufferIsEmpty()) {
    /* No items */
    return 0;
  }

  while((cnt < len) && bufferDequeue(data_ptr)) {
    cnt++;
    data_ptr++;
  }
  return cnt;
}

uint8_t FIFO::buffer_peek(char *data, uint index) {
  /* Add index to pointer */
  uint dataIndex = ((buffer.tailIndex + index) & BUFFER_MASK);
  *data = buffer.buffer[dataIndex];

  if(index >= bufferNumItems()) {
    /* No items at index */
    return 0;
  }
  
  return 1;
}

uint8_t FIFO::bufferIsEmpty() {
    return (buffer.headIndex == buffer.tailIndex);
}

uint8_t FIFO::bufferIsFull() {
    return ((buffer.headIndex - buffer.tailIndex) & BUFFER_MASK) == BUFFER_MASK;
}

uint FIFO::bufferNumItems() {
    return ((buffer.headIndex - buffer.tailIndex) & BUFFER_MASK);
}

/*************************************************
 * ***********************************************
 * ***********************************************
 */


void stage1(FIFO & outputFifo)
{
    if ()
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
