#include "Buffer.h"

void Buffer_init(Buffer_t* buf){
	buf->_head = 0;
	buf->_num_items = 0;
}

void Buffer_clear(Buffer_t* buf){
	buf->_head = 0;
	buf->_num_items = 0;
}

void Buffer_pushBack(Buffer_t* buf, float item){
    //determine position of new item
    uint8_t tail = buf->_head + buf->_num_items;
    if( tail >= BUFFER_SIZE ) {
        tail -= BUFFER_SIZE;
    }

    // add item to buffer
    buf->_buff[tail] = item;

    // increment number of items
    if(  buf->_num_items < BUFFER_SIZE ) {
    	 buf->_num_items++;
    }else{
        //no room for new items so drop oldest item
    	 buf->_head++;
        if(  buf->_head >= BUFFER_SIZE ) {
        	 buf->_head = 0;
        }
    }
}

float Buffer_popFront(Buffer_t* buf){
	float result;

	//return zero if buffer is empty
	if(buf->_num_items == 0 ) {
		return 0;
	}

	// get next value in buffer
    result = buf->_buff[buf->_head];

    // increment to next point
    buf->_head++;
    if( buf->_head >= BUFFER_SIZE )
    	buf->_head = 0;

    // reduce number of items
    buf->_num_items--;

    // return item
    return result;
}

uint8_t Buffer_isFull(Buffer_t* buf){
	 return buf->_num_items >= BUFFER_SIZE;
}

uint8_t Buffer_isEmpty(Buffer_t* buf ){
	 return buf->_num_items == 0;
}

