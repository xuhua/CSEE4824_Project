// Matrix-chain multiplication source code for SESC simulation.
// Young Jin Yoon <youngjin@cs.columbia.edu>

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "define.h"

/*
 * This helper function printout the CPU time in X86/X64 structure.
 * Comment them after optimization
 */
#define rdtsc(low,high) \
	__asm__ __volatile__("rdtsc" : "=a" (low), "=d" (high))

static inline long long int getcycles()
{
	unsigned long low;
	long high;
	rdtsc(low, high);
	return ((long long int)high << 32) | low;
}

double **matmul(double ***A, int *a, int mat_num)
{
	/*Cycle time holder*/
	long long int time_start;
	/* The Chain Matrix Multiplication STARTS HERE */
	double ***B;
	double **result;
	int i, j, k, l;
	/* memory allocation for intermediate results */ 
	B = (double ***) malloc((mat_num)*sizeof(double **));
	B[0] = A[0];
	
       	for(i=1; i<mat_num;i++)
	{
		B[i] = (double **)malloc(a[0]*sizeof(double *));
		for(j=0; j<a[0]; j++)
		{
			B[i][j] = (double *)malloc(a[i+1] * sizeof(double));
		}
	}
	time_start = getcycles();

	for(i=1; i<mat_num;i++)
	{
		for(j=0; j<a[0]; j++)
		{
			for(k=0; k<a[i+1]; k++)
			{
				B[i][j][k] = 0;
				for(l=0; l<a[i]; l++)
				{
					B[i][j][k] += B[i-1][j][l] * A[i][l][k];
				}
			} 
		}
	}
	result = B[mat_num-1];
	
	printf("CPU cycles: %lld\n",(getcycles()-time_start));
	/* memory deallocation - this loop does not deallocate B[0](= A[0]) and B[mat_num](=result).*/
	for(i=1; i<mat_num-1;i++)
	{
		for(j=0; j<a[0]; j++)
		{
			free(B[i][j]); 
		}
		free(B[i]);

	}
	free(B);
	return result;
	/* The Chain Matrix Multiplication ENDS HERE */

}
