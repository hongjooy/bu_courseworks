// AUTHOR1: Jooyoun Hong hongjooy@bu.edu
// AUTHOR2: Andy Shen shena@bu.edu


int finding_median(int a[], int size_a, int b[], int size_b, int im){

	//size_a  <  size_b
     
    if (size_a > size_b) {
    	return finding_median(b,size_b,a,size_a,im);
    }

    /* Keep the recurrsion until...*/
 	// 1) the size of the smaller array is zero!
    if (size_a==0) {
     	return b[im-1];
 	}
    if (im==1) {
    	 return (a[0]<b[0]) ? a[0]:b[0];
    }
    /* index of the median */
 	//int im = (sa+sb)/2 + 1; 
 	/* index of a */
    int index_a = (im/2<size_a) ? im/2 : size_a;
    int index_b = im - index_a;

    /* Compare the lower 25% of entire array (smaller one can be dropped) */
    if (a[index_a-1]<=b[index_b-1]) {

    	// the low 1/4 of entire array is from a, and the numbers below it also can be dropped
    	return finding_median(&a[index_a],size_a-index_a,b,size_b,im-index_a);
    }
    else{
    	 return finding_median(a,size_a,&b[index_b],size_b-index_b,im-index_b);
    }
   
}

int main(){

	int a[] = {2,10,70,123};
	int b[] = {1,56,77,65,104};
	int size_a = sizeof(a)/4; 
	int size_b = sizeof(b)/4;
	int median_index = (size_a+size_b)/2 + 1;

	int ans = finding_median(a,4,b,5,median_index);

	
}