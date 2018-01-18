/* $Id: ssq1.c 55 2005-09-13 22:29:52Z asminer $ */
/* -------------------------------------------------------------------------
 * This program simulates a single-server FIFO service node using arrival
 * times and service times read from a text file.  The server is assumed
 * to be idle when the first job arrives.  All jobs are processed completely
 * so that the server is again idle at the end of the simulation.   The
 * output statistics are the average interarrival time, average service
 * time, the average delay in the queue, and the average wait in the service 
 * node. 
 *
 * Name              : ssq1.c  (Single Server Queue, version 1)
 * Authors           : Steve Park & Dave Geyer
 * Language          : ANSI C
 * Latest Revision   : 9-01-98
 * ------------------------------------------------------------------------- 
 */

#include <stdio.h>                              

#define FILENAME   "ssq1.DAT"                  /* input data file */
#define START      0.0

/* =========================== */
   double GetArrival(FILE *fp)                 /* read an arrival time */
/* =========================== */
{ 
  double a;

  fscanf(fp, "%lf", &a);
  return (a);
}

/* =========================== */
   double GetService(FILE *fp)                 /* read a service time */
/* =========================== */
{ 
  double s;

  fscanf(fp, "%lf\n", &s);
  return (s);
}

/* ============== */
   int main(void)
/* ============== */
{
  FILE   *fp;                                  /* input data file      */
  long   index     = 0;                        /* job index            */
  double arrival   = START;                    /* arrival time         */
  double delay;                                /* delay in queue       */
  double max_delay = 0;                        /* max delay            */
  int num_queue;                               /* number of jobs in queue at time = 400 */
  double num_delayed;                          /* number of jobs that are delayed */ 
  double service;                              /* service time         */
  double wait;                                 /* delay + service      */
  double departure = START;                    /* departure time       */
  double observation = 0;		                   /* observation time     */
  double arrival_rate = 0;		                 /* arrival rate 	       */
  double service_rate = 0;		                 /* service rate 	       */
  double avg_queue = 0;		                     /* avg number of people in queue */
  double avg_service = 0;		                   /* avg number of people in service */
  int found = 0;
  struct {                                     /* sum of ...           */
    double delay;                              /*   delay times        */
    double wait;                               /*   wait times         */
    double service;                            /*   service times      */
    double interarrival;                       /*   interarrival times */
    int num_queue;			                       /*   jobs in queue      */
  } sum = {0.0, 0.0, 0.0, 0, 0};

  fp = fopen(FILENAME, "r");
  if (fp == NULL) {
    fprintf(stderr, "Cannot open input file %s\n", FILENAME);
    return (1);
  }

  while (!feof(fp)) {
    index++;
    arrival      = GetArrival(fp);
    if (arrival < departure) {
      num_delayed++;
      sum.num_queue++;                         /* job gets added to queue */
      delay      = departure - arrival;        /* delay in queue    */
      if(delay > max_delay)
        max_delay = delay;
    }else{
      delay      = 0.0;                        /* no delay          */
    }

    service      = GetService(fp);
    wait         = delay + service;
    departure    = arrival + wait;             /* time of departure */

    if(departure >= 400 && !found) {
      num_queue = sum.num_queue;
      found = 1;
    }  

    sum.delay   += delay;
    sum.wait    += wait;
    sum.service += service;
  }
  observation = departure;
  sum.interarrival = arrival - START;
  
  printf("\n1. --------------------------------------\n");  	
  printf("for %ld jobs\n", index);
  printf("   average interarrival time = %8.6f\n", sum.interarrival / index);
  printf("   average service time .... = %8.6f\n", sum.service / index);
  printf("   average delay ........... = %8.6f\n", sum.delay / index);
  printf("   average wait ............ = %8.6f\n", sum.wait / index);
  printf("2. --------------------------------------\n");  	
  /* Homework problem 2 starts here */
  arrival_rate = 1/(sum.interarrival/index);
  service_rate = 1/(sum.service/index);
  avg_queue = sum.num_queue / observation;
  avg_service = index / observation;

  printf("   arrival rate ............ = %8.6f\n", arrival_rate);
  printf("   service rate ............ = %8.6f\n", service_rate);
  printf("   server utilization ...... = %8.6f\n", sum.service / observation);
  printf("   traffic intensity  ...... = %8.6f\n", arrival_rate / service_rate);
  printf("   average number in queue . = %8.6f\n", avg_queue);
  printf("   average number in service = %8.6f\n", avg_service);
  printf("   average number at server  = %8.6f\n", avg_queue + avg_service);
  printf("3. --------------------------------------\n");
  /* Homework problem 3 starts here */
  printf("   max delay ............... = %8.6f\n", max_delay);
  printf("   customers at time = 400 . = %d\n", num_queue);
  printf("   proportion of delayed ... = %8.6f\n", num_delayed / index);

  fclose(fp);
  return (0);
}
