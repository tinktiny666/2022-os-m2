#ifndef __LINKEDLIST__
#define __LINKEDLIST
#include "co.h"
struct threadInfo{
	co *thread;
	struct threadInfo *next;
	struct threadInfo *prev;	
};

typedef struct threadInfo threadNode;

// function prototype
threadNode *init();
threadNode *insertTail(threadNode *, threadNode *);

#endif