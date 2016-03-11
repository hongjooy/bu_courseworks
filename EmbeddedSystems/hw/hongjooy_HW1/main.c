#include <stdio.h>
#include <stdlib.h>
#include "bits.h"



int main(int argc, char *argv[]){

	FILE *fp_r;
	FILE *fp_w;
	fp_r = fopen(argv[1],"r"); //open the input.txt
	fp_w = fopen(argv[2],"w"); //output the output.txt


	char string_buff[150];
	char *buff;
	unsigned int out_buff;
	unsigned int first_out;
	unsigned int second_out;

	while(!feof(fp_r)){ // as long as there is content in the input file..

		fgets(string_buff,150,fp_r); // read the input file line by line and store 
									//and store into char array string_buff;

		out_buff = strtoul(string_buff, &buff, 10); // convert char array to unsigned int (base ten)
		first_out = BinaryMirror(out_buff);
		second_out = SequenceCount(out_buff);

		fprintf(fp_w,"%10u       %2u\n",first_out,second_out);
	}



	fclose(fp_r);
	fclose(fp_w);

	return 0;
}
