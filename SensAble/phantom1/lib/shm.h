#ifndef SHM_H
#define SHM_H

#include "win.h"
#include "common.h"

class shm_main
{
protected:
static const int padding=32;
HANDLE memory;
HANDLE e1,e2,mut;
char*name;
int length,qsize;
int*qstart,*qtotal;
void*data;

public: shm_main(void);
public: void super_init(char*n,int length,int queue);
};

class shm_server : public shm_main
{
public: enum MODE {DISCARD,BLOCK,REPLACE};
public: enum ERRORCODE {OKAY,QUEUE_FULL_DISCARDED,QUEUE_FULL_REPLACED};

private: enum MODE mode;

public: shm_server(void);
public: void init(char*n,int length,int queue,enum MODE m);
public: enum ERRORCODE send(void*data);
};

class shm_client : public shm_main
{
public: shm_client(void);
public: void init(char*n,int length,int queue);
public: void receive(void*data);
};


#endif