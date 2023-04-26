#include<iostream>
#include"omp.h"
#include<chrono>
using namespace std;

const int arr_size = 20000;
const int arr_size1 = 15000;

int arr[arr_size][arr_size1];

void init_arr();
long long part_sum( int);
long long part_min( int);
int sizeThread = 4;


int main() {
	
	init_arr();
	omp_set_nested(1);
	double t1 = omp_get_wtime();
#pragma omp parallel sections
	{
#pragma omp section
		{
			long long x = part_min(sizeThread);
			printf("minsum %d = %d \n", sizeThread, x);
		}
		
#pragma omp section
		{
			long long x = part_sum( sizeThread);
			printf("sum %d = %d \n", sizeThread , x);

		}
	}
	double t2 = omp_get_wtime();
	
	printf("Total time - %F seconds", t2 - t1);
	return 0;
}

void init_arr() {
	for (int i = 0; i < arr_size; i++) {
		for (size_t j = 0; j < arr_size; j++)
		{
			arr[i][j] = arr_size*arr_size - i*arr_size-j;
		}
	}
}

long long part_sum(int num_threads) {
	long long sum = 0;
	double t1 = omp_get_wtime();
#pragma omp parallel for reduction(+:sum) num_threads(num_threads)	
		for (int i = 0; i < arr_size ; i++) {
			for (size_t j = 0;  j < arr_size1;  j++)
			{
				sum += arr[i][j];
			}
		}
	double t2 = omp_get_wtime();
	printf("sum time - %Fseconds \n", t2 - t1);

	return sum;
}

long long part_min( int num_threads) {
	long long minsum = INT32_MAX;
	double t1 = omp_get_wtime();
#pragma omp parallel for num_threads(num_threads)
	for (int i = 0; i < arr_size; i++) {
		long long sum = 0;
		for (size_t j = 0; j < arr_size1; j++)
		{
			sum += arr[i][j];
		}
		
		if (minsum > sum)
		{
#pragma omp critical
			if (minsum > sum)
			{
				minsum = sum;
			}
		}
	}
	double t2 = omp_get_wtime();
	printf("min sum time - %F seconds \n", t2 - t1);


return minsum;
}