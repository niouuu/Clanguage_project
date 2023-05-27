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

void *order(void *x){
    
    
    int id_nhma=*(int *)x;
    printf("Order  %d just started \n",id_nhma);
    pthread_mutex_lock(&lock);
    
    
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
