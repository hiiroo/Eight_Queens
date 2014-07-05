//
//  eight_queens.h
//  Eight_Queens
//
//  Created by Mert on 6/13/14.
//  Copyright (c) 2014 SL. All rights reserved.
//

#ifndef Eight_Queens_eight_queens_h
#define Eight_Queens_eight_queens_h


#define EVALUATEDEBUGMODE 0//Debug mode
#define POPLIMIT 8//Size of points that every parent contains
#define POPULATION_SIZE 10//Size of population
#define TOURNAMENTSIZE 4//size of tournament for selection
#define SUCCESS 0//Defined successful evaluation value
#define EXPECTATION 0//Expectation
#define MATCHRATETHRESHOLD 0//minimum expectation from recombination
#define GENERATIONLIMIT 50000//100000//maximum number of generation will pass
#define MUTATION 1//Enable(1)/Disable(0) Mutation
#define MUTATIONCOEF 5//Number of mutations will happen in loop
#define MUTATIONREMOVEREPLICATION 1//remove replications in positions after mutation operation
#define CROSSOVERREMOVEREPLICATION 1//remove replications in positions after crossover operation
#define ROULTTESIZE 100//size of roulette for probabilistic distribution of parent pointers


typedef struct POS_s {//position for location of queen
    int x;
    int y;
}POS_t[1], *POS;

typedef struct PARENT_s {//parent struct
    int matchrate;
    int size;
    POS *positions;
}PARENT_t[1], *PARENT;

typedef struct POPULATION_s {//population struct
    int size;
    PARENT *pop;
}POPULATION_t[1], *POPULATION;

typedef struct ROULETTE_s{//specified struct for roulette
    int size;
    PARENT* roulette;
}ROULETTE_t[1], *ROULETTE;

typedef struct POOL_s{//pool struct for tournament selection
    int size;
    PARENT* pool;
}POOL_t[1], *POOL;

PARENT init_parent(int size);
POPULATION init_population(int size);
int evaluate(PARENT p);//evaluation function
void mutation(PARENT p);//mutation procedure
void print_parent(PARENT p);//prints specified parent
void remove_replication(PARENT p);//removes replications in parents positions, it is a joker to increase succes
void free_population(POPULATION p);//frees population
void print_population(POPULATION p);//prints all population
void create_random_population(POPULATION p);//creates a population randomly
void population_remove(POPULATION population, PARENT p);//it havent completed just an extra

int population_sum(POPULATION p);//sum of matchrates of parents in population
double population_average(POPULATION p);//population of average
PARENT population_maximum(POPULATION p);//***OLD*** return maximum matchrated parent in population
PARENT population_minimumrate(POPULATION p);//updated one
void environment(POPULATION population);//Main loop of selection and desicion mechanism
POOL init_tournament_pool(POPULATION p);//initiates tournament pool from given population
ROULETTE roulette_array(POPULATION population);//creates roulette array
void tournament_selection(POOL pool, PARENT popfirst, PARENT popsecond);//it is not actively used
void crossover(PARENT p1, PARENT p2, PARENT child1, PARENT child2);//one point crossover returns two childs

#endif
