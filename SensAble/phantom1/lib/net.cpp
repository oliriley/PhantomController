#include "threads.h"
#include "net.h"


net_main::net_main(void){};
net_server::net_server(void){};
net_client::net_client(void){};

void net_main::terminate(void){closesocket(fd);}

SOCKET net_main::getsocket(void)
  {
  BOOL val=TRUE;
  SOCKET sock;
  SOCKADDR_IN server_address;
  WSAStartup(MAKEWORD(2,2),&wsa_data);

  sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  
  setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,(char*)&val,sizeof(val));
    
  server_address.sin_family=AF_INET;
  server_address.sin_port=htons(port);
  server_address.sin_addr.s_addr=htonl(INADDR_ANY);
  
  bind(sock,(SOCKADDR*)&server_address,sizeof(server_address));
  listen(sock,32);

  return sock;
  }

SOCKET net_main::acceptconnection(void)
  {
  int clilen;
  DWORD stringlen;
  struct sockaddr_in client;
  char name[32];
  clilen=sizeof(client);
  stringlen=sizeof(name);

  SOCKET fd=accept(sock,(struct sockaddr*)&client,&clilen);
  
  WSAAddressToString((struct sockaddr*)&client,clilen,NULL,name,&stringlen);
  
  
  if(fd==INVALID_SOCKET){printf("ERROR on accept(): error %ld. Retrying...\n",WSAGetLastError());return acceptconnection();}
  else printf("Server accepted a connection. File Descriptor (%d) for \"%s\"\n",fd,name);
  
  return fd;
  }

SOCKET net_main::connecttoserver(void)
  {
  SOCKET sock;
  struct hostent*server;
  sockaddr_in client;
  int socket_stat;
    
  WSAStartup(MAKEWORD(2,2),&wsa_data);
  sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

  server=gethostbyname(address);
  if(server==NULL)
    {
    printf("Net Client ERROR - no such host - %s. Retrying...\n",address);
    Sleep(waittime);
    return connecttoserver();
    }
  else printf("Host \"%s\" found.\n",address);

  memset(&client,0,sizeof(client));
  client.sin_family=AF_INET;
  memcpy(&client.sin_addr.s_addr,server->h_addr,server->h_length);
  client.sin_port=htons(port);

  while(1)
    {
    socket_stat=connect(sock,(SOCKADDR*)&client,sizeof(client));
    if(socket_stat==SOCKET_ERROR)
      {
      printf("Net Client: connect() - Failed to connect (Error: %d) to %s:%d. Retrying...\n",WSAGetLastError(),address,port);
      Sleep(waittime);
      continue;
      }
    break;
    }
   
  return sock;
  }

void net_main::maininit(int p,int l,int m1,int m2,int m3)
  {
  port=p;
  sendmode=m1;
  recvmode=m2;
  recvmode2=m3;
  length=l;
  out=(char*)malloc(length);
  in=(char*)malloc(length);
  outtemp=(char*)malloc(length);
  intemp=(char*)malloc(length);
  memset(out,0,sizeof(out));
  memset(in,0,sizeof(in));
  memset(outtemp,0,sizeof(outtemp));
  memset(intemp,0,sizeof(intemp));
  
  socketready=0;
  hasdatatosend=0;
  receiveddata=0;
  sock=fd=0;
  address=NULL;
  
  msend=new mutex();
  mreceive=new mutex();
  msocket=new mutex();
  csenddata=new condition_variable();
  creceivedata=new condition_variable();
  csendspace=new condition_variable();
  creceivespace=new condition_variable();
    
  }


static void start0(void*a){((net_main*)a)->start(0);}
static void start1(void*a){((net_main*)a)->start(1);}

void net_server::init(int p,int l,int m1,int m2,int m3)
  {
  maininit(p,l,m1,m2,m3);
  
  sock=getsocket();
  
  treceive=new thread(start1,this);
  tsend=new thread(start0,this);

  }

void net_client::init(char*add,int p,int l,int m1,int m2,int m3)
  {
  maininit(p,l,m1,m2,m3);
  
  address=(char*)malloc(strlen(add)+32);
  strcpy(address,add);
  
  treceive=new thread(start1,this);
  tsend=new thread(start0,this);
  }
  
  
void net_main::update(char*a)
   {
   msend->lock();
   if(sendmode==2)while(hasdatatosend==1)csendspace->wait(msend);
   if(hasdatatosend!=1 || sendmode!=1)
     {
     memcpy(outtemp,a,length);
     hasdatatosend=1;
     csenddata->wake();
     }
   msend->unlock();
   }

void net_main::receive(char*a)
   {
   mreceive->lock();
   if(recvmode2==1)while(receiveddata==0)creceivedata->wait(mreceive);
   memcpy(a,intemp,length);
   receiveddata=0;
   creceivespace->wake();
   mreceive->unlock();
   }

void net_main::get_send_data(void)
  {
  msend->lock();
  while(hasdatatosend==0)csenddata->wait(msend);
  memcpy(out,outtemp,length);
  hasdatatosend=0;
  csendspace->wake();
  msend->unlock();
  }

void net_main::put_receive_data(void)
  {
  mreceive->lock();
  if(recvmode==2)while(receiveddata==1)creceivespace->wait(mreceive);
  if(receiveddata!=1 || recvmode!=1)
    {
    memcpy(intemp,in,length);
    receiveddata=1;
    creceivedata->wake();
    }
  mreceive->unlock();
  }


void net_client::establish(void)
  {
  msocket->lock();
  if(socketready==0)fd=connecttoserver();
  socketready=1;
  msocket->unlock();
  }

void net_server::establish(void)
  {
  msocket->lock();
  if(socketready==0)fd=acceptconnection();
  socketready=1;
  msocket->unlock();
  }

void net_client::destroy(void)
  {
  socketready=0;
  closesocket(fd);
  Sleep(waittime);
  }

void net_server::destroy(void)
  {
  socketready=0;
  closesocket(fd);
  }


int net_main::sendone(void)
  {
  get_send_data();

  int sent=send(fd,out,length,0);
 
  if(sent==SOCKET_ERROR){printf("Net Server: send() error - %ld. Restarting...\n",WSAGetLastError());return 0;}
  else if(sent!=length){printf("Net Server: Not enough bytes sent %d/%d Restarting...\n",sent,length);return 0;}
  //else {printf("Server: send() is OK.\n");printf("Server: Bytes Sent: %ld.\n", sent);}
  return 1;
  }

int net_main::receiveone(void)
  {
  int received=recv(fd,in,length,0);
      
  if(received==SOCKET_ERROR){printf("Net Server: recv() error - %ld. Restarting...\n",WSAGetLastError());return 0;}
  else if(received!=length){printf("Net Server: Not enough bytes received %d/%d Restarting...\n",received,length);return 0;}
  //else {printf("Server: send() is OK.\n");printf("Server: Bytes Sent: %ld.\n", sent);}
      
  put_receive_data();  
  return 1;
  }


void net_main::start(int op)
  {
  while(1)
    {
    establish();
    if(op==0)while(sendone());
    else while(receiveone());
    destroy();
    }   
  }
  

  /*

net_server::net_server(void){};

void net_server::init(int p,enum MODE mode,int size,void(*f)(void*))
  {
  port=p;
  this->mode=mode;
  length=size;
  out=(char*)malloc(size);
  in=(char*)malloc(size);
  temp=(char*)malloc(size);
  hasdata=0;
  callback=f;
  th=new thread([](void*a){((net_server*)a)->start();},this);
  }

void net_server::update(char*a)
   {
   m.lock();
   memcpy(temp,a,length);
   hasdata=1;
   m.unlock();
   c.wake();
   }

void net_server::start(void)
  {
  WSADATA wsa_data;
  SOCKET sock,sock2;
  SOCKADDR_IN server_address;
  WSAStartup(MAKEWORD(2,2),&wsa_data);
  
  sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    
  server_address.sin_family=AF_INET;
  server_address.sin_port=htons(port);
  server_address.sin_addr.s_addr=htonl(INADDR_ANY);
  
  bind(sock,(SOCKADDR*)&server_address,sizeof(server_address));
  listen(sock,5);
     
  while(1)
    {
    sock2=accept(sock,NULL,NULL);

    while(1)
      {
     // printf("Locking Mutex\n");
      m.lock();
    //  printf("Mutex locked... waiting for data\n");
      while(hasdata==0)c.wait(&m);
    //  printf("Data here... copying and updating\n");
      memcpy(out,temp,length);
      hasdata=0;
    //  printf("Unlocking..\n");
      m.unlock();



      int sent=0,received=0;

      sent=send(sock2,out,length,0);
 
      if(sent==SOCKET_ERROR){printf("Net Server: send() error - %ld. Restarting...\n",WSAGetLastError());break;}
      else if(sent!=length){printf("Net Server: Not enough bytes sent %d/%d Restarting...\n",sent,length);break;}
      else;// {printf("Server: send() is OK.\n");printf("Server: Bytes Sent: %ld.\n", sent);}

      received=recv(sock2,in,length,0);

      if(received==SOCKET_ERROR){printf("Net Server: recv() error - %ld. Restarting...\n",WSAGetLastError());break;}
      else if(received!=length){printf("Net Server: Not enough bytes received %d/%d Restarting...\n",received,length);break;}
      else;// {printf("Server: send() is OK.\n");printf("Server: Bytes Sent: %ld.\n", sent);}

      callback(in);

      }
    closesocket(sock2);
     
    }
  WSACleanup();        
  }


net_client::net_client(void){}

void net_client::init(int p,char*address,enum MODE mode,int size,void(*f)(void*))
  {
  port=p;
  this->address=(char*)malloc((strlen(address)+32));
  strcpy(this->address,address);
  this->mode=mode;
  length=size;
  out=(char*)malloc(size);
  in=(char*)malloc(size);
  temp=(char*)malloc(size);
  hasdata=0;
  callback=f;
  th=new thread([](void*a){((net_client*)a)->start();},this);
  }

void net_client::update(char*a)
   {
   m.lock();
   memcpy(temp,a,length);
   m.unlock();
   }

void net_client::start(void)
  {
  
  while(1)
    {
    WSADATA wsa_data;
    SOCKET sock;
    struct hostent*server;
    sockaddr_in client;
    int socket_stat;
    
    WSAStartup(MAKEWORD(2,2),&wsa_data);
    sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    server=gethostbyname(address);
    if(server==NULL){printf("Net Client ERROR - no such host - %s\n",address);Sleep(100);continue;}
    else printf("Host \"%s\" found.\n",address);

    memset(&client,0,sizeof(client));
    client.sin_family=AF_INET;
    memcpy(&client.sin_addr.s_addr,server->h_addr,server->h_length);
    client.sin_port=htons(port);
  
    socket_stat=connect(sock,(SOCKADDR*)&client,sizeof(client));

    if(socket_stat==SOCKET_ERROR){printf("Net Client: connect() - Failed to connect (Error: %d) to %s:%d.\n",WSAGetLastError(),address,port);Sleep(100);continue;}
   
 
    while(1)
      {      
      int received,sent;

      received=recv(sock,in,length,0);
           
      if(received==0||received==WSAECONNRESET){printf("Net Client: Connection Closed.\n");break;}
      if(received<0){printf("Net Client: receiving error - %d\n",received);break;}

      callback(in);

      m.lock();

      memcpy(out,temp,length);

      m.unlock();


       
      sent=send(sock,out,length,0);

      if(sent==SOCKET_ERROR){printf("Client: send() error %ld.\n",WSAGetLastError());break;}
      
      }
    WSACleanup(); 
    Sleep(100);
    
    }
  }

*/