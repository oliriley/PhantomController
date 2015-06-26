
#include "common.h"

#include "shm.h"

#include "win.h"



void shm_main::super_init(char*n,int l,int q)
  {
  
  char*ne1,*ne2,*nm;
  int*a;
  int pad=shm_server::padding;
  name=(char*)malloc(sizeof(char)*(strlen(n)+32));
  ne1=(char*)malloc(sizeof(char)*(strlen(n)+32));
  ne2=(char*)malloc(sizeof(char)*(strlen(n)+32));
  nm=(char*)malloc(sizeof(char)*(strlen(n)+32));
  sprintf(name,"%s",n);
  sprintf(ne1,"%s-hasdata",n);
  sprintf(ne2,"%s-hasspace",n);
  sprintf(nm,"%s-mutex",n);
  length=l;
  if(q<=0)qsize=1;else qsize=q;
   
  memory=CreateFileMappingA(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,pad+qsize*length,name);
  if(memory==NULL||memory==INVALID_HANDLE_VALUE){fprintf(stderr,"Cannot create shared memory object \"%s\" - %d, address returned: %p",name,GetLastError(),memory);exit(1);}
  data=MapViewOfFile(memory,FILE_MAP_ALL_ACCESS,0,0,pad+qsize*length);
  if(data==NULL){fprintf(stderr,"Cannot view of file on client shared memory \"%s\" - %d, address returned: %p",name,GetLastError(),data);exit(1);}
  e1=CreateEventA(NULL,TRUE,FALSE,ne1);
  e2=CreateEventA(NULL,TRUE,TRUE,ne2);
  mut=CreateMutexA(NULL,FALSE,nm);
  if(e1==NULL||e1==INVALID_HANDLE_VALUE||e2==NULL||e2==INVALID_HANDLE_VALUE||mut==NULL||mut==INVALID_HANDLE_VALUE){fprintf(stderr,"Cannot create shared memory mutexes \"%s\"",name);exit(1);}
  
  WaitForSingleObject(mut,INFINITE);
  memset(data,0,pad+qsize*length);
  a=(int*)data;
  qstart=a;
  qtotal=a+1;
  data=(char*)data+pad;
  ResetEvent(e1);
  SetEvent(e2);
  ReleaseMutex(mut);

  }

shm_main::shm_main(void){}
shm_server::shm_server(void){}
shm_client::shm_client(void){}

void shm_server::init(char*n,int l,int q,enum MODE m){super_init(n,l,q);mode=m;}
void shm_client::init(char*n,int l,int q){super_init(n,l,q);}



enum shm_server::ERRORCODE shm_server::send(void*a)
  {
  enum ERRORCODE ret=OKAY;
  char*dest=NULL;

  //printf("WaitforCritical\n");
  WaitForSingleObject(mut,INFINITE);
  
  int&qtotal=*(this->qtotal);
  int&qstart=*(this->qstart);

  //printf("Got mutex- %d %d\n",qstart,qtotal);

  if(qtotal==qsize)
    if(mode==REPLACE)dest=((char*)data)+(qstart*length),qstart=(qstart+1)%qsize,ret=QUEUE_FULL_REPLACED;
    else if(mode==BLOCK)
      {
      while(qtotal==qsize)
        {
 //       printf("Wating for space - resetting hasspace\n");
        ResetEvent(e2); 
 //       printf("Wating for space - releasing mutex\n");
        ReleaseMutex(mut);
 //       printf("Wating for space - waiting for space\n");
        WaitForSingleObject(e2,INFINITE);
 //       printf("Wating for space - acquiring mutex\n");
        WaitForSingleObject(mut,INFINITE);
  ///      printf("Wating for space - mutex acquired\n");
        }
      dest=((char*)data)+(((qstart+qtotal++)%qsize)*length);
      }
    else ret=QUEUE_FULL_DISCARDED;
  else dest=((char*)data)+(((qstart+qtotal++)%qsize)*length);
  

 // printf("Writing data\n");
  if(dest)memcpy(dest,a,length);

 // printf("Releasing mutex - %d %d\n",((int*)data)[-8],((int*)data)[-7]);
  SetEvent(e1);
  ReleaseMutex(mut);
  
 // printf("Notifying reader\n");

  return ret;
  }


void shm_client::receive(void*a)
  {
 // printf("Waiting for mutex\n");
  WaitForSingleObject(mut,INFINITE);
  int&qtotal=*(this->qtotal);
  int&qstart=*(this->qstart);

 // printf("Got Mutex - %d %d\n",qstart,qtotal);


  while(qtotal==0)
    {
   // printf("Waiting for data - resetting hasdata\n");
    ResetEvent(e1);
  //  printf("Waiting for data - releasing mutex\n");
    ReleaseMutex(mut);
  //  printf("Waiting for data - waiting for data\n");
    WaitForSingleObject(e1,INFINITE);
 //   printf("Waiting for data - getting mutex\n");
    WaitForSingleObject(mut,INFINITE);
  //  printf("Waiting for data - got mutex and data\n");
    }

  memcpy(a,((char*)data)+(qstart*length),length);
  qstart=(qstart+1)%qsize;
  qtotal--;

 
 // printf("Notifying server there's spase\n");
  SetEvent(e2);
 // printf("Releasing mutex - %d %d\n",((int*)data)[-8],((int*)data)[-7]);
  ReleaseMutex(mut);
  

  }
  