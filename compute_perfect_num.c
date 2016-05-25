/*************************************************************************/
/*Program     : compute_perfect_num.c                                    */
/*Date        : 10/16/2014                                               */
/*Author      : Sreenidhi Krishna                                        */
/*Description : This program takes in an integer N and number of threads */ 
/*              as user input and verifies whether the sum of factors of */
/*              N, excluding N adds up to the number                     */   
/*************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int sqrtN,N,NThreads;
int mysum;
pthread_attr_t attr;
pthread_mutex_t mutexsum;

/*Function called by each thread to evaluate the factors*/
void * Factorize(void *arg)
{
     int i;
     long tid;
     tid = (long) arg;
     
     for(i=tid+2;i<=sqrtN;i=i+NThreads)
     {
          if((N%i)==0)
          {
               /* Mutex lock to update sum of factors */
               pthread_mutex_lock (&mutexsum);
               mysum += i+N/i;
               pthread_mutex_unlock (&mutexsum);  
               printf(" %d %d ",i,N/i);             
          }
     }
     pthread_exit((void*) 0);
}

int main(int argc,char **arv)
{
     int rc;
     long i;
     void *status;
     printf("Enter an integer and total number of threads you want to use\n");
     scanf("%d %d",&N,&NThreads);
     while(N<2)
     {
          printf("Error !! Enter a number greater than 1\n");
          scanf("%d",&N);
     }
     sqrtN = sqrt(N);
     if(NThreads>=sqrtN||NThreads<=0)
     {
          NThreads = sqrtN-1;
          printf("Warning!! Excess/Insufficient number of threads, modifying number of threads to %d\n",NThreads);
     }
     
     /* Initialize thread variables and attributes */
     pthread_t threads[NThreads];
     pthread_attr_init(&attr);
     pthread_mutex_init(&mutexsum,NULL);       
     pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE); 

     mysum = 1;
     
     printf("Factors of number %d are %d %d ",N,1,N);

     /* Create threads and call function to evaluate factors */
     for(i=0; i<NThreads; i++)
     {
          rc = pthread_create(&threads[i], &attr, Factorize, (void *)i);
          if (rc)
          {
               printf("ERROR; return code from pthread_create() is %d\n", rc);
               exit(-1);
          }
      }


 	pthread_attr_destroy(&attr);

     /* Wait for all threads to join main thread */
	for(i=0; i<NThreads; i++)
     {
          pthread_join(threads[i], &status);
	}

     /* After joining, print out the results and cleanup */
     if(mysum==N)
     {
          printf ("\nCongratulations !! Number %d is a perfect number\n", N);
     }
     else
     {
          printf ("\nSorry !! Number %d is NOT a perfect number\n", N);
     }
     

     pthread_mutex_destroy(&mutexsum);
     pthread_exit(NULL);        
     return 0;
}

