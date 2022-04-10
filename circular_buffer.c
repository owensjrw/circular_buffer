#include "circular_buffer.h"

/******************************************************************************
*Circular buffer struct identified as circular_buffer_t                       *
*.read is the Head(Read) marker for array[marker];                            *
*.write is the Tail(Write) marker for array[marker];                          *
*.slots_used is an indicator of the number of slots with valid data           *
*.max_ring_size is the capacity of the circurlar buffer                       *
*.ring Flexable Array Member to keep memory of struct congruent               *
******************************************************************************/

struct circular_buffer {
  size_t read;
  size_t write;
  size_t slots_used;
  size_t max_ring_size;
  buffer_value_t ring[];
};

/******************************************************************************
*Helper function to move head(write) and tail(read) markers                   *
*Helper function to check for full or empty buffer                            *
******************************************************************************/

static void move_by_one(circular_buffer_t *buffer, size_t *read_write){
  *read_write += 1;
  if(*read_write == buffer->max_ring_size){
    *read_write = 0;
  }
}

static size_t is_empty(circular_buffer_t *buffer){
  return buffer->slots_used == 0;
}

static size_t is_full(circular_buffer_t *buffer){
  return buffer->slots_used == buffer->max_ring_size;
}

/******************************************************************************
*Writes uint8_t size variables to buffer slots if buffer not full             *
******************************************************************************/

uint16_t write(circular_buffer_t *buffer, buffer_value_t value){
  if(is_full(buffer)){
    errno = ENOBUFS;
    return EXIT_FAILURE;
  }
  buffer->ring[buffer->write] = value;
  move_by_one(buffer, &buffer->write);
  buffer->slots_used += 1;
  return EXIT_SUCCESS;
}

/******************************************************************************
*Overwrites uint8_t size variables to buffer slots if buffer full             *
******************************************************************************/

uint16_t overwrite(circular_buffer_t *buffer, buffer_value_t value){
  if(!is_full(buffer)){
    write(buffer, value);
  } else {
    buffer->ring[buffer->write] = value;
    move_by_one(buffer, &buffer->read);
    move_by_one(buffer, &buffer->write);
  }
  return EXIT_SUCCESS;
}

/******************************************************************************
*Reads data from current tail position                                        *
*After read moves tail by one slot forward (pop() oldest data FIFO)           *
******************************************************************************/

uint16_t read(circular_buffer_t *buffer, buffer_value_t *read_value){
  if(is_empty(buffer)){
    errno = ENODATA;
    return EXIT_FAILURE;
  }
  *read_value = buffer->ring[buffer->read];
  move_by_one(buffer, &buffer->read);
  buffer->slots_used -= 1;
  return EXIT_SUCCESS;
}

/******************************************************************************
*Creates a circular buffer of uint8_t size buffer slots                       *
*Max capacity is of size_t                                                    *
******************************************************************************/

circular_buffer_t *new_circular_buffer(size_t capacity){
  circular_buffer_t *ring_buffer = malloc(sizeof(circular_buffer_t) +
                                    sizeof(buffer_value_t) * capacity);
  assert(ring_buffer != 0);

  ring_buffer->read = 0;
  ring_buffer->write = 0;
  ring_buffer->slots_used = 0;
  ring_buffer->max_ring_size = capacity;
  return ring_buffer;
}

/******************************************************************************
*Deletes buffer from memory                                                   *
******************************************************************************/

void delete_buffer(circular_buffer_t *buffer){
  free(buffer);
}

/******************************************************************************
*Virtual reset of data in buffer by reseting Head, Tail,                      *
*and slots_used back to 0                                                     *
******************************************************************************/

void clear_buffer(circular_buffer_t *buffer){
  buffer->read = 0;
  buffer->write = 0;
  buffer->slots_used = 0;
}
