#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>

#include "checkpoints.h"

int main (void){
	/* Acá pueden realizar sus propias pruebas */
	// assert(alternate_sum_4(8,2,5,1) == 10);	
	// assert(alternate_sum_4_using_c(8,2,5,1) == 10);	
	// assert(alternate_sum_4_simplified(8,2,5,1) == 10);	
	// assert(alternate_sum_8(6,2,7,3,4,5,6,7) == 6);

	// uint32_t *x = malloc(sizeof(uint32_t));
	// product_2_f(x , 234, 3.14159);
	// uint32_t y = *x;
	// assert(y == 735);
	// free(x);
	/*
	double *a = malloc(sizeof(double));
	product_9_f(a, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 1, 1, 1,2, 1, 1, 1);
	double b = *a;
	assert(b == 3628800);
	free(a);
	*/

	if(strLen("") == 0){
		printf("OK!");
	}
	if(strLen("sar") == 3){
		printf("OK!");
	}

	char* a = "Omega 4";
	char* ac = strClone(a);
	if(ac){
		printf("%s", ac);
	}
	return 0;    
}
