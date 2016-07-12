#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h> 
#include <assert.h> 


/* functions that has to be implemented */ 
void add_oxygen_atom_to_air(void); 
void * active_carbon_atoms(void *);
 
#define COUNT 10
#define SLEEP_MAX 21 /* MAX time to sleep */
// Note: make sleep large just to make sure there are not 
// enough Oxygen for Carbon threads. 

pthread_mutex_t lock;
int oxygen_atoms = 0; // total O atoms released (bonded + unbonded)
int co2_count; 
int O_inAir = 0; // free O atoms in air

void add_carbon_dioxyde(void)
{ 
  // call us only when there are two Oxygen atoms 
  // assume this is taken care at the higher level 
  // Then we have one more co2 
  // Might run in different threads 
  co2_count ++; 
  
}

void * generate_oxygen(void * dummy)
{
  // Might run in different threads 
  int i;
  for(i=0; i < COUNT*2; i++) { 

    //want to know the number of oxygen atoms created  
    oxygen_atoms ++;

    add_oxygen_atom_to_air();
    
    usleep(rand() % SLEEP_MAX); 
  }
  pthread_exit(NULL);
}


void add_oxygen_atom_to_air()
{
  // this function is called when an oxygen atom is 
  // released into the air

  pthread_mutex_lock(&lock);
  O_inAir++;
  pthread_mutex_unlock(&lock);
}

void * activate_carbon_atoms() 
{
  // there are infinite amounts of active carbon atoms. 
  // They are looking for two oxygen atoms to make stable 
  // carbon dioxide. 
  int type;
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &type);

  while(1) 
  {
    pthread_mutex_lock(&lock);
    if(O_inAir == 2)
    {
      add_carbon_dioxyde();
      O_inAir-=2;
    }
    pthread_mutex_unlock(&lock);
  
  }
  pthread_exit(NULL);
}

#define CTHREADS   20


int main() 
{ 
// init everything 
   pthread_t carbon_threads[CTHREADS]; 
   int i;
   for(i=0; i<CTHREADS; i++) {
    assert(!pthread_create(&carbon_threads[i], NULL, activate_carbon_atoms, NULL));
  }

   pthread_t oxygen_thread;
   assert(!pthread_create(&oxygen_thread, NULL, generate_oxygen, NULL));

   void * dummy; 
   pthread_join(oxygen_thread, &dummy); 
   usleep(100); 

   printf("You code %s\n", co2_count == COUNT ? "Works. Good stuff. Run agin to make sure" : "Does not work. Back to design");

   
   pthread_mutex_destroy(&lock); // destroy mutex
   for(i=0; i<CTHREADS; i++)
     pthread_cancel(carbon_threads[i]); // cancle each carbon_thread
   pthread_exit(NULL);
 
 return 0; 

} 
