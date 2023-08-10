#include "co.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>

co *head, *tail, *currentThread;
int idCount = 0;	// initial coroutinu number
co* removeDead(co* node)
{
   co* tmp1=node->prev;
   co* tmp2=node->next;
   tmp1->next=tmp2;
   if(tmp2){
    tmp2->prev=tmp1;
   }
   free(node);
   return tmp1;
}

// return the tail pointer into which to insert  
co *insertTail(co *tail, co *node)
{
	tail->next = node;
	node->next = NULL;
	node->prev = tail;
  tail=node;
	return tail;
}

// sp->$rsp; arg->%rdi; jmp *entry;
static inline void stack_switch_call(void *sp, void *entry, void *arg) {
  asm volatile (
#if __x86_64__
    "movq %0, %%rsp;movq %2, %%rdi; call *%1"
      : : "b"((unsigned char*)sp), "d"(entry), "a"(arg) : "memory"
#else
    "movl %0, %%esp; movl %2, 4(%0); call *%1"
      : : "b"((unsigned char*)sp - 8), "d"(entry), "a"(arg) : "memory"
#endif
  );
}


co *co_start(const char *name, void (*func)(void *), void *arg) {
	// initialize a co struct of coroutine
  // if idcount==1 create head tail currentThread
  if(idCount==0)
  {
    srand(time(NULL));
    head=tail=currentThread=(co*)malloc(sizeof(co));
    head->prev=NULL;
    head->next=NULL;
  }
	co *thread = (co*)malloc(sizeof(co));
	strcpy(thread->name,name);
	thread->func = func;
	thread->arg = arg;
	thread->cid = ++idCount;	
	thread->status = CO_NEW;
	
	tail = insertTail(tail, thread);

	return thread; 
}

void co_wait(co *co) {

	while(co->status != CO_DEAD) {
		co_yield();
	}
	assert(co->status == CO_DEAD);
	// release resource
  removeDead(co);
}

// Search the next thread to run if the thread's status is CO_NEW
co *searchNextNewThread() //寻找下一个可以运行的协程
{
	co *nextThread = head;
	int randomThreadIndex = rand() % idCount + 1;
	while(randomThreadIndex--) 
		nextThread = nextThread->next;	
	
	return nextThread; 
}

void co_yield() {
  co* tmp=currentThread;
	int val = setjmp(currentThread->env);
	if(val == 0) {
		// which indicats it was called by setjmp directly
		co *cur = searchNextNewThread();	
    currentThread=cur; 
		if(cur->status == CO_NEW) {
			cur->status = CO_ALIVE;
			stack_switch_call(cur->stack+STACK_SIZE-1, cur->func, cur->arg);			
		}else if(cur->status==CO_ALIVE)
		{
			longjmp((cur->env), cur->cid);
		}
		cur->status = CO_DEAD;	// 
	}
  currentThread=tmp;
}