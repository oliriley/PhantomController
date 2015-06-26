#ifndef TIMER_H
#define TIMER_H

#include "common.h"

double get_cpu_time(void);
uint64 get_raw_cpu(void);

class timer
{
int64 st;
int64 pf;
map < int , int64 > ss;

public: timer();
public: uint64 get(void);
public: double operator()(void);
public: double operator()(int a);
public: void start(int a);
};

void getstamp(char*a);

#endif
