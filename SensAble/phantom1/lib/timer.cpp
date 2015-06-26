#include "win.h"

#include "timer.h"


double get_cpu_time(void)
  {
  uint64 t1,t2;
  QueryPerformanceFrequency((LARGE_INTEGER*)&t2);
  QueryPerformanceCounter((LARGE_INTEGER*)&t1);
  return (double)t1/(double)t2;}

uint64 get_raw_cpu(void)
  {
  uint64 t1;
  QueryPerformanceCounter((LARGE_INTEGER*)&t1);
  return t1;
  }

/*
void getstamp(char*a)
  {
  time_t rawtime;
  struct tm*timeinfo;
  time(&rawtime);
  timeinfo=localtime(&rawtime);
  char t1[32];
  sprintf(t1,"%s",asctime(timeinfo));
  char t2[32],t3[32],t4[32],t5[32]; 
  sscanf(t1,"%s %s %s %s %s",t5,t2,t3,t4,t5);
  int q1;
  for(q1=0;t4[q1];q1++)if(t4[q1]==':')t4[q1]='.';
  sprintf(a,"%s.%s.%s",t4,t2,t3);
  }
*/



timer::timer()
  {
  QueryPerformanceFrequency((LARGE_INTEGER*)&pf);
  QueryPerformanceCounter((LARGE_INTEGER*)&st);
  }

uint64 timer::get(void){return get_raw_cpu()-st;}
  
double timer::operator()(void){return (double)get()/(double)pf;}

double timer::operator()(int a){return (double)(get()-ss[a])/(double)pf;}

void timer::start(int a){ss[a]=get();}
