#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define MAX 200

int size;

typedef struct
{
	double d;
	char c;	
} Square;

int get_size()
{
	return size;
}
void set_size(int _size)
{
	size = _size;
}

// check that out!
void print_grid(Square grid[][get_size()], int size)
{
	int i, j;
	for (i=0; i<size; i++)
	{
		for (j=0; j<size; j++)
			printf("%.4lf%c ", grid[i][j].d, grid[i][j].c);
		//if (i % (int)(sqrt(size)) == 0)
		printf("\n");
	}
}

// set flags to squares
void assign_color(Square grid[][get_size()], int size)
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
		for (j=1; j<size-1; j++)
		{
			b = !b;
			if (b) 
				grid[i][j].c = 'b'; 
			else 
				grid[i][j].c = 'w';
		}	
}

// save the desnity of the square we're calculating
int get_density(Square grid[][get_size()], int size, int x, int y)
{
	int i,j;
	// check color for every square
	for (i=x; i<size; i++)
		for (j=y; j<size; j++)
		{
			if (grid[i][j].c == 'b' || grid[i][j].c == 'w')
			{
				grid[i][j].d += grid[i-1][j].d + grid[i][j-1].d + grid[i][j+1].d + grid[i+1][j].d;
				grid[i][j].d /= 5;
			}
		}
	return 1;
}


int main(int argc, char *argv[])
{
	char  *n;
	char str[MAX];
	double N;
	n = argv[1]; //to use as a char use *n
	N = (atoi(n));	//print number N with %g
	
	int i, j;
	int ubound = (N+2);	
	//int size = ubound; //(int) N+2;
	Square grid[ubound][ubound];
	set_size(ubound);
	
	if ( argc != 2 )
		printf("USAGE: <executable> <number of years> \n");
	
	for (i=0; i<ubound; i++)
		for (j=0; j<ubound; j++)
			fscanf(stdin, "%lf", &grid[i][j].d);
	
	assign_color(grid, ubound);	
//before
	printf("#Before \n");
	print_grid(grid, ubound);
	get_density(grid, size, 0, 0);
//after
	printf("#After \n");
	print_grid(grid, ubound);
	return 0;
}
