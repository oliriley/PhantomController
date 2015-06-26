#ifndef LA_H
#define LA_H

#include "common.h"

typedef double type;
typedef double realtype;
typedef unsigned int uint;

class Vec
  {public:
  uint n;
  type*ss;
  Vec();
  Vec(uint);
  Vec(const Vec &);
  ~Vec();
  Vec&operator=(const Vec &);
  type&operator[](uint);
  int print(void);
  Vec range(uint,uint);
};

class RVec : public Vec {};

class Mat
  {public:
  type*ss;
  uint x,y;
  Mat();
  Mat(uint,uint);
  Mat(const Mat &);
  ~Mat();
  type*operator[](uint);
  Mat&operator=(const Mat &);
  int print(void);
  int swap_row(uint,uint);
  Vec row(uint);
  Vec column(uint);
  int set_row(uint,Vec);
  int set_column(uint,Vec);
  Mat range(uint,uint);
};


Mat operator*(Mat ,Mat );

Vec operator*(Mat ,Vec );
  
Vec operator*(Vec ,Mat );

Vec operator-(Vec ,Vec );
Vec operator+(Vec ,Vec );

Vec operator*(Vec ,type );
Vec operator*(type ,Vec );
Vec operator/(Vec ,type );

type operator*(Vec ,Vec );
Mat operator^(Vec ,Vec );

Mat operator*(Mat ,type );  
Mat operator*(type ,Mat );
Mat operator/(Mat ,type );
Mat operator+(Mat ,Mat );
Mat operator-(Mat ,Mat );

Mat transpose(Mat );
Mat invert(Mat );
Mat midentity(uint);


realtype norm(type);
realtype norm(Vec );
int memswap(void *,void *,uint);


class LUP : public Mat
{public:
uint*p;
LUP(const Mat &);
~LUP();
int lup(void);
Mat L(void);
Mat U(void);
Mat P(void);
Vec solve(Vec & b);
Mat invert(void);
};


#endif

