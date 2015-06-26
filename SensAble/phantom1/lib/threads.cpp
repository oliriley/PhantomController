#include "win.h"

#include "common.h"
#include "threads.h"

struct thread_launcher {thread_function f;void*data;};

DWORD WINAPI doit(LPVOID a)
  {
  struct thread_launcher*s=(struct thread_launcher*)a;
  s->f(s->data);
  return 0;
  }
  
unsigned int __stdcall doit2(void*a)
  {
	struct thread_launcher*s=(struct thread_launcher*)a;
  s->f(s->data);
	return 0;
  }

void doit3(void*a)
  {
	struct thread_launcher*s=(struct thread_launcher*)a;
  s->f(s->data);
  }

thread::thread(void){th=NULL;}

thread::thread(thread_function f,void*p)
  {
  struct thread_launcher*d=(struct thread_launcher*)malloc(sizeof(thread_launcher));
  d->data=p;
  d->f=f;
  //th=CreateThread(0,0,doit,d,0,NULL);
  th=(HANDLE)_beginthreadex(NULL,0,doit2,d,0,NULL);
  //th=(HANDLE)_beginthread(doit3,0,d);
  }
void thread::detach(void)
  {
  }
void thread::join(void)
  {
  WaitForSingleObject(th,INFINITE);
  CloseHandle(th);
  }


mutex::mutex(void) {InitializeCriticalSectionAndSpinCount(&m,0);}
void mutex::lock(void){EnterCriticalSection(&m);}
void mutex::unlock(void){LeaveCriticalSection(&m);}
LPCRITICAL_SECTION mutex::getcs(void){return &m;}

lock_guard::lock_guard(mutex&m1){m=&m1;m->lock();}
lock_guard::~lock_guard(void){m->unlock();}

/*condition_variable::condition_variable(void){InitializeConditionVariable(&c);}
void condition_variable::wait(mutex*m){SleepConditionVariableCS(&c,m->getcs(),INFINITE);}
void condition_variable::wake(void){WakeConditionVariable(&c);}*/

condition_variable::condition_variable(void){c=CreateEventA(NULL,TRUE,FALSE,NULL);}
void condition_variable::wait(mutex*m){m->unlock();WaitForSingleObject(c,INFINITE);ResetEvent(c);m->lock();}
void condition_variable::wake(void){SetEvent(c);}
 
