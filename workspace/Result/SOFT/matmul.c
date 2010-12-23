// Matrix-chain multiplication source code for SESC simulation.
// Young Jin Yoon <youngjin@cs.columbia.edu>

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "define.h"
#include <errno.h>
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

double **rand_matrix(double **ptr_matrix, int row, int col)
{
	int i,j;
	for (i = 0; i< row; i++) {
		for (j = 0; j< col; j++) {
			ptr_matrix[i][j] = rand()%10;
		}
	}
	return ptr_matrix;
}

void printf_matrix_fp(float **c, int mat_num, char* comm)
{
	int i,j;
	printf(" %s:\n",comm);
	for (i = 0; i < mat_num; i++) {
		for (j = 0; j < mat_num; j++) {
			printf(" %7f",c[i][j]);
		}
		printf("\n");
	}
}

void printf_matrix_int(int **c, int row, int column, char* comm)
{
	int i,j;
	printf(" %s:\n",comm);
	for (i = 0; i < row; i++) {
		for (j = 0; j < column; j++) {
			printf(" %d",c[i][j]);
		}
		printf("\n");
	}
}

double **matmul_two(double **result, double **array_a, double **array_b, int x, int y, int z)
{
	int i,j,k;
	for (i = 0; i< x ;i++) {
		for (j = 0; j< z;j++) {
			result[i][j] = 0;
			for(k = 0; k< y; k++)
				result[i][j] += array_a[i][k]*array_b[k][j];
		}
	}
	return result;
}

int dynamic_programming(int **m, int **s, int *a, int mat_num)
{
	int i,j,k,q,L;
	for (L=2;L<=mat_num;++L) {
		for (i=0;i<mat_num-L+1;++i) {
			j=i+L-1;
			m[i][j]=2147483647;
			for (k=i;k<=j-1;++k) {
				q=m[i][k]+m[k+1][j]+a[i]*a[k+1]*a[j+1];
				if (q<m[i][j]) {
					m[i][j]=q;
					s[i][j]=k;
				}
			}
		}
	}
	return 0;
}

double **matrix_chain(double ***mat_ptr, int *p, int **s, int i, int j)
{
	int k;
	if (i < j) {
		k = s[i][j];
		return matmul_two(*mat_ptr,matrix_chain(mat_ptr,p,s,i,k),\
				  matrix_chain(mat_ptr,p,s,k+1,j),p[i],p[k+1],p[j+1]);
	}
	else
		return *(mat_ptr + i);
}
/**************************************************************************************/
double **matmul(double ***A, int *a, int mat_num)
{
	/*Cycle time holder*/
	//	long long int time_start;
	double **result;
	/* The Chain Matrix Multiplication STARTS HERE */
	int i;
	/* memory allocation for m s holder*/ 
	//time_start = getcycles();
	int **m, **s;
	m = malloc(mat_num * sizeof(int *));
	for (i = 0; i< mat_num; i++) {
		m[i] = malloc(mat_num * sizeof(int));
	}
	s = malloc(mat_num * sizeof(int *));
	for (i = 0; i< mat_num; i++) {
		s[i] = malloc(mat_num * sizeof(int));
	}
	/*********Dynamic Programming****************/
	dynamic_programming(m,s,a,mat_num);
	/* Debug print */
	/*
	printf_matrix_int(m, mat_num, mat_num, "m_Matrix");
	printf_matrix_int(s, mat_num, mat_num, "s_Matrix");
	printf_matrix_int(&a, 1, mat_num+1, "a_matrix");
	*/
	/********** Chain multiplication **************/
	result =  matrix_chain(A, a, s, 0, mat_num-1);
	/*Free memory*/
	for (i = 0; i<mat_num; i++) {
		free(m[i]);
		free(s[i]);
	}
	//printf("CPU cycles: %lld\n",(getcycles()-time_start));
	return result;
 }
