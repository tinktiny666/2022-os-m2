#ifndef __CO_C__
#define __CO_C__
#define STACK_SIZE	1024*256

#include <setjmp.h>

typedef enum {
	CO_NEW = 1,	// new coroutine,还未执行
	CO_ALIVE, 
	CO_DEAD		// 已经结束，但还未释放资源 	
}co_status;

typedef struct co_thread{
	char name[64];
	int cid;
	void (*func)(void *arg);
	void *arg;

	co_status status;	
	jmp_buf env;	// context
	unsigned char stack[STACK_SIZE];	// stack of coroutine
  struct co_thread* prev;
  struct co_thread* next;
}co;

// function prototype
co* co_start(const char *name, void (*func)(void *), void *arg);
void co_yield();
void co_wait(co *co);


// return the tail pointer into which to insert  
co *insertTail(co *tail, co *node);
co* removeDead(co* node);

#endif
