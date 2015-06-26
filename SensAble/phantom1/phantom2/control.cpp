#include "lib/common.h"

static const double ANGULAR_RESOLUTION_TURRET[2]={5532.4615,0.0};
static const double ANGULAR_RESOLUTION_THIGH[2]={4731.1265,0.0};
static const double ANGULAR_RESOLUTION_SHIN[2]={4731.1265,0.0};

static const double TURRET_JOINT_LIMIT[2][2]={{1.0,1.2},{0.0,0.0}};
static const double THIGH_JOINT_LIMIT[2][2]={{-1.6,-1.8},{0.0,0.0}};
static const double SHIN_JOINT_LIMIT[2][2]={{0.0,0.2},{0.0,0.0}};
static const double SHIN_THIGH_MAX_JOINT_LIMIT[2][2]={{0.50,0.7},{0.0,0.0}};
static const double SHIN_THIGH_MIN_JOINT_LIMIT[2][2]={{-0.40,-0.60},{0.0,0.0}};

static const double SAFETY_GAINS[2][3]={{2000.0,3000.0,2000.0},{0.0,0.0,0.0}};
static const double SAFETY_SPEED_MARGINS[2][2]={{2.0,0.5},{1.0,1.0}};
static const double SAFETY_SPEED_TARGETS[2]={1.0,1.0};
static const double SAFETY_SPEED_LIMITS[2]={10.0,10.0};
static const double SAFETY_SPEED_RATIO[2]={1.25,1.25};
static const int HISTORY_SIZE=11;



double singlecontrol(double first,double second,double pos,double vel,double target,double vmargin,double gain)
  {
  if(first<second)
    {
    if(pos<first)return 0.0;
    double pc=0.0,vc=0.0;    
    
    if(pos<second)pc=(pos-first)/(second-first);else pc=1.0;
    
    if(vel>target+vmargin)vc=1.0;
    else if(vel>target-vmargin)vc=fabs(vel-target)/vmargin;
    else vc=1.0;
    
    return -(vel-target)*pc*vc*gain;
    }
  else 
    {
    if(pos>first)return 0.0;
    double pc=0.0,vc=0.0;    
    
    if(pos>second)pc=(-pos+first)/(first-second);else pc=1.0;
    
    if(vel>target+vmargin)vc=1.0;
    else if(vel>target-vmargin)vc=fabs(vel-target)/vmargin;
    else vc=1.0;
    
    return -(vel-target)*pc*vc*gain;
    }
  }

double singlecoef(double first,double second,double pos,double vel,double vmargin)
  {
  if(first<second)
    {
    if(pos<first)return 1.0;
    double pc=0.0,vc=0.0;    
    
    if(pos<second)pc=(pos-first)/(second-first);else pc=1.0;
    
    if(vel>vmargin)vc=1.0;
    else if(vel>-vmargin)vc=fabs(vel)/vmargin;
    else vc=1.0;
    
    return (1.0-pc)*(1.0-vc);
    }
  else 
    {
    if(pos>first)return 1.0;
    double pc=0.0,vc=0.0;    
    
    if(pos>second)pc=(-pos+first)/(first-second);else pc=1.0;
    
    if(vel>vmargin)vc=1.0;
    else if(vel>-vmargin)vc=fabs(vel)/vmargin;
    else vc=1.0;
    
    return (1.0-pc)*(1.0-vc);
    }
  }

double singlespeedlimit(double border,double limit,double vel,double gain)
  {
  double vc=0.0;
  if(vel>border)
    if(vel>limit)vc=1.0;
    else vc=(vel-border)/(limit-border);
  else if(vel<-border)
    if(vel<-limit)vc=1.0;
    else vc=(-vel-border)/(limit-border);
  else vc=0.0;
  
  return -gain*vel*vc;
  }


class phantom
{
int type,total,start;
Vec hist[HISTORY_SIZE];
Vec curv,curx,lastu;

public: phantom(int t)
  {
  type=t;
  total=start=0;
  for(int q1=0;q1<HISTORY_SIZE;q1++)hist[q1]=Vec(3);
  curv=curx=lastu=Vec(3);
  }
  
public: void addsample(Vec raw)
  {
  Vec x=raw;
    
  x[0]=raw[0]/ANGULAR_RESOLUTION_TURRET[type];
  x[1]=raw[1]/ANGULAR_RESOLUTION_THIGH[type];
  x[2]=raw[2]/ANGULAR_RESOLUTION_SHIN[type];
  
  hist[start]=x;
  if(total==0){for(int q1=1;q1<HISTORY_SIZE;q1++)hist[q1]=hist[0];total=10;}
  start=(start+1)%HISTORY_SIZE;
  curx=x;
  calculatev();
  }
  
public: Vec getx(void){return curx;}
public: Vec getv(void){return curv;}

private: void calculatev(void)
  {
  Vec f0=hist[(start+HISTORY_SIZE-1)%HISTORY_SIZE];
  Vec f1=hist[(start+HISTORY_SIZE-2)%HISTORY_SIZE];
  Vec f2=hist[(start+HISTORY_SIZE-3)%HISTORY_SIZE];
  Vec f3=hist[(start+HISTORY_SIZE-4)%HISTORY_SIZE];
  Vec f4=hist[(start+HISTORY_SIZE-5)%HISTORY_SIZE];
  
  Vec v=(f0-f1)/2.0;
  //Vec v=(f0+f1-f2-f3)/4.0;q
  //Vec v=(7.0*f0 + f1 - 10.0*f2 - f3 + 3.0*f4)/10.0;
  
  v=v*1000.0;
  //v=curv*0.875+v*0.125;
  v=curv*0.9+v*0.1;
  //v=curv*0.75+v*0.25;
  curv=v;
  }
  
private: double clamp(double a)
  {
  if(a<-30000.0)a=-30000.0;
  if(a>30000.0)a=30000.0;
  return a;
  }  

private: double climb(double last,double cur)
  {
  if(cur>last)if(cur-last>2000.0)cur=last+2000.0;
  if(cur<last)if(last-cur>2000.0)cur=last-2000.0;
  return cur;
  }

private: Vec coefs(Vec x,Vec v)
  {
  double c1,c2;
  double u1,u11,u12;
  double u2,u21,u22,u23;
  double u3,u31,u32,u33;
  double margin;
  
  margin=SAFETY_SPEED_MARGINS[type][1];
  
  
  c1=TURRET_JOINT_LIMIT[type][0];c2=TURRET_JOINT_LIMIT[type][1];
  
  u11=singlecoef(c1,c2,x[0],v[0],margin);
  u12=singlecoef(-c1,-c2,x[0],v[0],margin);
   
  u1=u11*u12;
  
  
  c1=SHIN_JOINT_LIMIT[type][0];c2=TURRET_JOINT_LIMIT[type][1];  
  u21=singlecoef(c1,c2,x[1],v[1],margin);
  
  c1=SHIN_THIGH_MIN_JOINT_LIMIT[type][0];c2=SHIN_THIGH_MIN_JOINT_LIMIT[type][1];
  u22=singlecoef(c1,c2,x[1]-x[2],v[1]-v[2],margin);
  
  c1=SHIN_THIGH_MAX_JOINT_LIMIT[type][0];c2=SHIN_THIGH_MAX_JOINT_LIMIT[type][1];  
  u23=singlecoef(c1,c2,x[1]-x[2],v[1]-v[2],margin);
  
  u2=u21*u22*u23;
  
  
  c1=THIGH_JOINT_LIMIT[type][0];c2=THIGH_JOINT_LIMIT[type][1]; 
  u31=singlecoef(c1,c2,x[2],v[2],margin);
  
  c1=SHIN_THIGH_MIN_JOINT_LIMIT[type][0];c2=SHIN_THIGH_MIN_JOINT_LIMIT[type][1];  
  u32=singlecoef(-c1,-c2,x[2]-x[1],v[2]-v[1],margin);
  
  c1=SHIN_THIGH_MAX_JOINT_LIMIT[type][0];c2=SHIN_THIGH_MAX_JOINT_LIMIT[type][1];
  u33=singlecoef(-c1,-c2,x[2]-x[1],v[2]-v[1],margin);
  
  u3=u31*u32*u33;
  
  
  Vec u(3);
  
  u[0]=u1;
  u[1]=u2;
  u[2]=u3;
  
  return u;
  }

public: void discretize(Vec u,long*a)
  {
  for(unsigned int q1=0;q1<u.n;q1++)
    {
    long c1=(long)u[q1];
    if(c1<-30000)c1=-30000;
    if(c1>30000)c1=30000; 
    a[q1]=c1;
    }
  }
  
  
public: Vec getu(Vec ud)
  {
  Vec x=getx();
  Vec v=getv();
  
  double c1,c2;
  double u1,u11,u12,u13;
  double u2,u21,u22,u23,u24;
  double u3,u31,u32,u33,u34;
  double margin,target,slratio,speedlimit;
  double g1,g2,g3;

  g1=SAFETY_GAINS[type][0];
  g2=SAFETY_GAINS[type][1];
  g3=SAFETY_GAINS[type][2];
  
  margin=SAFETY_SPEED_MARGINS[type][0];
  target=SAFETY_SPEED_TARGETS[type];
  slratio=SAFETY_SPEED_RATIO[type];
  speedlimit=SAFETY_SPEED_LIMITS[type];
  
  c1=TURRET_JOINT_LIMIT[type][0];c2=TURRET_JOINT_LIMIT[type][1];
  
  u11=singlecontrol(c1,c2,x[0],v[0],-target,margin,g1);
  u12=singlecontrol(-c1,-c2,x[0],v[0],target,margin,g1);
  
  u1=u11+u12;
  
  
  c1=SHIN_JOINT_LIMIT[type][0];c2=TURRET_JOINT_LIMIT[type][1];  
  u21=-singlecontrol(c1,c2,x[1],v[1],-target,margin,g2);
  
  c1=SHIN_THIGH_MIN_JOINT_LIMIT[type][0];c2=SHIN_THIGH_MIN_JOINT_LIMIT[type][1];
  u22=-singlecontrol(c1,c2,x[1]-x[2],v[1]-v[2],target,margin,g2);
  
  c1=SHIN_THIGH_MAX_JOINT_LIMIT[type][0];c2=SHIN_THIGH_MAX_JOINT_LIMIT[type][1];  
  u23=-singlecontrol(c1,c2,x[1]-x[2],v[1]-v[2],-target,margin,g2);
  
  u2=u21+u22+u23;
  
  
  c1=THIGH_JOINT_LIMIT[type][0];c2=THIGH_JOINT_LIMIT[type][1]; 
  u31=-singlecontrol(c1,c2,x[2],v[2],target,margin,g3);
  
  c1=SHIN_THIGH_MIN_JOINT_LIMIT[type][0];c2=SHIN_THIGH_MIN_JOINT_LIMIT[type][1];  
  u32=-singlecontrol(-c1,-c2,x[2]-x[1],v[2]-v[1],-target,margin,g3);
  
  c1=SHIN_THIGH_MAX_JOINT_LIMIT[type][0];c2=SHIN_THIGH_MAX_JOINT_LIMIT[type][1];
  u33=-singlecontrol(-c1,-c2,x[2]-x[1],v[2]-v[1],target,margin,g3);
  
  u3=u31+u32+u33;
  
  
  Vec uu(3);
  
  uu[0]=u1;
  uu[1]=u2;
  uu[2]=u3;
  
  Vec uc=coefs(x,v);
  
  //printf("%lf %lf %lf\n",uc[0],uc[1],uc[2]);
  
  u13=singlespeedlimit(speedlimit,speedlimit*slratio,v[0],g1);
  u24=-singlespeedlimit(speedlimit,speedlimit*slratio,v[1],g2);
  u34=-singlespeedlimit(speedlimit,speedlimit*slratio,v[2],g3);
  
  Vec u(3);
  
  for(unsigned int q1=0;q1<u.n;q1++)u[q1]=uu[q1]*(1.0-uc[q1])+ud[q1]*uc[q1];
  
  
  u[0]=clamp(u13+clamp(u[0]));
  u[1]=clamp(u24+clamp(u[1]));
  u[2]=clamp(u34+clamp(u[2]));

  u[0]=climb(lastu[0],u[0]);
  u[1]=climb(lastu[1],u[1]);
  u[2]=climb(lastu[2],u[2]);
  
  lastu=u;
  
  return u;
  }
};




static timer t;

static net_server ns;

static phantom*p1;

void init_control(void)
  {
  p1=new phantom(0);
  ns.init(19999,128,0,0,0);
  }










void control(double*fin,double*fout,long*lin,long*lout)
  {
  double tosend[16];
  
  Vec raw1(3);
  raw1[0]=(double)lin[0];
  raw1[1]=(double)lin[1];
  raw1[2]=(double)lin[2];
  
  p1->addsample(raw1);
  
  
  
  Vec uu1(3);
  Vec uu2(3);


  uu1[0]=2000.0;
  if(t(1)>1.0)uu1[0]=-2000.0;
  if(t(1)>2.0){uu1[0]=2000.0;t.start(1);}
  
  uu1[0]=0.0;
  


  double received[16];
  memset(received,0,sizeof(received));
  //ns.receive((char*)received);

  uu2[0]=received[0];
  uu2[1]=received[1];
  uu2[2]=received[2];

  //uu2.print();

  Vec u1=p1->getu(uu1);
  
  p1->discretize(u1,lout+0);
  
  Vec x1=p1->getx();
  Vec v1=p1->getv();
  
  for(int q1=0;q1<3;q1++)tosend[q1]=x1[q1];
  for(int q1=0;q1<3;q1++)tosend[q1+3]=v1[q1];
  for(int q1=0;q1<3;q1++)tosend[q1+6]=(double)lout[q1];
  for(int q1=0;q1<3;q1++)tosend[q1+9]=fin[q1];
  
  
  
  
  t.start(11);  	
  ns.update((char*)tosend);
  double r1=t(11);
  static int count1=0;
  count1++;
  //if(count1%100==0 || r1>0.00001)printf("%d - %lf\n",count1,r1*1000);
  }