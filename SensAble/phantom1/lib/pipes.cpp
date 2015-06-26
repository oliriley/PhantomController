#include "pipes.h"
#include "threads.h"
#include "common.h"
#include <cstdlib>
#include <cstdio>
#include "win.h"

static void startit(void*a){((out_pipe*)a)->start();}

void out_pipe::init(char*n,int l,int q,enum MODE m)
  {
  name=(char*)malloc(sizeof(char)*(strlen(n)+32));
  sprintf(name,"\\\\.\\pipe\\%s",n);
  pipe=NULL;
  length=l;
  if(q<=0)qsize=1;else qsize=q;
  connected=false;
  mode=m;
  data=malloc(qsize*length);
  current=malloc(length);
  qstart=qtotal=0;  
  th=new thread(startit,this);
  }

out_pipe::out_pipe(void){}

enum out_pipe::ERRORCODE out_pipe::send(void*a)
  {
  enum ERRORCODE ret=OKAY;
  char*dest=NULL;
    
  if(1)
    {
    LG(m);
    if(!connected)return NOT_CONNECTED;

    if(qtotal==qsize)
      if(mode==REPLACE)dest=((char*)data)+(qstart*length),qstart=(qstart+1)%qsize,ret=QUEUE_FULL_REPLACED;
      else if(mode==BLOCK){while(qtotal==qsize)c2.wait(&m);if(connected)dest=((char*)data)+(((qstart+qtotal++)%qsize)*length);}
      else return QUEUE_FULL_DISCARDED;
    else dest=((char*)data)+(((qstart+qtotal++)%qsize)*length);

    if(dest)memcpy(dest,a,length);
    }
  
  c1.wake();
  return ret;
  }
   
void out_pipe::start(void)
  { 
  
  pipe=CreateNamedPipe(name,   PIPE_ACCESS_DUPLEX|FILE_FLAG_FIRST_PIPE_INSTANCE   ,   PIPE_TYPE_MESSAGE|PIPE_READMODE_MESSAGE|PIPE_WAIT   ,1,length*0,0,0,NULL);                     
      
  if(pipe==INVALID_HANDLE_VALUE){fprintf(stderr,"CreateNamedPipe \"%s\" failed, GLE=%d.\n",name,GetLastError());exit(1);}
  printf("Pipe server awaiting client connection on \"%s\"\n", name);
  
  while(1)
    {
    BOOL conn=ConnectNamedPipe(pipe,NULL)?TRUE:(GetLastError()==ERROR_PIPE_CONNECTED);
    if(conn)printf("Client connected. Enablilng streaming on \"%s\"\n",name);

    m.lock();
    connected=(conn!=0);

    while(connected) 
      { 
      while(qtotal==0)c1.wait(&m);
      memcpy(current,((char*)data)+(qstart*length),length);
      qstart=(qstart+1)%qsize;
      qtotal--;

      m.unlock();
            
      if(mode==BLOCK)c2.wake();

      DWORD written;
      BOOL success;
      
      success=WriteFile(pipe,current,length,&written,NULL);

      m.lock();

      if(!success || (length!=written))
        {
        printf("Writing Failed... disconnecting server \"%s\", GLE=%d.\n",name,GetLastError());
        connected=false;
        qtotal=qstart=0;
        c2.wake();
        }

      }
    m.unlock();
    DisconnectNamedPipe(pipe);
    }
  }




void in_pipe::init(char*n,int l)
  {
  name=(char*)malloc(sizeof(char)*(strlen(n)+32));
  sprintf(name,"\\\\.\\pipe\\%s",n);
  pipe=NULL;
  length=l;
  connected=false;
  }

in_pipe::in_pipe(void){}

void in_pipe::receive(void*a)
  {
  while(1)
    {
    if(!connected)start();

    BOOL success;
    DWORD read;
      
    success=ReadFile(pipe,a,length,&read,NULL);
    
    if(!success || length!=read)end();
    else break;
    }
  }

void in_pipe::start(void)
  {
  while (1) 
    {
    WaitNamedPipe(name,NMPWAIT_WAIT_FOREVER);

    pipe=CreateFile(name,  GENERIC_READ|GENERIC_WRITE  ,0,NULL,  OPEN_EXISTING  ,0,NULL);

    if(pipe!=INVALID_HANDLE_VALUE)goto okay;

    if(GetLastError()!=ERROR_PIPE_BUSY)continue;

    okay:

    DWORD nm=PIPE_READMODE_MESSAGE; 
    BOOL success=SetNamedPipeHandleState(pipe,&nm,NULL,NULL);
    
    if(!success)
      {
      printf("SetNamedPipeHandleState failed on \"%s\". GLE=%d\n",name,GetLastError()); 
      end();
      continue;
      }

    printf("Pipe Client connected on \"%s\"",name);

    connected=true;
    break;
    }
  }

void in_pipe::end(void)
  {
  printf("Pipe Client disconnected on \"%s\". Reconnecting...",name);
  connected=false;
  CloseHandle(pipe);
  }

  