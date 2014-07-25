//
//  main.c
//  Eight_Queens
//  
//  Created by Mert on 6/13/14.
//  Copyright (c) 2014 SL. All rights reserved.
// 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "eight_queens.h"

int main()
{
    srand((int)time(NULL));
    
    POPULATION population = init_population(POPULATION_SIZE);
    
    create_random_population(population);
    
    environment(population);
    
    /*
     int i;
     int ref[8][2] = {{8,4}, {7,7}, {6,3}, {5,8}, {4,2}, {3,5}, {2,1}, {1, 6}};
     int ref[8][2] = {{8,6}, {7,4}, {6,7}, {5,1}, {4,8}, {3,2}, {2,5}, {1, 3}};
     int ref[8][2] = {{8,5}, {7,7}, {6,1}, {5,3}, {4,8}, {3,6}, {2,4}, {1, 2}};
     
     PARENT test = init_parent(POPLIMIT);
     
     for (i = 0; i < test->size; i++) {
     test->positions[i] = malloc(sizeof(POS_t));
     test->positions[i]->x = ref[i][0];
     test->positions[i]->y = ref[i][1];
     }
     
     printf("%d\n", evaluate(test));
     */
    
    free_population(population);
    return 0;
}
