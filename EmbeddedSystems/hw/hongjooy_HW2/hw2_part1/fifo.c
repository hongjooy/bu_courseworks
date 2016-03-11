#include "fifo.h"
#include <assert.h>
#include <stdio.h>

void init_fifo(fifo_t *F) {
  F->wptr = F->rptr = 0;
}

void put_fifo(fifo_t *F, int d) {
  if (((F->wptr + 1) % MAXFIFO) != F->rptr) {
    F->data[F->wptr] = d;
    F->wptr = (F->wptr + 1) % MAXFIFO;
    assert(fifo_size(F) <= 10);
  }
}

int get_fifo(fifo_t *F) {
  int r;
  if (F->rptr != F->wptr) {
    r = F->data[F->rptr];
    F->rptr = (F->rptr + 1) % MAXFIFO;
    return r;
  }
  return -1;
}

unsigned fifo_size(fifo_t *F) {
  if (F->wptr >= F->rptr)
    return F->wptr - F->rptr;
  else
    return MAXFIFO - (F->rptr - F->wptr) + 1;
}


void actor_mul(fifo_t *i1, fifo_t *i2, fifo_t *q){
	assert(i1 != 0);
	assert(i2 != 0);
	assert(q != 0);
	//firing rule: fire if there's at least one token at each
	// of the inputs, i1 and i2.
	if ((fifo_size(i1) > 0) && (fifo_size(i2) > 0)){
		put_fifo(q, get_fifo(i1)*get_fifo(i2));
	};

}


void actor_fork(fifo_t *i1, fifo_t *q1, fifo_t *q2){
	assert(i1 != 0);
	assert(q1 != 0);
	assert(q2 != 0);
	if (fifo_size(i1)!=0){
		int a = get_fifo(i1);
		put_fifo(q1,a);
		put_fifo(q2,a);
	}

}

void actor_increment(fifo_t *i1, fifo_t *q){
	assert(i1 != 0);
	assert(q != 0);
	if (fifo_size(i1) != 0){
		put_fifo(q, get_fifo(i1) + 1);
	}
	
}

void actor_print(fifo_t *i1){
	assert(i1 != 0);
	printf("%d\n",get_fifo(i1));
}


















 
