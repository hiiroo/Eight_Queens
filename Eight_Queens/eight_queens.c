//  eight_queens.c
//  Eight_Queens
//
//  Created by Mert on 6/13/14.
//  Copyright (c) 2014 SL. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "eight_queens.h"

PARENT init_parent(int size){
    PARENT p = malloc(sizeof(PARENT_t));
    p->matchrate = 0;
    p->size = size;
    p->positions = (POS*)malloc(size * sizeof(POS_t));
    return p;
}

POPULATION init_population(int size){
    
    int i;
    
    POPULATION temp = malloc(sizeof(POPULATION_t));
    temp->size = size;
    temp->pop = malloc(size*sizeof(PARENT));
    
    for (i = 0; i < size; i++) {
        temp->pop[i] = init_parent(POPLIMIT);
    }
    
    return temp;
}

void free_population(POPULATION p){
    int k, popsize = p->size;
    
    for (k = 0; k < popsize; k++) {
        PARENT temp = p->pop[k];
        if (temp != NULL) {
            free(temp->positions);
            free(temp);
        }
    }
    free(p->pop);
    free(p);
}

void create_random_population(POPULATION p){
    
    int i, j;
    
    srand((int)time(NULL));
    
    for (i = 0; i < p->size; i++) {
        PARENT temp = p->pop[i];
        for (j = 0; j < POPLIMIT; j++) {
            temp->positions[j] = malloc(sizeof(POS_t));
            temp->positions[j]->x = rand() % POPLIMIT;
            temp->positions[j]->y = rand() % POPLIMIT;
            if (temp->positions[j]->x == 0) {
                temp->positions[j]->x++;
            }
            if (temp->positions[j]->y == 0){
                temp->positions[j]->y++;
            }
        }
        temp->matchrate = evaluate(temp);
    }
    
}

void remove_replication(PARENT p){
    int mapsize = POPLIMIT + 1;
    int mapx[mapsize];
    int mapy[mapsize];
    int temp1 = 0, temp2 = 0, i;
    int isMissing = 0;
    
    do {
        
        isMissing = 0;
        //for x
        for (i = 0; i < mapsize; i++) {
            mapx[i] = 0;
        }
        
        for (i = 0; i < p->size; i++) {
            mapx[p->positions[i]->x]++;
        }
        
        for (i = 1; i < mapsize; i++) {
            if(mapx[i] == 0){
                temp1 = i;
                isMissing = 1;
            }else if (mapx[i] > 1){
                temp2 = i;
                isMissing = 1;
            }
        }
        
        if (isMissing == 1) {
            for (i = 0; i < p->size; i++) {
                if(p->positions[i]->x == temp2){
                    p->positions[i]->x = temp1;
                    break;
                }
            }
        }
    } while (isMissing == 1);
    
    do{
        
        //for y
        isMissing = 0;
        
        for (i = 0; i < mapsize; i++) {
            mapy[i] = 0;
        }
        
        for (i = 0; i < p->size; i++) {
            mapy[p->positions[i]->y]++;
        }
        
        for (i = 1; i < mapsize; i++) {
            if(mapy[i] == 0){
                temp1 = i;
                isMissing = 1;
            }else if (mapy[i] > 1){
                temp2 = i;
                isMissing = 1;
            }
        }
        
        if (isMissing == 1) {
            for (i = 0; i < p->size; i++) {
                if(p->positions[i]->y == temp2){
                    p->positions[i]->y = temp1;
                    break;
                }
            }
        }
        
    }while (isMissing == 1);
    
}

int evaluate(PARENT p){
    int matchrate = 0;
    int size = p->size, i,k;
    int bitmap[POPLIMIT+1][POPLIMIT+1];
    
    //Zero bitmap
    for(i = 0; i < (POPLIMIT+1); i++){
    	for(k = 0; k < (POPLIMIT+1); k++){
    		bitmap[i][k] = 0;
    	}
    }
    //
    
    //Fill bitmap
    for(k = 0; k < size; k++){
    	bitmap[p->positions[k]->x][p->positions[k]->y] = 1;
    }
    //
    
    if(EVALUATEDEBUGMODE == 1){
        //Print bitmap
    	for(i = 1; i <= size; i++) {
    		for(k = 1; k <= size; k++){
    			printf("%d ", bitmap[i][k]);
    		}
    		printf("\n");
    	}
    	//
    }
    
    //Check horizontal and vertical axis for collisions
    int bincnt = 0;
    
    for(i = 0; i < size ; i++){
    	for(k = 0; k < size; k++){
    		bincnt = bincnt + bitmap[i][k];
    	}
    	if(bincnt > 1){
    		if(EVALUATEDEBUGMODE == 1){
    			printf("Collision at line of %d,%d\n", i,k);
    		}
    		matchrate++;
    	}
        //    	else if(bincnt < 1){
        //    		matchrate++;
        //    	}
    	bincnt = 0;
    }
    //
    
    bincnt = 0;
    int x,y;
    for(x = POPLIMIT; x >= 1; x--){
    	int tempx = x;
    	int tempy = 1;
    	for(y = x; y <= POPLIMIT; y++){
    		if(bitmap[tempx][tempy] == 1){
    			bincnt++;
    		}
    		if(EVALUATEDEBUGMODE == 1){
    			printf("Debug x:%d, y:%d\n", tempx, tempy);
    		}
    		tempx++;
    		tempy++;
    	}
    	if(bincnt > 1){
    		if(EVALUATEDEBUGMODE == 1){
    			printf("Collision at line of %d,%d\n", tempx,tempy);
    		}
    		matchrate++;
    	}
    	bincnt = 0;
    }
    
    //other half
    bincnt = 0;
    for(y = 2; y <= POPLIMIT; y++){
    	int tempx = 1;
    	int tempy = y;
    	for(x = y; x <= POPLIMIT; x++){
    		if(bitmap[tempx][tempy] == 1){
    			bincnt++;
        	}
    		if(EVALUATEDEBUGMODE == 1){
    			printf("Debug x:%d, y:%d\n", tempx, tempy);
    		}
    		tempx++;
    		tempy++;
    	}
    	if(bincnt > 1){
    		if(EVALUATEDEBUGMODE == 1){
    			printf("Collision at line of %d,%d\n", tempx,tempy);
    		}
    		matchrate++;
    	}
    	bincnt = 0;
    }
    
    //*************************
    
    bincnt = 0;
    for(y = 1; y <= POPLIMIT; y++){
    	int tempx = 1;
    	int tempy = y;
    	for(x = 1; x <= y; x++){
    		if(bitmap[tempx][tempy] == 1){
    			bincnt++;
    		}
    		if(EVALUATEDEBUGMODE == 1){
    			printf("Debug x:%d, y:%d\n", tempx, tempy);
    		}
    		tempx++;
    		tempy--;
    	}
    	if(bincnt > 1){
    		if(EVALUATEDEBUGMODE == 1){
    			printf("Collision at line of %d,%d\n", tempx,tempy);
    		}
    		matchrate++;
    	}
    	bincnt = 0;
    }
    
    bincnt = 0;
    for(x = POPLIMIT; x >= 1; x--){
    	int tempx = x;
    	int tempy = 8;
    	for(y = x; y <= POPLIMIT; y++){
    		if(bitmap[tempx][tempy] == 1){
    			bincnt++;
    		}
    		if(EVALUATEDEBUGMODE == 1){
    			printf("Debug x:%d, y:%d\n", tempx, tempy);
    		}
    		tempx++;
            tempy--;
    	}
    	if(bincnt > 1){
    		if(EVALUATEDEBUGMODE == 1){
    			printf("Collision at line of %d,%d\n", tempx,tempy);
    		}
    		matchrate++;
    	}
    	bincnt = 0;
    }
    
    
    
    return matchrate;
}

void print_parent(PARENT p){
    int k;
    for (k = 0; k < p->size; k++) {
        
        POS temp = p->positions[k];
        if (temp != NULL) {
            printf("%d - ", temp->x);
            printf("%d\n", temp->y);
        }
        
    }
    printf("MATCH RATE :%d\n", evaluate(p));
}

void print_population(POPULATION p){
    int i;
    
    for (i = 0; i < p->size; i++) {
        printf("Parent %d\n", i);
        print_parent(p->pop[i]);
    }
}

int population_sum(POPULATION p){
    int i = 0, _sum = 0;
    for (i = 0; i < p->size; i++) {
        _sum = _sum + p->pop[i]->matchrate;
    }
    return _sum;
}

double population_average(POPULATION p){
    
    int sum = 0;
    int size = p->size, i;
    PARENT tpar;
    
    for (i = 0; i< size; i++) {
        tpar = p->pop[i];
        sum = sum + tpar->matchrate;
    }
    
    return (sum/size);
}

PARENT population_maximum(POPULATION p){
    int max, i;
    PARENT tpar = p->pop[0];
    max = tpar->matchrate;
    
    for (i = 0; i < p->size; i++) {
        if (p->pop[i]->matchrate > max) {
            max = p->pop[i]->matchrate;
            tpar = p->pop[i];
        }
        
    }
    return tpar;
}

PARENT population_minimumrate(POPULATION p){
    int min, i;
    PARENT tpar = p->pop[0];
    min = tpar->matchrate;
    
    for (i = 0; i < p->size; i++) {
        if (p->pop[i]->matchrate < min) {
            min = p->pop[i]->matchrate;
            tpar = p->pop[i];
        }
        
    }
    return tpar;
}

void population_remove(POPULATION population, PARENT p){
    int i;
    for (i = 0; i < population->size; i++) {
        if(population->pop[i]->positions == p->positions){
            population->pop[i] = NULL;
        }
    }
}//incomplete

void mutation(PARENT p){
    
    srand((int)time(NULL));
    
    int psize = p->size, i, val1, val2;
    for (i = 0; i < psize*MUTATIONCOEF; i++) {
        
        val1 = (rand() % POPLIMIT);
        val2 = (rand() % POPLIMIT);
        
        if(val1 == 0){
            val1++;
        }
        if (val2 == 0) {
            val2++;
        }
        
        p->positions[(rand() % p->size)]->x = val1;
        p->positions[(rand() % p->size)]->y = val2;
    }
    
    if(MUTATIONREMOVEREPLICATION == 1){
    	remove_replication(p);
    }
    p->matchrate = evaluate(p);
}

void crossover(PARENT p1, PARENT p2, PARENT child1, PARENT child2){
    
    srand((int)time(NULL));
    
    int cut, i, size = p1->size;
    
    cut = rand() % size;
    
    
    for (i = 0; i < cut; i++) {
        child1->positions[i] = p1->positions[i];
    }
    
    for (i = cut; i < size; i++) {
        child1->positions[i] = p2->positions[i];
    }
    
    for (i = 0; i < cut; i++) {
        child2->positions[i] = p2->positions[i];
    }
    
    for (i = cut; i < size; i++) {
        child2->positions[i] = p1->positions[i];
    }
    
    if(CROSSOVERREMOVEREPLICATION == 1){
    	remove_replication(child1);
    	remove_replication(child2);
    }
    
    child1->matchrate = evaluate(child1);
    child2->matchrate = evaluate(child2);
}

void environment(POPULATION population){
    
    int gencount = 0, i, popaverage = 0, success = 0, MIN = MATCHRATETHRESHOLD;
    popaverage = population_average(population);
    
    srand((int)time(NULL));
    
    while ((success != 1) && (popaverage > EXPECTATION) && (gencount <= GENERATIONLIMIT)) {
        
        POPULATION temppop = init_population(POPULATION_SIZE);
        
        for (i = 0; i < population->size -1 ; i++) {
            
            POOL tournamentpool = init_tournament_pool(population);
            
            PARENT tpar1 = NULL;
            PARENT tpar2 = NULL;
            PARENT tempmax = NULL;
            //Tournament Selection Part
            tempmax = population_maximum((POPULATION)tournamentpool);
            
            tpar1 = population_minimumrate((POPULATION)tournamentpool);
            
            int p, min = 0;
            for (p = 0; p < tournamentpool->size; p++) {
                if((tournamentpool->pool[p]->matchrate >= min) && (tournamentpool->pool[p]->positions != tpar1->positions)){//I will continue later
                    tpar2 = tournamentpool->pool[p];
                }
            }
            //end
            
            PARENT child1 = NULL;
            PARENT child2 = NULL;
            
            child1 = init_parent(POPLIMIT);
            child2 = init_parent(POPLIMIT);
            
            crossover(tpar1, tpar2, child1, child2);
            
            int rate1 = child1->matchrate;
            int rate2 = child2->matchrate;
            
            if (rate1 < MIN){
                MIN = rate1;
                print_parent(child1);
                break;
            }else{
            	if(MUTATION == 1){
            		mutation(child1);
            	}
            }
            
            if (rate2 < MIN) {
                MIN = rate2;
                print_parent(child2);
                break;
            }else{
            	if(MUTATION == 1){
            		mutation(child2);
            	}
            }
            
            if(child1->matchrate == SUCCESS){
                success = 1;
                printf("Child 1 - Solution found.\n");
                print_parent(child1);
                break;
            }else if(child2->matchrate == SUCCESS){
                success = 1;
                printf("Child 2 - Solution found.\n");
                print_parent(child2);
                break;
            }
            
            temppop->pop[i] = child1;
            temppop->pop[i+1] = child2;
        }
        popaverage = population_average(temppop);
        
        //        printf("Population Maximum:%d\n", population_minimumrate(population)->matchrate);
        printf("GENERATION: %d - POPULATION AVERAGE: %f\n", gencount,population_average(population));
        
        free_population(population);
        population = temppop;
        gencount++;
    }
    printf("Population maximum\n");
    print_parent(population_maximum(population));//Means best of population
    
}

POOL init_tournament_pool(POPULATION p){
    srand((int)time(NULL));
    
    POOL temp = (POOL)init_population(TOURNAMENTSIZE);
    
    int i;
    
    for (i = 0; i < temp->size; i++) {
        temp->pool[i] = p->pop[(rand() % p->size)];
    }
    return temp;
}

ROULETTE roulette_array(POPULATION population){
    int _localcnt = 0, i, j, _elementsize = 0;
    double matchrate = 0, _population_sum = 0;
    ROULETTE roulette = (ROULETTE)init_parent(ROULTTESIZE);
    roulette->size = ROULTTESIZE;
    _population_sum = population_sum(population);
    
    for (i = 0; i < population->size; i++) {
        matchrate = population->pop[i]->matchrate;
        _elementsize = ceil((matchrate*100)/_population_sum);
        for (j = 0; j < _elementsize; j++) {
            roulette->roulette[_localcnt] = population->pop[i];
            _localcnt++;
        }
    }
    return roulette;
}
