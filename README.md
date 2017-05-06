# Weed-Take-Over---Threaded-Density-Calculation-Program
A agricultural fringe group, known as BLUT (Beautifying Latent Urban Terrain), 
is an advocate for a particular type of native Ontario weed, which they believe to be 
extremely beautiful.  This weed, however, is banned from the city of Hamilton.  BLUT's 
mission is to "seed" Hamilton with this weed, so that all citizens may enjoy it.  
Members of BLUT plan to plant this weed around the perimeter of Hamilton (where doing 
so is legal), and some members hope to covertly plant some weeds in the city limits.

Before undertaking this mission, BLUT would like answers to the following:
 -If we plant the weeds in a given initial pattern, how many years will it take for the
  weed to reach a steady-state in Hamilton?
 -If we plant the weeds in a given initial pattern, what will the distribution of weeds
  look like in Hamilton after N years?

BLUT knows this about the growth distribution of the weed:
In any one square region, C, the weed-density in Year X+1 is the average of the 
weed-densities from Year X of regions C,N,S,E,W, where each of C,N,S,E,W are the
square regions as shown in Figure 1.

  -|-----|-----|-----|-
   |     |  N  |     |
  -|-----|-----|-----|-
   |  W  |  C  |  E  |
  -|-----|-----|-----|-
   |     |  S  |     |
  -|-----|-----|-----|-
     Figure 1.

To help them answer their questions, BLUT hired a mathematics expert, who pointed out 
that the calculations contain some dependencies. Consider Figure 2.
The calculation for next year's density for region 7  involves regions: 3,7,6,8,11.
The calculation for next year's density for region 11 involves regions: 7,10,11,12,15.
So region 7 depends on region 11, but region 11 depends on region 7.
Thus next year's densities for regions 7 and 11 will depend on the order in which the
calculations for 7 and 11 were performed.


  -|-----|-----|-----|-----|-
   |  1  |  2  |  3  |  4  | 
  -|-----|-----|-----|-----|-
   |  5  |  6  |  7  |  8  | 
  -|-----|-----|-----|-----|-
   |  9  | 10  | 11  | 12  | 
  -|-----|-----|-----|-----|-
   | 13  | 14  | 15  | 16  | 
  -|-----|-----|-----|-----|-
      Figure 2.

The mathematician suggested partitioning Hamilton into black and white regions, and 
the perimeter into grey regions, as shown in Figure 3, which is in:  
    http://www.scs.ryerson.ca/~dwoit/courses/cps590/coursedirPublic590/A1Figure3.jpg  
No dependencies exist as long as all the densities for the black regions are calculated 
first, followed by the white regions. The grey regions are outside Hamilton, so the 
members of BLUT could keep those distributions at a constant level from year to year. 
Thus the densities of the grey regions will not change from year to year, and it is 
never necessary to re-calculate their yearly distributions. (The grey regions contribute 
to the calculations for the black/white regions, however.)
The mathematician expects that when enough years pass, the values in the black/white
regions will converge to a distribution reflective of the fixed values of the perimeter.
Thus, BLUT will be able to accomplish their mission of weeding-up Hamilton, without 
breaking the law (except possibly in year 0--the initial seeding).

The mathematician suggested BLUT hire a student to write a program to answer BLUT's 
questions.  BLUT hired a student from U of T, but this student's program ran very slowly, 
especially when BLUT tried using it to simulate weeding-up the GTA, a very large region.

BLUT heard that the Ryerson second year CS students were learning about programming with
threads, so they have hired YOU to write a more efficient program, with threads.

Assumptions:
  The black/white area is always an NxN square, where 
  N is always a power of 2 in [2,4,8,16,32,64].
  The number of spawned threads, T, is always 4. 
  Global variables may be used.
  The density grid may be statically or dynamically allocated. 
  The density grid may be a global.

Your Program Must:
  Compile and work on a CS moon.
  Use Linux user-level threads via the moon's POSIX pthreads library. ( gcc -pthread )
  Synchronize main and/or threads appropriately using joins (no more sophisticated
    thread synchronization methods are required).
  Be robust, and print appropriate error messages to stderr.
  Partition the white/black region into T areas.
  Perform the calculations for the next year by using 4 parallel threads, with each thread
    doing the calculations for its own area of the grid only.
  Avoid ALL dependencies in the calculations by using the black/white approach.
  
  Store the Densities as type double. 
  Read initial density grid from stdin, in accordance with the following format:
    -N is on line 1.
    -N+2 lines follow, with each line corresponding to the initial densities of 
     the grid, including the grey (perimeter) regions.
    i.e., the following is the input file for the grid of Figure 4, where the numbers
    in the regions correspond to that region's weed distribution value in year 0.

4
4 4 2 2 4 4
3 0 0 0 0 3
3 0 0 0 0 3
3 0 0 0 0 3
3 0 0 0 0 3
5 5 4 4 5 3

Your Program Must (continued):
  Take one command-line argument, which is the number of years to calculate.
  Print the initial density grid, and re-print the grid after each year (indicating
  current year).
  If the densities reach steady-state before the given number of years have been
  calculated, then your program terminates, with an appropriate message, such as:
    "steady-state reached after X years"
  Note that steady-state is reached when EVERY density value in year X+1 is
  unchanged from year X, where "unchanged" means within plus or minus .0001 of 
  the previous year's value.



 /-----|-----|-----|-----|-----|-----\
 |  4  |  4  |  2  |  2  |  4  |  4  |
 |-----|-----|-----|-----|-----|-----|
 |  3  |  0  |  0  |  0  |  0  |  3  |
 |-----|-----|-----|-----|-----|-----|
 |  3  |  0  |  0  |  0  |  0  |  3  |
 |-----|-----|-----|-----|-----|-----|
 |  3  |  0  |  0  |  0  |  0  |  3  |
 |-----|-----|-----|-----|-----|-----|
 |  3  |  0  |  0  |  0  |  0  |  3  |
 |-----|-----|-----|-----|-----|-----|
 |  5  |  5  |  4  |  4  |  5  |  3  |
 \-----------------------------------/
      Figure 4. 
