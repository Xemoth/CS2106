#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include "buffer.h"

void initBuffer(TBuffer *buffer)
{
	buffer->count=0;
	buffer->front=0;
	buffer->back=0;
	sem_init(&buffer->full, 0, 0); 
	sem_init(&buffer->empty, 0, 0);
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
}

void enq(TBuffer *buffer, const char *data, int len)
{
	// I only added lock and unlock and also initialisation in buffer.h for question 3

	pthread_mutex_lock(&buffer->mutex);

	if(buffer->count >= QLEN) {
		sem_wait(&buffer->empty);
	}

	unsigned int bytesToCopy = (len < ENTRY_SIZE ? len : ENTRY_SIZE);

	memcpy(buffer->data[buffer->back], data, bytesToCopy);
	buffer->len[buffer->back] = bytesToCopy;
	buffer->count++;
	buffer->back = (buffer->back + 1) % QLEN;
	
	sem_post(&buffer->full);

	pthread_mutex_unlock(&buffer->mutex);
}

int deq(TBuffer *buffer, char *data)
{
	// I only added lock and unlock and also initialisation in buffer.h for question 3

	pthread_mutex_lock(&buffer->mutex);

	if(buffer->count == 0){
		sem_wait(&buffer->full);
	}

	int len = buffer->len[buffer->front];
	memcpy(data, buffer->data[buffer->front], len);
	buffer->count--;
	buffer->front = (buffer->front + 1) % QLEN;

	sem_post(&buffer->empty);	

	pthread_mutex_unlock(&buffer->mutex);

	return len;

}
