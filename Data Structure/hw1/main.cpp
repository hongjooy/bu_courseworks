// AUTHOR1: Jooyoun Hong hongjooy@bu.edu
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
using namespace std;

int binary_sum(ifstream&);

int main(int argc, char *argv[]){
	
	
	ifstream infile;
	infile.open(argv[1]);
	int ans = binary_sum(infile);

	cout << ans << endl;
	return 0;
}

int binary_sum(ifstream& intext){
	

	string bit;
	int dec_num_ult = 0;

	while(intext >> bit){
		bitset<32> bit_num(bit); //put each bit into array binum size of 32
		
		int dec_num_temp = 0;
		int dec_num = 0;
		int k = 0;
		
		// binary to decimal
		for (int i = 0; i < 32; i++){

			k = (bit_num[i])%2; // from right to left, checks either the bit is 1 or 0

			dec_num_temp = k<<i; // multiply 2^i
			dec_num = dec_num + dec_num_temp;

		}
	

		dec_num_ult += dec_num;
	

	}



	return dec_num_ult;



}