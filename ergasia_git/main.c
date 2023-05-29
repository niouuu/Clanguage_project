//
//  main.c
//  ergasia_git
//
//  Created by NIKOS ANTWNIOU on 26/5/23.
//

#include <stdio.h>
#include <pthread.h>
#include "metavlhtes.h"
#include <stdlib.h>
#include<unistd.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
unsigned int* random_number_seed(void){
   return &random1;
}

void *order(void *x){
    
    
    int id_nhma=*(int *)x;
    printf("Order  %d just started \n",id_nhma);
    pthread_mutex_lock(&lock);
    // Each time a new customer(thread) starts ordering, we use a new probability to which type each pizza will be
    //thats why i created a function called random_number_seed
    unsigned int random_number_pizza=rand_r(random_number_seed())%Norderhigh+Norderlow;
    int pizzas[random_number_pizza];
    //analoga me to poses pitses thelei,briskoume poies einai aples kai poies special
    for (int i=0; i<random_number_pizza; i++) {
        
        float possibility_plain=(float)rand_r(random_number_seed())/RAND_MAX;
        //based on what i was given from my university,i created one if,because if the random possibility created is smaller or equal to 0.6 the pizza is plain and if not is special
        if(possibility_plain<=0.6){
            pizzas[i]=0;//plain
            
            
        }else{
            pizzas[i]=1;//special
            
        }
        
        
    }
    
    return 0;
}

int main(int argc, const char * argv[]){
    // insert code here...
    
    int resources;
    //Customers and random initialization
    Ncust=atoi(argv[1]);
    random1=atoi(argv[2]);
   

    pthread_t threads[Ncust];//Array for threads used for customers
    pthread_mutex_init(&lock, NULL);//initialize mutex
    pthread_cond_init(&cond, NULL);//initialize condition
    int id[Ncust];//Array for orders id

    
    for (int i=0; i<Ncust;i++) {
        id[i]=i+1;
        printf("Thread with id %d, created\n",i+1);
        

        resources=pthread_create(&threads[i], NULL, order, &id[i]);//thread creation and order function called through pthread_create
        
        

    }
    for (int i = 0; i < Ncust; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}
