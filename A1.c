/* To run program
 compile:  gcc -pthread -o A1 A1.c
 run: ./A1 N <file
 	where N is the number of years of density evaluation and file is a matrix of numbers, separated by spaces, of N+2 columns and N+2 rows
 	eg.
 		5.9998 5.9999 6.0002 6.0003 5.9998 5.99997
		6.0001 6.0000 6.0001 6.0002 6.00001 5.99998
		4.9999 5.9999 6.0000 5.9998 5.99997 6.00001
		6.00001 6.0001 5.9998 5.9999 6.0002 5.99997
		5.9998 5.99997 6.00001 4.9999 5.9999 6.0000
		6.0001 6.0000 6.0001 5.9998 5.9999 6.0002
 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include <pthread.h>
#define NUM_THREADS 4
#define MAX 100

int size;
char color;
void set_size(int _size);
int get_size();
char get_color();

typedef struct
{
	double d;
	char c;	
} Square;

Square grid[MAX][MAX];
Square steady[MAX];

int ac;	// array counter
int get_density(Square grid[][get_size()], int size, int x, int y, int x1, int y1, char b_w);
void print_grid(Square grid[][get_size()], int size);
// our thread function
void *compute_density(void *args)
{
	int tid = *(int *) args;
	int i;
	// divide up the jobs folk
 	switch(tid)
	{
		case 0: get_density( grid,  size, 0, 	0, size/2, size/2, get_color()); break;
		case 1:	get_density( grid,  size,	0,  size/2, size/2, size, get_color()); break;
		case 2:	get_density( grid,  size, size/2,  0, size, size/2, get_color()); break;
		case 3:	get_density( grid,  size,  size/2,  size/2, size, size, get_color()); break;
	}
}

int get_size() 	{return size;}
void set_size(int _size) {size = _size;}
char get_color() {return color;}
void set_color(char c) {color = c;}

// check that out!
void print_grid(Square grid[MAX][MAX], int size)
{
	int i, j;
	for (i=0; i<size; i++)
	{
		for (j=0; j<size; j++)
			printf("%.4lf%c ", grid[i][j].d, grid[i][j].c);
		printf("\n");
	}
	printf("\n");
}

// set flags to squares
void assign_color(Square grid[MAX][MAX], int size)
{
	int i,j;
	int b;	// alternating boolean
	b = 0;
	// first fill all with gray
	for (i=0; i<size; i++)
		for (j=0; j<size; j++)
			grid[i][j].c = 'g';	
	// start @[1,1] for b/w flags
	for (i=1; i<size-1; i++)
	{
		for (j=1; j<size-1; j++)
		{
			b = !b;
			if (b) 
				grid[i][j].c = 'b'; 
			else 
				grid[i][j].c = 'w';
		}
		b = !b;
	}	
}

// save the desnity of the square we're calculating
int get_density(Square grid[MAX][MAX], int size, int x, int y, int x1, int y1, char b_w)
{
	int i,j;
	double temp;
	
	if (!(b_w=='b' || b_w=='w'))
		return 0;
	
	// check color for every square
	for (i=x; i<x1; i++)
		for (j=y; j<y1; j++)
		{
			// also check for steady state
			if (grid[i][j].c == b_w)
			{
				// current density
				temp = grid[i][j].d + grid[i-1][j].d + grid[i][j-1].d + grid[i][j+1].d + grid[i+1][j].d;
				temp /= 5;
				if ( temp-grid[i][j].d <= 0.0001 && temp-grid[i][j].d >= -0.0001 )
					steady[ac++] =  grid[i][j];	// test
					// Change density
				grid[i][j].d = temp;	
			}
		}
	return 1;
}

// return rc
void run_threads(pthread_t *threads, int *thread_args, char c, int total_comparables, int N) //thread_args[NUM_THREADS]
{
	
	int i, rc;
	set_color(c);
	for (i=0; i<NUM_THREADS; ++i) {
      thread_args[i] = i;
      rc = pthread_create(&threads[i], NULL, compute_density, (void *) &thread_args[i]);
      assert(0 == rc);
   	} 
   // wait for each thread to complete
   for (i=0; i<NUM_THREADS; ++i) {
      // block until thread i completes
      rc = pthread_join(threads[i], NULL);
      assert(0 == rc);
   }
   for (i=0; i<ac; i++) //steady[i].c!='\0'
	{
		//printf("steady counter: %d\n", ac);
		if (ac > total_comparables)
		{
			fprintf(stderr, "ERROR: the number of steady-state squares outrun the number of comparable squares\n");
			exit(1);
		}
		else if (ac == total_comparables)
		{
			printf("SUCCESS: steady state reached after %d years. Terminating..\n", (int)N);
			exit(0);
		}	
	}
}   

int main(int argc, char *argv[])
{
	char  *n;
	double N;	
	int i, j, ubound;
	int steady_counter;
	int total_comparables;
	char c_temp;

	n = argv[1]; //to use as a char use *n
	N = (atoi(n));	//print number N with %g
	ubound = (N+2);	
	ac = 0;
	
	total_comparables = N*N;

	// Define thread array
	pthread_t threads[NUM_THREADS];

	// Global size of our grid
	set_size(ubound);
	
	if ( argc != 2 )
		fprintf(stderr, "USAGE: <executable> <number of years> \n");
	
	for (i=0; i<ubound; i++)
		for (j=0; j<ubound; j++)
			fscanf(stdin, "%lf", &grid[i][j].d);
			
	// Set flags to our grid for threads
	assign_color(grid, ubound);	

	int thread_args[NUM_THREADS];
	
   // Display initial density
   printf("Initial density\n");
   print_grid(grid, size);
   
   // Computer density for N years
   for (i=0; i<N; i++)
   {
   		printf("Year %d\n", i+1);
   // Run parallel threads; once for black and once for white squares
		run_threads(threads, thread_args, 'b', total_comparables, N); 
		run_threads(threads, thread_args, 'w', total_comparables, N);
		print_grid(grid, size);
		// steady square
	//printf("steady squares:\n");
		/*
		
		*/
	}	
	
	return 0;
}
