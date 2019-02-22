/*************************************************************************
	> File Name: echo_queue.h
	> Author: XXDK
	> Created Time: Thu 09 Aug 2018 05:48:51 PM CST
 ************************************************************************/

#ifndef _QUEUE_H
#define _QUEUE_H

typedef char* datatype;

typedef struct linknode
{
	datatype data;
	struct linknode* next;
}LinkNode;

typedef struct{
	LinkNode *front;
	LinkNode *rear;
}LinkQueue;

extern LinkQueue *create_empty_linkqueue();

extern int is_empty_linkqueue(LinkQueue *q);

extern int push_linkqueue(LinkQueue *q, datatype data);

extern datatype pop_linkqueue(LinkQueue *q);

#endif // _QUEUE_H
