#include "bits.h"
#include <stdio.h>


unsigned int BinaryMirror(unsigned int input_num){

	unsigned int bit;
	unsigned int out_num = 0;
	const int NUM_BIT = 32;
	int i;
	for (i = (NUM_BIT-1); i >= 0; i--){ // (i+1) = number of bits i want to convert
		bit = input_num>>i; //about the most significant bit
		unsigned int k = bit%2;

		out_num = out_num + (k<<((NUM_BIT-1)-i));

	};

	return out_num;

}

unsigned int SequenceCount(unsigned int input_num){

	const int NUM_BIT = 32;
	unsigned int bit = 0;
	int standby = 0;
	int count = 0; // counts the '10'

	int i;
	for (i=(NUM_BIT-1); i>=0; i--){
		bit = input_num >> i;
		if((bit%2) == 1){ //if it's 1
			
			standby = 1; // be ready to get 0
		}
		
		else{ //if it's 0
			if (standby == 1) // if the previous bit was 1
			{
				count++; // there we go!
			}
			standby = 0; // if not, then standby down
		}
	}
	return count;

}
