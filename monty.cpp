/*
 * monty.c
 *
 *  Created on: Dec 7, 2013
 *      Author: x
 */


#include "monty.h"
using namespace std;
/* @tparam __w  Word size, the number of bits in each element of
*              the state vector.
* @tparam __n  The degree of recursion.
* @tparam __m  The period parameter.
* @tparam __r  The separation point bit index.
* @tparam __a  The last row of the twist matrix.
* @tparam __u  The first right-shift tempering matrix parameter.
* @tparam __d  The first right-shift tempering matrix mask.
* @tparam __s  The first left-shift tempering matrix parameter.
* @tparam __b  The first left-shift tempering matrix mask.
* @tparam __t  The second left-shift tempering matrix parameter.
* @tparam __c  The second left-shift tempering matrix mask.
* @tparam __l  The second right-shift tempering matrix parameter.
* @tparam __f  Initialization multiplier. */
void omploop(LD64 cnt)
{
	LD64 change = 0;
	LD64 stay = 0;
	LD64 counter = (ceil(cnt/2));
	unsigned seeder = lrand48() * (1 + drand48());
	random_device gen;
	uniform_int_distribution<int> tri(0,2);
	uniform_int_distribution<int> bi(0,1);
	int goat = 0, notgoat = 0, door = 0, win = 0, i = 0;
	omp_set_dynamic(0);
	omp_set_nested(1);
	omp_set_max_active_levels(counter*4*2);
	printf("Calculating %.0Lf (stay) with %d threads\n", counter, omp_get_num_procs() * 4 + 1);
	omp_set_num_threads(omp_get_num_procs() * 4 + 1);
	printf("%d\n", omp_get_num_threads());
	printf ("Outer: max_act_lev=%d, num_thds=%d, max_thds=%d\n",
omp_get_max_active_levels(), omp_get_num_threads(),
omp_get_max_threads());	
double ptime = omp_get_wtime();
#pragma omp parallel for firstprivate(tri, gen) shared(counter) reduction(+:win) nowait
	for(i=0; i<counter; i++)
	{
		win += (tri(gen) == tri(gen));
	}
	ptime = fdim(omp_get_wtime() , ptime);
	stay = win / counter;
	int pwin = win;
	printf("Calculating %.0Lf(change)\n", counter);
	win = 0;
	omp_set_num_threads(omp_get_num_procs() * 4 + 1);
	double btime = omp_get_wtime();
#pragma omp parallel for firstprivate(tri, gen) shared(counter) reduction(+:win)
	for(i = 0; i < counter; ++i)
	{
		int sw = tri(gen) - tri(gen);
		win += (abs(sw) && sw);
//		printf("goat %d\tdoor%d\t win? %d\n",goat, door, win > c);
	}
	btime = fdim(omp_get_wtime() , btime);
	change = win / counter;
	printf("Total iterations -> %Lf\n\n", cnt);
	printf("Timimg stay(%f) (cha(%f) |[%f] %f\n\n", ptime, btime, fdim(ptime,btime), (fmax(ptime,btime) - fdim(ptime, btime))/fmax(ptime, btime));
	printf("Getting a goat by staying ->  (%d) %Lf\n", pwin, stay);
	printf("Getting a goat by changing -> (%d) %Lf\n", win, change);
	printf("Getting a goat by comparison -> s(%Lf) | c(%d)", (pwin+win)/(counter*2), stay/change);
}

int main(int argc, char **argv)
{
  LD64 games = 0;
  	if(argc > 1)
  	{
  		char *n = NULL;
  		games = ceil(abs(strtold(argv[1], NULL)));
  	}
  	if(!games)
  		games = UINT_MAX;
  	omploop(games);

//  std::cout << "staying: " << 100.0*wins_stay/games << "%, changing: " << 100.0*wins_change/games << "%\n";
  	return 0;
}
