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
int oxygen_atoms = 0; 
int co2_count; 
int CO_present = 0;

/* You would need synch primitives 
 * Use them wisely
 */ 


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

    add_oxygen_atom_to_air(); // need to implement this below.
    
    usleep(rand() % SLEEP_MAX); 
  }
}


void add_oxygen_atom_to_air()
{
  // this function is called when an oxygen atom is 
  // released into the air. Before this I have a count
  // but now I need a suitable primitive so that C atoms
  // can bound with these.
  // What is a suitable data structure here? 
  // or may be a synch prmitive?
}

void * activate_carbon_atoms() 
{
  // there are infinite amounts of active carbon atoms. 
  // They are looking for two oxygen atoms to make stable 
  // carbon dioxide. 

  while(1) {
    pthread_mutex_lock(&lock);
    if(oxygen_atoms == 2)
    {
      add_carbon_dioxyde();
      oxygen_atoms-=2;
    }
    pthread_mutex_unlock(&lock);

  }
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

   // This bit is a real hack. I wait for the oxygen thread  
   // once it exits, give some time for carbon threads to create CO2
   // finally I check the co2_count; it should be COUNT 
   // if not there is some issue 
   void * dummy; 
   pthread_join(oxygen_thread, &dummy); 
   usleep(100); 

   printf("You code %s\n", co2_count == COUNT ? "Works. Good stuff. Run agin to make sure" : "Does not work. Back to design");
// Clean up. 
// Implement the clean up code. 
 return 0; 

} 
