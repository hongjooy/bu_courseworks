#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//const int SIZE_150 = 120;
const int SIZE_2 = 2;
const int SIZE_3 = 3;

typedef struct instr{
	char instruction[150]; // store instruction before parsing
	int index; //instruction index
	char kind[SIZE_2]; // what kind of instruction?
	char arg1[SIZE_3]; // first insturction argument
	char arg2[4]; // second instruction argument
	int cycle; // number of cycles 

}instr;

static int R1, R2, R3, R4, R5, R6 = 0; // six general purpose registers

int mem[100][2];



void store_reg(char, int); // find register & store integer
int get_reg(char); // get the register value
int comp_reg(char, char);


int main(int argc, char *argv[]){

	FILE *fp;


	fp = fopen(argv[1],"r"); //open the input.txt

	instr instr_array[100]; // stores all the instructions
	char instr_buff[150]; 
	char temp[2]; //for string instruction number temp
	

	int current; 

/************************************** Read from file *******************************************/

	int instr_num = 0; // used to store a single instruction to a array of string
	while(!feof(fp)){ // as long as there is content in the input file..

		fgets(instr_buff,150,fp); // read the input file line by line and store 
										//and store into char array string_buff;
		strcpy(instr_array[instr_num].instruction, instr_buff);
		instr_num++;

	}




	printf("Total number of instructions in the code: %d\n", instr_num);
	fclose(fp);


	/************************************ Instruction Parsing *******************************/

	char *rest1; //string from MOV
	char *rest2; //string from the first argument
	char *rest3; //the last argument
	int s1, s2, s3;


	int i, j;
	for (i = 0; i<instr_num; i++){

		rest1 = strchr(instr_array[i].instruction, '\t');
		s1 = (int)(rest1-instr_array[i].instruction);
		rest1 = rest1+1;
		strncpy(temp, instr_array[i].instruction+0, s1); // //stores single line of instruction
	 	instr_array[i].index = atoi(temp);
		rest2 = strchr(rest1,' ');
		s2 = (int)(rest2-rest1);
		strncpy(instr_array[i].kind, rest1+0, s2);
		rest2 = rest2 + 1;
		
		switch (instr_array[i].kind[0]){
			case 'M': // MOV Rn, <num>
			case 'A': // ADD Rn, Rm or ADD Rn, <num>
				instr_array[i].cycle = 1;
				rest3 = strchr(rest2,',');
				s3 = (int)(rest3-rest2);
				strncpy(instr_array[i].arg1, rest2+0, s3);
				rest3 = rest3+2;
				strcpy(instr_array[i].arg2, rest3);
				break; // doesn't break the while loop 
			case 'J': //JE(JMP) <Address>
				instr_array[i].cycle = 1;
				strcpy(instr_array[i].arg1, rest2);
				
				break; // doesn't break the while loop 

			case 'S': //ST [Rm], Rn
				instr_array[i].cycle = 40; // attention: 40 for now!!!!!!!
				rest3 = strchr(rest2,',');
				strncpy(instr_array[i].arg1, rest2+1, 2);
				//printf("%s\n", instr_array[i].arg1);
				strcpy(instr_array[i].arg2, rest3+2);
				//printf("%s\n", instr_array[i].arg2);

				break;	
			case 'L': //LD Rn, [Rm]
				instr_array[i].cycle = 40; // attention: 40 for now!!!!!!!
				rest3 = strchr(rest2,',');
				//printf("%s\n", rest3);
				strncpy(instr_array[i].arg1, rest2, 2);
				strncpy(instr_array[i].arg2, rest3+3, 2);
				//printf("%s\n", instr_array[i].arg2);
				break;

			case 'C':
				instr_array[i].cycle = 2;
				rest3 = strchr(rest2,',');
				s3 = (int)(rest3-rest2);
				strncpy(instr_array[i].arg1, rest2+0, s3);
				//printf("%s\n", instr_array[i].arg1);
				rest3 = rest3+2;
				strcpy(instr_array[i].arg2, rest3);
				//printf("%s\n", instr_array[i].arg2);
				break; // doesn't break the while loop 
		
		}
	

	}	


    /******************************Instruction Components***********************************


	1. Instruction Number: instr_array[j].index (int)
	2. Instruction Kind: instr_array[j].kind[0],instr_array[j].kind[1] (char)
	3. Instruction Argument 1: instr_array[j].arg1[0],instr_array[j].arg1[1] (char)
	4. Instruction Argument 2: instr_array[j].arg2 (char)
	5. Instruction Cycle: instr_array[j].cycle (int)


	 ***************************************************************************************/
	int tmp, tmp2, flag, mem_track = 0;
	int start_ind = instr_array[0].index;


//	printf("R1: %d\nR2: %d\nR3: %d\nR4: %d\nR5: %d\nR6: %d\n ", R1,R2,R3,R4,R5,R6);



	/**************************************** Simulation Flow *********************************/


	for (i = 0; i < instr_num ; i++){
		printf("%s\n", instr_array[i].instruction);
		
		if(instr_array[i].kind[0] == 'M'){//MOV instruction
			store_reg(instr_array[i].arg1[1],atoi(instr_array[i].arg2));

			
		}
		else if(instr_array[i].kind[0] == 'A'){
			if (instr_array[i].arg2[0] == 'R'){ // ADD Rn, Rm 
					tmp = get_reg(instr_array[i].arg1[1]) + get_reg(instr_array[i].arg2[1]);
					store_reg(instr_array[i].arg1[1], tmp);
				}
				else { //ADD Rn, <num>
					tmp = get_reg(instr_array[i].arg1[1]) + atoi(instr_array[i].arg2);
					store_reg(instr_array[i].arg1[1], tmp);
				}

		}
		else if(instr_array[i].kind[0] == 'C'){
			if(comp_reg(instr_array[i].arg1[1],instr_array[i].arg2[1]) == 1){
				flag = 1;
			}
			else{
				flag = 0;
			}	
		}
		else if((instr_array[i].kind[0] == 'J') && (instr_array[i].kind[1] == 'E')){
			if (flag == 1){
				tmp = atoi(instr_array[i].arg1);
				i = tmp - start_ind-1;
				flag = 0;
			}
		}
		else if((instr_array[i].kind[0] == 'J') && (instr_array[i].kind[1] == 'M')){
			tmp = atoi(instr_array[i].arg1);
			i = tmp - start_ind-1;
		}
		else if(instr_array[i].kind[0] == 'S'){ // ST [Rm], Rn --> store content of Rn to mem address in Rm
			//1. Get the content from Rn (arg2)
			tmp = get_reg(instr_array[i].arg2[1]);
			//2. Get the memory address stored in Rm (arg1)
			tmp2 = get_reg(instr_array[i].arg1[1]);
			//3. Allocate memory 
			mem[mem_track][0] = tmp2;
			//4. Store the value 
			mem[mem_track][1] = tmp;
			mem_track++;
		}
		else if(instr_array[i].kind[0] == 'L'){ // LD Rn, [Rm] --> loads from the address stored in Rm into Rn
			//1. Get the memory address from Rm (arg2)
			tmp2 = get_reg(instr_array[i].arg2[1]);
			int k;
			for (k = 0; k < mem_track ; k++){
				if (mem[k][0] == tmp2){ //once it finds the matching number
					tmp = mem[k][1]; // store the value from the memory address
					break; //break the loop
				}
			}
			store_reg(instr_array[i].arg1[1], tmp); //store into the register
		}
		printf("R1: %d | R2: %d | R3: %d | R4: %d | R5: %d | R6: %d\n\n\n ", R1,R2,R3,R4,R5,R6);
	}

    /* printing memory location */
	// int row, columns;
	// for (int row=0; row<100; row++)
	// {
	//     for(int columns=0; columns<2; columns++)
	//          printf("%d     ", mem[row][columns]);
	//     printf("\n");
	// }
	
	return 0;
}



void store_reg(char reg_num, int store_num){
	if(reg_num == '1'){
			R1 = store_num;
		}
	else if(reg_num == '2'){
		R2 = store_num;
	}
	else if(reg_num == '3'){
		R3 = store_num;
	}
	else if(reg_num == '4'){
		R4 = store_num;
	}
	else if(reg_num == '5'){
		R5 = store_num;
	}
	else if(reg_num == '6'){
		R6 = store_num;
	}
	


}

int get_reg(char reg_num){
	if(reg_num == '1'){
		return R1;
	}
	else if(reg_num == '2'){
		return R2;
	}
	else if(reg_num == '3'){
		return R3;
	}
	else if(reg_num == '4'){
		return R4;
	}
	else if(reg_num == '5'){
		return R5;
	}
	else if(reg_num == '6'){
		return R6;
	}
	else return -1;

}

int comp_reg(char reg_1, char reg_2){
	if (get_reg(reg_1) == get_reg(reg_2)){
		return 1;
	}
	else return 0;

}












