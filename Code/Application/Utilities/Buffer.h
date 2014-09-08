#ifndef BUFFER_H
#define BUFFER_H
#include "Framework.h"

#define BUFFER_SIZE 15

typedef struct{
	uint8_t     _num_items;             // number of items in the buffer
	uint8_t     _head;                  // first item in the buffer (will be returned with the next pop_front call)
	float       _buff[BUFFER_SIZE];     // x values of each point on the curve
}Buffer_t;

void 	Buffer_clear(Buffer_t*);
void Buffer_pushBack(Buffer_t* buf, float item);
float 	Buffer_popFront();
uint8_t Buffer_isFull(Buffer_t*);
uint8_t Buffer_isEmpty(Buffer_t*);

#endif
