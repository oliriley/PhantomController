#ifndef PIPES_H
#define PIPES_H

#include "win.h"
#include "threads.h"

class out_pipe
{
public: enum MODE {DISCARD,BLOCK,REPLACE};
public: enum ERRORCODE {OKAY,NOT_CONNECTED,QUEUE_FULL_DISCARDED,QUEUE_FULL_REPLACED};

private:

HANDLE pipe;
char*name;
int length;
int qsize,qstart,qtotal;
void*data;
void*current;
bool connected;
enum MODE mode;

mutex m;
condition_variable c1,c2;
thread*th; 

public: out_pipe(void);
public: void start(void);
public: void init(char*n,int length,int queue,enum MODE m);
public: enum ERRORCODE send(void*data);
};

class in_pipe
{
private:
HANDLE pipe;
char*name;
int length;
bool connected;

mutex m;
condition_variable c1,c2;
thread*th; 

public: in_pipe(void);
private: void start(void);
private: void end(void);
public: void init(char*n,int length);
public: void receive(void*data);
};

#endif