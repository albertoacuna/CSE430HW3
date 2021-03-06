
//Define semaphores, mutexes, etc here
/* */
#include <pthread.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <time.h>
#include <math.h>
#include <time.h>

#define FCOUNT 2
#define MCOUNT 2
#define RR_CAP 2

#define RR_MAXSLEEP 10
#define OTHER_SLEEP 10


void *thread_fmain(void *);
void *thread_mmain(void *);
void man_leave();
void man_enter();
void woman_leave();
void woman_enter();
void use_rr();
void do_other_stuff();
int get_simple_tid(pthread_t);
int maleCount = 0;
int femaleCount=0;

pthread_t threadIDs[FCOUNT+MCOUNT];

//Define semaphores, mutexes, etc here
/* */
sem_t male_semaphore;
sem_t female_semaphore;
pthread_mutex_t male_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t female_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t male_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t female_full = PTHREAD_COND_INITIALIZER;

int main()
{
   pthread_attr_t attribs;
   int i;
   int tids[FCOUNT+MCOUNT];
   pthread_t pthreadids[FCOUNT+MCOUNT];

   //Initialize semaphores, mutexes etc here.
   /* */
   sem_init(&male_semaphore, 0, RR_CAP);
   sem_init(&female_semaphore, 0, RR_CAP);
   pthread_mutex_init(&male_mutex, 0);
   pthread_mutex_init(&female_mutex, 0);

   srandom(time(NULL) % (unsigned int) RAND_MAX);

   pthread_attr_init(&attribs);
   for (i=0; i< FCOUNT; i++)
   {
       tids[i] = i;
       pthread_create(&(pthreadids[i]), &attribs, thread_fmain, &(tids[i]));
   }

   for (i=0; i< MCOUNT; i++)
   {
       tids[i+FCOUNT] = i+FCOUNT;
       pthread_create(&(pthreadids[i]), &attribs, thread_mmain, &(tids[i+FCOUNT]));
   }

   for (i=0; i< FCOUNT+MCOUNT; i++)
       pthread_join(pthreadids[i], NULL);

   return 0;
}

void *thread_fmain(void * arg)
{
   int tid =  *((int *) arg);
   threadIDs[tid] = pthread_self();

   while(1==1)
   {
       do_other_stuff();
       woman_enter();
       use_rr();
       woman_leave();
   }
}

void *thread_mmain(void * arg)
{
   int tid =  * ((int *) arg);
   threadIDs[tid] = pthread_self();

   while(1==1)
   {
       do_other_stuff();
       man_enter();
       use_rr();
       man_leave();
   }
}


void woman_enter()
{
	clock_t start, stop;
	double waittime;
	start = clock();
   int id = get_simple_tid(pthread_self());
   //Man entry code here
   /* */
   printf("Thread f%d trying to get in the restroom\n", id);
   pthread_mutex_lock(&female_mutex);

   while(femaleCount == RR_CAP){

       pthread_cond_wait(&female_full, &female_mutex);
   }
   //  printf("%d\n", value);
   		sem_wait(&female_semaphore);
   		stop = clock();
   		waittime = (double) (stop-start)/CLOCKS_PER_SEC;
   		++femaleCount;
   		pthread_mutex_unlock(&female_mutex);
   		printf("Thread f%d got in!\n", id);

	printf("Wait time for thread f%d was %f\n", id, waittime);

}

void woman_leave()
{
   int id = get_simple_tid(pthread_self());

   //Exit code here
   /* */
   pthread_mutex_lock(&female_mutex);
   sem_post(&female_semaphore);
   --femaleCount;
   pthread_cond_broadcast(&female_full);
   pthread_mutex_unlock(&female_mutex);
   printf("thread f %d left!\n",id);
}

void man_enter()
{
	clock_t start, stop;
	double waittime;
	start = clock();
   int id = get_simple_tid(pthread_self());
   //Man entry code here
   /* */
   printf("Thread m%d trying to get in the restroom\n", id);
   pthread_mutex_lock(&male_mutex);
   while(maleCount == RR_CAP){
       pthread_cond_wait(&male_full, &male_mutex);
   }
   //  printf("%d\n", value);
   		sem_wait(&male_semaphore);
   		stop = clock();
   		waittime = (double) (stop-start)/CLOCKS_PER_SEC;
   		++maleCount;
   		pthread_mutex_unlock(&male_mutex);
   		printf("Thread f%d got in!\n", id);
	printf("Wait time for thread m%d was %f\n", id, waittime);
}

void man_leave()
{
   int id = get_simple_tid(pthread_self());

   //Exit code here
   /* */
   pthread_mutex_lock(&male_mutex);
   sem_post(&male_semaphore);
   --maleCount;
   pthread_cond_broadcast(&male_full);
   pthread_mutex_unlock(&male_mutex);

   printf("Thread m%d left!\n", id);

}


void use_rr()
{
   struct timespec req, rem;
   double usetime;
   usetime = RR_MAXSLEEP * (random() / (1.0*(double) ((unsigned long) RAND_MAX)));
   req.tv_sec = (int) floor(usetime);
   req.tv_nsec = (unsigned int) ( (usetime - (int) floor(usetime)) * 1000000000);
   printf("Thread %d using restroom for %lf time\n", get_simple_tid(pthread_self()), usetime);
   nanosleep(&req,&rem);
}

void do_other_stuff()
{
   struct timespec req, rem;
   double worktime;
   worktime = OTHER_SLEEP * (random() / (1.0*(double) ((unsigned long) RAND_MAX)));
   req.tv_sec = (int) floor(worktime);
   req.tv_nsec = (unsigned int) ( (worktime - (int) floor(worktime)) * 1000000000);
   printf("Thread %d working for %lf time\n", get_simple_tid(pthread_self()), worktime);
   nanosleep(&req,&rem);
}

int get_simple_tid(pthread_t lid)
{
   int i;
   for (i=0; i<FCOUNT+MCOUNT; i++)
       if (pthread_equal(lid, threadIDs[i]))
           return i;
   printf("Oops! did not find a tid for %lu\n", lid);
   _exit(-1);
}
