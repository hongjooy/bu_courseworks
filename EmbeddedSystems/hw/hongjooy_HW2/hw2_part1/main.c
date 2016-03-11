#include <stdio.h>
#include "fifo.h"

int main() {

	fifo_t num;
	fifo_t q_num1;
	fifo_t q_num2; //loop back
	fifo_t ind;
	fifo_t q_ind1;
	fifo_t q_ind2;
	fifo_t q_ind3; //loop back
	fifo_t print_in;

	init_fifo(&num);
	init_fifo(&q_num1);
	init_fifo(&q_num2);
	init_fifo(&ind);
	init_fifo(&q_ind1);
	init_fifo(&q_ind2);
	init_fifo(&q_ind3);
	init_fifo(&print_in);


	put_fifo(&num, 43);
	put_fifo(&ind, 1);
	
	int i ;
	for (i = 0; i < 6; i++){
		actor_fork(&num, &q_num1, &q_num2);
		actor_fork(&ind, &q_ind1, &q_ind2);

		actor_mul(&q_num1, &q_ind1, &print_in);
		actor_increment(&q_ind2, &q_ind3);

		put_fifo(&num, get_fifo(&q_num2));
		put_fifo(&ind, get_fifo(&q_ind3));
	
		actor_print(&print_in);
	}

	
	
	return 0;
}
