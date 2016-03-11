#ifndef FIFO_H
#define FIFO_H

#define MAXFIFO 1024

typedef struct fifo {
  int data[MAXFIFO];
  unsigned wptr;
  unsigned rptr;
} fifo_t;

void     init_fifo(fifo_t *F);
void     put_fifo (fifo_t *F, int d);
int      get_fifo (fifo_t *F);
unsigned fifo_size(fifo_t *F);
void 	 actor_mul(fifo_t *i1, fifo_t *i2, fifo_t *q);
void     actor_fork(fifo_t *i1, fifo_t *q1, fifo_t *q2);
void     actor_increment(fifo_t *i1, fifo_t *q);
void     actor_print(fifo_t *i1);

#endif

