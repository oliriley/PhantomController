#include "la.h"

realtype norm(type a){return a<0.0?-a:a;}
realtype norm(Vec a){return sqrt(a*a);}
realtype norm(Vec & a){return sqrt(a*a);}

int memswap(void * a,void * b,uint length)
  {void * c=malloc(length);
  memcpy(c,a,length);
  memcpy(a,b,length);
  memcpy(b,c,length);
  return 0;}

Mat::Mat()
  {x=y=0;
  ss=NULL;}
Mat::Mat(uint a,uint b)
  {ss=(type*)malloc(sizeof(type)*a*b);
  x=a;y=b;
  memset(ss,0,sizeof(type)*x*y);}
Mat::Mat(const Mat & a)
  {x=a.x;y=a.y;
  ss=(type*)malloc(sizeof(type)*x*y);
  memcpy(ss,a.ss,sizeof(type)*x*y);}
Mat::~Mat(){if(ss!=NULL)free(ss);}
type* Mat::operator[](uint a){return ss+y*a;}
Mat& Mat::operator=(const Mat & a)
  {if(this==&a)return *this;
  this->~Mat();
  x=a.x;y=a.y;
  ss=(type*)malloc(sizeof(type)*x*y);
  memcpy(ss,a.ss,sizeof(type)*x*y);
  return *this;}
int Mat::print(void)
  {uint q1,q2;
  printf("%d %d\n",x,y);
  for(q1=0;q1<x;q1++)
    {for(q2=0;q2<y-1;q2++)printf("%lf ",(*this)[q1][q2]);
    printf("%lf\n",(*this)[q1][y-1]);}
  printf("\n");
  return 0;}

int Mat::swap_row(uint a,uint b){memswap((*this)[a],(*this)[b],sizeof(type)*y);return 0;}

Vec Mat::row(uint a)
  {Vec b(y);
  memcpy(b.ss,(*this)[a],sizeof(type)*y);
  return b;}
  
Vec Mat::column(uint a)
  {Vec b(x);
  for(uint q1=0;q1<x;q1++)b[q1]=(*this)[q1][a];
  return b;}
  
int Mat::set_row(uint a,Vec  b)
  {memcpy((*this)[a],b.ss,sizeof(type)*y);
  return 0;}

int Mat::set_column(uint a,Vec b)
  {for(uint q1=0;q1<x;q1++)((*this)[q1][a])=(b[q1]);
  return 0;}

Mat Mat::range(uint a,uint b)
  {Mat n(b-a+1,this->y);
  for(uint q1=a;q1<=b;q1++)n.set_row(q1-a,this->row(q1));
  return n;}

Vec::Vec()
  {n=0;
  ss=NULL;}
Vec::Vec(uint len)
  {n=len;
  ss=(type*)malloc(sizeof(type)*n);
  memset(ss,0,sizeof(type)*n);}
Vec::Vec(const Vec & a)
  {n=a.n;
  ss=(type*)malloc(sizeof(type)*n);
  memcpy(ss,a.ss,sizeof(type)*n);}
Vec::~Vec(){if(ss!=NULL)free(ss);}
Vec& Vec::operator=(const Vec & a)
  {this->~Vec();
  this->n=a.n;
  ss=(type*)malloc(sizeof(type)*n);
  memcpy(ss,a.ss,sizeof(type)*n);
  return *this;}
type& Vec::operator[](uint a)
  {return ss[a];}
int Vec::print(void)
  {uint q1;
  printf("%d\n%lf",n,ss[0]);
  for(q1=1;q1<n;q1++)printf(" %lf",ss[q1]);
  printf("\n\n");
  return 0;}
  

Vec Vec::range(uint a,uint b)
  {Vec n(b-a+1);
  for(uint q1=a;q1<=b;q1++)n[q1-a]=(*this)[q1];
  return n;}
  


Mat operator*(Mat  a,Mat  b)
  {Mat c(a.x,b.y);
  uint q1,q2,q3;
  type d1;
  for(q1=0;q1<c.x;q1++)for(q2=0;q2<c.y;q2++)
    {d1=0.0;
    for(q3=0;q3<a.y;q3++)d1+=a[q1][q3]*b[q3][q2];
    c[q1][q2]=d1;}
  return c;}

Vec operator*(Mat  a,Vec b)
  {Vec c(a.x);
  uint q1,q2;
  type d1;
  for(q1=0;q1<c.n;q1++)
    {d1=0.0;
    for(q2=0;q2<a.y;q2++)d1+=a[q1][q2]*b[q2];
    c[q1]=d1;}
  return c;}
  
Vec operator*(Vec  a,Mat b)
  {Vec c(b.y);
  uint q1,q2;
  type d1;
  for(q1=0;q1<c.n;q1++)
    {d1=0.0;
    for(q2=0;q2<b.x;q2++)d1+=b[q2][q1]*a[q2];
    c[q1]=d1;}
  return c;}

Vec operator-(Vec  a,Vec  b)
  {Vec c(a.n);
  for(uint q1=0;q1<a.n;q1++)c[q1]=a[q1]-b[q1];
  return c;}
  
Vec operator+(Vec  a,Vec  b)
  {Vec c(a.n);
  for(uint q1=0;q1<a.n;q1++)c[q1]=a[q1]+b[q1];
  return c;}

Vec operator*(Vec a,type b)
  {Vec c(a.n);
  for(uint q1=0;q1<c.n;q1++)c[q1]=a[q1]*b;
  return c;}

Vec operator*(type b,Vec a)
  {Vec c(a.n);
  for(uint q1=0;q1<c.n;q1++)c[q1]=a[q1]*b;
  return c;}

Vec operator/(Vec a,type b)
  {Vec c(a.n);
  for(uint q1=0;q1<c.n;q1++)c[q1]=a[q1]/b;
  return c;}
  
type operator*(Vec  a,Vec  b)
  {type d1=0.0;
  for(uint q1=0;q1<a.n;q1++)d1+=a[q1]*b[q1];
  return d1;}

Mat operator^(Vec a,Vec  b)
  {Mat c(a.n,b.n);
  for(uint q1=0;q1<a.n;q1++)for(uint q2=0;q2<b.n;q2++)
    c[q1][q2]=a[q1]*b[q2];
  return c;}
  
Mat operator*(Mat  a,type b)
  {Mat c(a.x,a.y);
  for(uint q1=0;q1<c.x;q1++)for(uint q2=0;q2<c.y;q2++)c[q1][q2]=a[q1][q2]*b;
  return c;}
  
Mat operator*(type  b,Mat  a)
  {Mat c(a.x,a.y);
  for(uint q1=0;q1<c.x;q1++)for(uint q2=0;q2<c.y;q2++)c[q1][q2]=a[q1][q2]*b;
  return c;}

Mat operator/(Mat  a,type  b)
  {Mat c(a.x,a.y);
  for(uint q1=0;q1<c.x;q1++)for(uint q2=0;q2<c.y;q2++)c[q1][q2]=a[q1][q2]/b;
  return c;}
    
Mat operator+(Mat a,Mat  b)
  {Mat c(a.x,a.y);
  for(uint q1=0;q1<c.x;q1++)for(uint q2=0;q2<c.y;q2++)c[q1][q2]=a[q1][q2]+b[q1][q2];
  return c;}

Mat operator-(Mat  a,Mat  b)
  {Mat c(a.x,a.y);
  for(uint q1=0;q1<c.x;q1++)for(uint q2=0;q2<c.y;q2++)c[q1][q2]=a[q1][q2]-b[q1][q2];
  return c;}
 
Mat transpose(Mat  a)
  {Mat b(a.y,a.x);
  for(uint q1=0;q1<a.y;q1++)for(uint q2=0;q2<a.x;q2++)b[q1][q2]=a[q2][q1];
  return b;}

Mat invert(Mat  a)
  {LUP b(a);
  return b.invert();}

Mat midentity(uint n)
  {Mat a(n,n);
  for(uint q1=0;q1<n;q1++)a[q1][q1]=1.0;
  return a;}


LUP::LUP(const Mat & a) : Mat(a)
  {p=(uint*)malloc(sizeof(uint)*x);
  for(uint q1=0;q1<x;q1++)p[q1]=q1;
  lup();}
  
LUP::~LUP(){free(p);}

int LUP::lup(void)
  {uint q1,q2,q3,c1,c2;
  for(q1=0;q1<x-1;q1++)
    {for(c1=q2=q1;q2<x;q2++)if(norm((*this)[q2][q1])>norm((*this)[c1][q1]))c1=q2;
    swap_row(c1,q1);
    c2=p[q1];p[q1]=p[c1];p[c1]=c2;
    for(q2=q1+1;q2<x;q2++)(*this)[q2][q1]/=(*this)[q1][q1];
    for(q2=q1+1;q2<x;q2++)for(q3=q1+1;q3<y;q3++)
      (*this)[q2][q3]-=((*this)[q2][q1])*((*this)[q1][q3]);
    }
  return 0;} 
  
Mat LUP::L(void)
  {Mat l(*this);
  uint q1,q2;
  for(q1=0;q1<x;q1++)for(q2=q1+1;q2<y;q2++)l[q1][q2]=0.0;
  for(q1=0;q1<x;q1++)l[q1][q1]=1.0;
  return l;}
  
Mat LUP::U(void)
  {Mat u(*this);
  uint q1,q2;
  for(q1=0;q1<x;q1++)for(q2=0;q2<q1;q2++)u[q1][q2]=0.0;
  return u;}
  
Mat LUP::P(void)
  {Mat pp(x,y);
  uint q1;
  for(q1=0;q1<x;q1++)pp[q1][p[q1]]=1.0;
  return pp;}

Vec LUP::solve(Vec & b)
  {Vec x(this->y);
  Vec t(this->y);
  uint q1,q2;
  for(q1=0;q1<x.n;q1++)x[q1]=b[this->p[q1]];
  for(q1=0;q1<t.n;q1++)
    {type d1=0.0;
    for(q2=0;q2<q1;q2++)d1+=t[q2]*(*this)[q1][q2];
    t[q1]=x[q1]-d1;}
  for(q1=this->x-1;q1!=-1;q1--)
    {type d1=0.0;
    for(q2=q1+1;q2<this->y;q2++)
      d1+=x[q2]*(*this)[q1][q2];
    x[q1]=(t[q1]-d1)/(*this)[q1][q1];}
  return x;}

Mat LUP::invert(void)
  {Mat res(this->x,this->y);
  Vec e1(this->x);
  uint q1;
  for(q1=0;q1<this->x;q1++)
    {e1[q1]=1.0;
    res.set_column(q1,solve(e1));
    e1[q1]=0.0;}
  return res;}
