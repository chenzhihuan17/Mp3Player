/*************************************************************************
	> File Name: echo_queue.c
	> Author: XXDK
	> Created Time: Thu 09 Aug 2018 05:49:53 PM CST
 ************************************************************************/

#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

LinkQueue *create_empty_linkqueue()
{
	LinkNode *head;
	LinkQueue *q;

	head = (LinkNode *)malloc(sizeof(LinkNode));
	if(head == NULL){
		printf("Fail to create empty linkqueue!\n");
		return NULL;
	}
	head->next = NULL;
	q = (LinkQueue *)malloc(sizeof(LinkQueue));
	if(q == NULL){
		printf("Fail to create empty linkqueue!\n");	
		return NULL;
	}
	q->front = head;
	q->rear  = head;

	return q;
}

int is_empty_linkqueue(LinkQueue *q)
{
	return q->front == q->rear ? 1 : 0;
}

int push_linkqueue(LinkQueue *q,datatype data)
{
	LinkNode *tmp;
	tmp = (LinkNode *)malloc(sizeof(LinkNode));
	if(tmp == NULL){
		printf("Fail to enter linkqueue!\n");
		return -1;
	}
	tmp->data = data;
	q->rear->next = tmp; ///< insert to the end of the list
	q->rear = tmp;
	//printf("xxxxxxxxxxxxxxxxxxxx ------------ xxxxx enter linkqueue!\n");

	return 0;
}

datatype pop_linkqueue(LinkQueue *q)
{
	LinkNode *tmp;

	if(is_empty_linkqueue(q)){
		//printf("The linkqueue is empty!\n");
		return (datatype)-1;
	}

	tmp = q->front;
	q->front = tmp->next; ///< pop the head of the list.
	free(tmp);

	return q->front->data;
}

