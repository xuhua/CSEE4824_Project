// Program Y: source code for SESC simulation.
// Young Jin Yoon <youngjin@cs.columbia.edu>

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "define.h"

extern double **matmul(double ***A, int *a, int mat_num);
double **matmul_brute_force(double ***A, int *a, int mat_num);


int main(int argc, char *argv[])
{
	int mat_num, max_row_col, min_row_col;
	int *a;
	double ***A;
	double **result;
	double **ori_result;
	int i, j, k, l;
	int verbose = 0;
	int check = 0;
#ifdef SESC
	sesc_init();
	sesc_fast_sim_begin();
#endif

	if((argc < 4 || argc > 5) || 
	   (argc == 5 && strcmp(argv[1],"-v")))
	{
		printf("Usage: %s [-v] <MAT_NUM> <MAX_ROW_COL> <MIN_ROW_COL> \n\n",argv[0]);
		printf("<MAT_NUM>:     Total number of matrices, \n");
		printf("<MAX_ROW_COL>: The maximum number of rows or columns, \n");
		printf("<MIN_ROW_COL>: The minimum number of rows or columns, \n");
		printf("-v:            Print the elements of all generated matrices and the result of multiplications. \n");
#ifdef SESC
			sesc_exit(0);
#else
			exit(0);
#endif
	}

	if(argc == 5)
	{
		verbose = 1;
		mat_num = strtol(argv[2], NULL, 10);
		max_row_col = strtol(argv[3], NULL, 10);
		min_row_col = strtol(argv[4], NULL, 10);
	} 
	else //argc == 4 
	{
		verbose = 0;
		mat_num = strtol(argv[1], NULL, 10);
		max_row_col = strtol(argv[2], NULL, 10);
		min_row_col = strtol(argv[3], NULL, 10);
	}	

	if(mat_num <= 0 || mat_num >= INT_MAX) 
	{
		printf("ERROR: <MAT_NUM> is incorrectly set!\n");
#ifdef SESC
		sesc_exit(0);
#else
		exit(0);
#endif
	}
	else if(max_row_col <= 0 || max_row_col >= INT_MAX)
	{
		printf("ERROR: <MAX_ROW_COL> is incorrectly set!\n");
#ifdef SESC
		sesc_exit(0);
#else
		exit(0);
#endif
	}	
	else if(min_row_col <= 0 || min_row_col > max_row_col)
	{
		printf("ERROR: <MIN_ROW_COL> is incorrectly set!\n");
#ifdef SESC
		sesc_exit(0);
#else
		exit(0);
#endif
	}

	a = (int *) malloc((mat_num+1) * sizeof(int));
	A = (double ***) malloc(mat_num * sizeof(double **));
	
	/* Stage 1: create 7 random numbers for rows and columns */
	for(i=0; i<mat_num+1;i++)
	{
		a[i] = RAND(min_row_col, max_row_col-min_row_col+1); 
		if(verbose) printf("a[%d]= %d\n",i,a[i]);
	}

	/* Stage 2 and 3: memory allocation of arrays and random generation of element */
	for(i=0; i<mat_num; i++)
	{
		A[i] = (double **) malloc(a[i] * sizeof(double *));
		for(j=0; j<a[i]; j++)
		{
			A[i][j] = (double *) malloc(a[i+1] * sizeof(double));
			for(k=0; k<a[i+1]; k++)
			{
				A[i][j][k] = RAND_DOUBLE; // generate double type random number from 0 to 1.	
			}
		}
	}
	
	/* Extra stage: printing arrays */
	if(verbose)
	{
		for(i=0; i<mat_num; i++)
		{
			printf("A[%d]:\n\n",i);
			for(j=0; j<a[i]; j++)
			{
				for(k=0; k<a[i+1]; k++)
				{
					printf("%lf ",A[i][j][k]);	
				} 
				printf("\n");
			}
			printf("\n\n");
		}
	}
#ifdef SESC
	sesc_fast_sim_end();
#endif
	/* Note that result should have a[0] columns and A[MAT_NUM] rows. */
	result = matmul(A, a, mat_num);
#ifdef SESC
	sesc_fast_sim_begin();
#endif
	ori_result = matmul_brute_force(A, a, mat_num);

	/* print checked result */
	if(!verbose)
	{
		for(i=0; i<a[0];i++)
		{
			for(j=0; j<a[mat_num];j++)
			{
				if(abs(result[i][j] - ori_result[i][j]) > ALPHA) 
				{
					printf("result[%d][%d]: %lf should be %lf\n",i,j,result[i][j],ori_result[i][j]);
					check = 1;	
				}	
			}
		}
		if (!check) printf("your result is correct!\n");
	}

	/* print result and memory deallocation */
	if(verbose) printf(" Original Result: \n\n");
	for(i=0; i<a[0];i++)
	{
		if(verbose)
		{
			for(j=0; j<a[mat_num];j++)
			{
				printf(" %lf",ori_result[i][j]);
			}
		 	printf("\n");
		}
		free(ori_result[i]);
	}

	if(verbose) printf("\n\n Your result: \n\n"); 
	for(i=0; i<a[0];i++)
	{
		if(verbose)
		{
			for(j=0; j<a[mat_num];j++)
			{
				printf(" %lf",result[i][j]);
			}
			printf("\n");
		}
		free(result[i]);
	}
	free(result);
	free(ori_result);

	/* memory deallocation for matrix A */	
	for(i=0; i<mat_num; i++)
	{
		for (j=0; j<a[i]; j++)
		{
			free(A[i][j]);
		}
		free(A[i]);
	}
	free(A);

#ifdef SESC
	sesc_fast_sim_end();
	sesc_exit(0);
#endif
}


/* matmul_brute_force : only for comparison. it is as same as programY initially. You cannot modify this. */ 
double **matmul_brute_force(double ***A, int *a, int mat_num)
{
	/* STARTS HERE */
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

	/* matrix multiplication */
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
	/* END HERE */

}



