#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <errno.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <stdint.h>

/******************************************************************************
*Templates circular_buffer
******************************************************************************/
typedef uint8_t buffer_value_t;
//typedef void buffer_value_t; <-- How to implement this??
typedef struct circular_buffer circular_buffer_t;

/******************************************************************************
*Signatures of functions in the .c file                                       *
******************************************************************************/
void write(circular_buffer_t *buffer, buffer_value_t value);
void overwrite(circular_buffer_t *buffer, buffer_value_t value);
void read(circular_buffer_t *buffer, buffer_value_t *read_value);
circular_buffer_t *new_circular_buffer(size_t capacity);
void delete_buffer(circular_buffer_t *buffer);
void clear_buffer(circular_buffer_t *buffer);

#endif
