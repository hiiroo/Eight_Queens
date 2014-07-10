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

POS* init_positions(int size){
    POS* temp = malloc(size * sizeof(POS_t));
    
    int i;
    for (i = 0; i < size; i++) {
        temp[i] = malloc(sizeof(POS_t));
    }
    
    return temp;
}

PARENT init_parent(int size){
    PARENT p = malloc(sizeof(PARENT_t));
    p->matchrate = 0;
    p->size = size;
    p->positions = init_positions(size);
    return p;
}

POPULATION init_population(int size){
    
    int i;
    
    POPULATION temp = malloc(sizeof(POPULATION_t));
    temp->size = size;
    temp->pop = malloc(size*sizeof(PARENT_t));
    
    for (i = 0; i < size; i++) {
        temp->pop[i] = init_parent(POPLIMIT);
    }
    
    return temp;
}

PARENT copy_parent(PARENT p1){
    
    int i;
    
    PARENT p2 = malloc(sizeof(PARENT_t));
    p2->size = POPLIMIT;
    p2->positions = malloc(POPLIMIT*sizeof(POS_t));
    
    for (i = 0; i < POPLIMIT; i++) {
        p2->positions[i] = malloc(sizeof(POS_t));
        p2->positions[i]->x = p1->positions[i]->x;
        p2->positions[i]->y = p1->positions[i]->y;
        
        if(p2->positions[i]->x == 0 && p2->positions[i]->y == 0){
        	printf("Anomally Detected at %d\n", i);
        	print_parent(p1);
        }
        
    }
    
    p2->matchrate = evaluate(p2);
    
    
    return p2;
}

void free_population(POPULATION p){
    int k, popsize = p->size;
    
    for (k = 0; k < popsize; k++) {
        PARENT temp = p->pop[k];
        if (sizeof(temp) >= sizeof(PARENT_t)) {
            free(temp->positions);
            free(temp);
        }
    }
    if(sizeof(p) >= sizeof(POPULATION_t)){
        free(p->pop);
        free(p);
    }
}

void create_random_population(POPULATION p){
    
    int i, j;
    
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
    	if(bitmap[p->positions[k]->x][p->positions[k]->y] > 0){
    		matchrate = matchrate + bitmap[p->positions[k]->x][p->positions[k]->y];
    	}else{
    		bitmap[p->positions[k]->x][p->positions[k]->y] = 1;
    	}
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
    
    for(i = 1; i < size ; i++){
    	for(k = 1; k < size; k++){
    		bincnt = bincnt + bitmap[i][k];
    	}
    	if(bincnt > 1){
    		if(EVALUATEDEBUGMODE == 1){
    			printf("Collision at line of %d,%d\n", i,k);
    		}
    		matchrate++;
    	}
        else if(bincnt < 1){
        	matchrate++;
        }
    	bincnt = 0;
    }
    
    for(i = 1; i < size ; i++){
    	for(k = 1; k < size; k++){
    		bincnt = bincnt + bitmap[k][i];
    	}
    	if(bincnt > 1){
    		if(EVALUATEDEBUGMODE == 1){
    			printf("Collision at line of %d,%d\n", i,k);
    		}
    		matchrate++;
    	}
    	else if(bincnt < 1){
    		matchrate++;
    	}
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
    
    int psize = p->size, i, val1, val2;
    for (i = 0; i < psize*MUTATIONRATE; i++) {
        
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
    
    int cut, i, size = p1->size;
    double localcrsvrrate = rand() % 100;
    localcrsvrrate = localcrsvrrate / 100;
    
    if (CROSSOVERRATE > localcrsvrrate) {
        
        cut = rand() % size;
        
        for (i = 0; i < cut; i++) {
            child1->positions[i] = malloc(sizeof(POS_t));
            POS temp = child1->positions[i];
            
            temp->x = p1->positions[i]->x;
            temp->y = p1->positions[i]->y;
        }
        
        for (i = cut; i < size; i++) {
            
            child1->positions[i] = malloc(sizeof(POS_t));
            POS temp = child1->positions[i];
            
            temp->x = p2->positions[i]->x;
            temp->y = p2->positions[i]->y;
            
        }
        
        for (i = 0; i < cut; i++) {
            
            child2->positions[i] = malloc(sizeof(POS_t));
            POS temp = child2->positions[i];
            
            temp->x = p2->positions[i]->x;
            temp->y = p2->positions[i]->y;
            
        }
        
        for (i = cut; i < size; i++) {
            
            child2->positions[i] = malloc(sizeof(POS_t));                
            POS temp = child2->positions[i];
            
            temp->x = p1->positions[i]->x;
            temp->y = p1->positions[i]->y;
            
        }
        
    }else{
        int i;
        
        for (i = 0; i < child1->size; i++) {
            child1->positions[i] = p1->positions[i];
            child2->positions[i] = p2->positions[i];
        }
        
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
    
    while ((success != 1) && (popaverage > EXPECTATION) && (gencount <= GENERATIONLIMIT)) {
        
        POPULATION temppop = init_population(POPULATION_SIZE);
        
        for (i = 0; i < population->size; i = i + 2) {
            
            //            print_population(population);
            
            POPULATION tournamentpool = init_tournament_pool(population);
            
            //            printf("TOURNAMENT POOL\n");
            //            print_population(tournamentpool);
            
            PARENT tpar1 = NULL;
            PARENT tpar2 = NULL;
            
            tpar1 = population_minimumrate(tournamentpool);
            tpar2 = population_maximum(tournamentpool);
            
            int p, min = 0;
            for (p = 0; p < tournamentpool->size; p++) {
                if((tournamentpool->pop[p]->matchrate >= min) && (tournamentpool->pop[p]->positions != tpar1->positions)){//I will continue later
                    tpar2 = tournamentpool->pop[p];
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
            
            if (rate1 <= MIN){//RATING
                
                if (HILLCLIMB == 1) {
                    
                    PARENT temp = copy_parent(child1);
                    
                    remove_replication(temp);
                    
                    if((temp->matchrate < rate1) || (temp == SUCCESS)){
                        success = 1;
                        printf("Child 1 - Solution found by hillclimb.\n");
                        print_parent(temp);
                        
                        free(temp->positions);
                        free(temp);
                        
                        break;
                    }
                }
                
                MIN = rate1;
                
            }else{
            	if(MUTATION == 1){
            		mutation(child1);
            	}
            }//END
            
            if (rate2 <= MIN) {//RATING
                
                if (HILLCLIMB == 1) {
                    
                    PARENT temp = copy_parent(child2);//init_parent(POPLIMIT);
                    
                    remove_replication(temp);
                    
                    if((temp->matchrate < rate2) || (temp == SUCCESS)){
                        success = 1;
                        printf("Child 2 - Solution found by hillclimb.\n");
                        print_parent(temp);
                        
                        free(temp->positions);
                        free(temp);
                        
                        break;
                    }
                }
                
                MIN = rate2;
                
            }else{
            	if(MUTATION == 1){
            		mutation(child2);
            	}
            }//END
            
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
        
        printf("GENERATION: %d - POPULATION AVERAGE: %f\n", gencount,population_average(population));
        
        free_population(population);
        population = temppop;
        gencount++;
    }
    printf("Population maximum\n");
    print_parent(population_maximum(population));//Means best of population
    
}

POPULATION init_tournament_pool(POPULATION p){
    
    POPULATION temp = malloc(sizeof(POPULATION_t));
    temp->size = TOURNAMENTSIZE;
    temp->pop = malloc(TOURNAMENTSIZE*sizeof(PARENT_t));
    
    int i;
    int selectionStart;// = rand() % POPLIMIT;
    
    for (i = 0; i < TOURNAMENTSIZE; i++) {
        selectionStart = rand() % (POPULATION_SIZE);
        temp->pop[i] = copy_parent(p->pop[selectionStart]);
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
