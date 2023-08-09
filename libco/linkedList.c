#include "linkedList.h"
#include <stddef.h>
#include <stdlib.h>

threadNode *init()
{
	threadNode* node=(threadNode*)malloc(sizeof(threadNode));
}

// return the tail pointer into which to insert  
threadNode *insertTail(threadNode *tail, threadNode *node)
{
	tail->next = node;
	node->next = NULL;
	node->prev = tail;

	return node;
}
