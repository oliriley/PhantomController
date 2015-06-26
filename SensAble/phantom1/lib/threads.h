#ifndef THREADS_H
#define THREADS_H

#include "win.h"


class thread
{
HANDLE th;

public: thread(void);
public: thread(thread_function,void*);
public: void detach(void);
public: void join(void);

};


class mutex
{
CRITICAL_SECTION m;

public: mutex(void);
public: LPCRITICAL_SECTION getcs(void);
public: void lock(void);
public: void unlock(void);
};

class lock_guard
{
mutex*m;
public: lock_guard(mutex&m1);
public: ~lock_guard(void);
};

#define LG(a) lock_guard lg(a)


class condition_variable
{
//CONDITION_VARIABLE c;
HANDLE c;
public: condition_variable(void);
public: void wait(mutex*m);
public: void wake(void);
};



#endif