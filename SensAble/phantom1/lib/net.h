#ifndef NET_H
#define NET_H

#include "win.h"
#include "common.h"

class net_main
{
protected:

static const int waittime=100;


WSADATA wsa_data;
SOCKET sock;
SOCKET fd;

int port;
char*address;

int length;
int sendmode,recvmode,recvmode2;
char*outtemp,*intemp,*out,*in;
int receiveddata;
int hasdatatosend;
int socketready;


thread*tsend,*treceive;
mutex*msend,*mreceive,*msocket;
condition_variable*csenddata,*creceivedata,*csendspace,*creceivespace;

void maininit(int p,int l,int m1,int m2,int m3);

SOCKET getsocket(void);
SOCKET acceptconnection(void);
SOCKET connecttoserver(void);

void get_send_data(void);
void put_receive_data(void);
int sendone(void);
int receiveone(void);

virtual void establish(void)=0;
virtual void destroy(void)=0;

public: net_main(void);
public: void update(char*); 
public: void receive(char*); 
public: void start(int op);
public: void terminate(void);

};


class net_server : public net_main
{
private:
void establish(void);
void destroy(void);

public: net_server(void);
public: void init(int p,int l,int m1,int m2,int m3);
};


class net_client : public net_main
{
private:
void establish(void);
void destroy(void);

public: net_client(void);
public: void init(char*address,int p,int l,int m1,int m2,int m3);
};

#endif
