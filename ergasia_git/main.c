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
//function to helps me cancel the right order
void cancelation_id(int id){
    cancel_id=id;
}
//sales function,helps me count the summary of plain,special pizzas and the profit

void sales(int pizzas[],int pizza){
    for (int i=0; i<pizza; i++) {
        if(pizzas[i]==0){
            sum_of_plainpizzas++;
            total_profit +=Cplain;
        }else{
            sum_of_specialpizzas++;
            total_profit +=Cspecial;
        }
        
    }
}

void delivery(int id){
    while(Ndeliverer<1){
        printf("No delivery guy available\n");
        pthread_cond_wait(&cond, &lock);
    }
    pthread_mutex_unlock(&lock);
    Ndeliverer--;
    printf("DELIVERY ORDER %d \n",id);
    unsigned int delivery_time=rand_r(random_number_seed())%Tdelhigh+Tdellow;
    sleep(delivery_time*2);
    pthread_mutex_lock(&lock);
    
    Ndeliverer++;
    
    
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);
    clock_gettime(CLOCK_REALTIME, &finish_time2);
    double z  = (finish_time2.tv_sec - start_time2.tv_sec) ;
    
    printf("Order with number %d delivered in %f minutes \n",id,z);
    clock_gettime(CLOCK_REALTIME, &cold_time_finish);
    order_time_sum=order_time_sum+z;
    if(order_max_time<z){
        order_max_time=order_time_sum;
    }
    double z2=(cold_time_finish.tv_sec - cold_time_start.tv_sec) ;
    cold_time_sum=cold_time_sum+z2;
    if(cold_max_time<=z2){
        cold_max_time=cold_time_sum;
    }
}
void packing(int id,int pizzas){
    
    while(Npacker<1){
        printf("Not enough packers..please wait \n");
        pthread_cond_wait(&cond, &lock);
    }
    pthread_mutex_unlock(&lock);
    Npacker--;
    
    
    for(int i=0;i<pizzas;i++){
        sleep(Tpack);
    }
    pthread_mutex_lock(&lock);
    
    Npacker++;
    
   
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);
    
    clock_gettime(CLOCK_REALTIME, &finish_time1);
    double z  = (finish_time1.tv_sec - start_time1.tv_sec);

    printf("Order with number %d got ready in %f minutes \n",id,z);
    
    delivery(id);
    
}




void baking(int pizzas,int id){
    while(Noven<pizzas){
        printf("Not enough ovens..please wait \n");
        pthread_cond_wait(&cond, &lock);
    }
    pthread_mutex_unlock(&lock);
    Noven = Noven-pizzas;
    printf("Baking started for order %d,diathesimoi fournoi %d\n",id,Noven);
    sleep(Tbake);
    pthread_mutex_lock(&lock);
    
    Noven = Noven+pizzas;
    
   
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);
    clock_gettime(CLOCK_REALTIME, &cold_time_start);
    packing(id,pizzas);
   
}

void preparation(int pizzas,int id){
    while(Ncook<1){
        printf("Not enough cooks..please wait \n");
        pthread_cond_wait(&cond, &lock);
    }
    pthread_mutex_unlock(&lock);
    
    Ncook --;
    
    printf("Preparation started for the order\n");
    for(int i=0;i<pizzas;i++){
        sleep(Tprep);
        printf("number %d pizza prepared for order %d \n",i+1,id);
    }
    pthread_mutex_lock(&lock);
    
    Ncook++;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);
    baking(pizzas,id);
}





void *order(void *x){
    
    
    int id_nhma=*(int *)x;
    printf("Order  %d just started \n",id_nhma);
    pthread_mutex_lock(&lock);
    // Each time a new customer(thread) starts ordering, we use a new probability to which type each pizza will be
    //thats why i created a function called random_number_seed
    unsigned int random_number_pizza=rand_r(random_number_seed())%Norderhigh+Norderlow;
    int pizzas[random_number_pizza];
    
    for (int i=0; i<random_number_pizza; i++) {
        
        float possibility_plain=(float)rand_r(random_number_seed())/RAND_MAX;
        //based on what i was given from my university,i created one if,because if the random possibility created is smaller or equal to 0.6 the pizza is plain and if not is special
        if(possibility_plain<=0.6){
            pizzas[i]=0;//plain
            
            
        }else{
            pizzas[i]=1;//special
            
        }
        
        
    }
    int paying_time=rand_r(random_number_seed())%Torderlow+Torderhigh;//I used rand_r to create a random amount of time to wait till the client uses his card
    sleep(paying_time);
    //then i used another possibility to find if he had enough money for the order
    float possibility_failed=(float)rand_r(random_number_seed())/RAND_MAX;
    
    if(possibility_failed<=0.1){
        //if he didnt have enough money,the order is failed and we print a message
        printf("Order with number %d failed\n",id_nhma);
        unsuccesfull_orders++;
        cancelation_id(id_nhma);
        pthread_mutex_unlock(&lock);
        
        
    }else{
        //if the client had enough money,then the order is sumbitted
        printf("Order with number %d submitted\n",id_nhma);
        succesfull_orders++;
        sales(pizzas,random_number_pizza);
        
    }
    pthread_mutex_unlock(&lock);
    preparation(random_number_pizza,id_nhma);

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
        clock_gettime(CLOCK_REALTIME, &start_time1);
        clock_gettime(CLOCK_REALTIME, &start_time2);
     
        resources=pthread_create(&threads[i], NULL, order, &id[i]);
        int random_sleep=rand_r(&random1)%Torderhigh+Torderlow;
        sleep(random_sleep);
        
        if(cancel_id!=0){
            pthread_cancel(threads[cancel_id-1]);
            printf("cancelled %d \n",cancel_id);
            cancel_id=0;
            
        }
    }
    for (int i = 0; i < Ncust; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Total profit: %d euros\n",total_profit);
    printf("Total plain pizzas sold: %d\n",sum_of_plainpizzas);
    printf("Total special pizzas sold: %d\n",sum_of_specialpizzas);
    printf("Successfull orders: %d\n",succesfull_orders);
    printf("Unsuccessfull orders: %d\n",unsuccesfull_orders);
    printf("Average customer service time:  %d \n",order_time_sum/succesfull_orders);
    printf("Max customer service  time:  %d \n",order_max_time);
    printf("Average cold time:  %d \n",cold_time_sum/succesfull_orders);
    printf("Max cold time:  %d \n",cold_max_time);
    
    pthread_cancel(threads[Ncust]);
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
    return 0;
}
