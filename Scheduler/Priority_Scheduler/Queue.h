#ifndef QUEUE_H
#define QUEUE_H

#include <Arduino.h>
#include <stdio.h>

typedef struct PCB{
	void (*taskptr)(void* arg);
	uint8_t priority;
	int wakeupTime;
	int BP, SP;
	struct PCB* next;
};

class Queue{
	public:
		Queue(void);
		struct strnode* start;
		struct strnode* end;
		void addTask(char name, void (*taskptr)(void* arg), uint8_t priority);
		void enqueue(struct strnode* node, int wakeupTime);
		struct strnode* dequeue();
		void printQueue();
		void quickSort(struct strnode** headRef);
		struct strnode* getStart();
		void addNode(struct strnode* head, struct strnode* node);
		
	private:
		int nextBP = 0x08FF;
		struct strnode* tmp;
		struct strnode* createNode(char name, void (*taskptr)(void* arg), uint8_t priority, int wakeupTime);
		struct strnode* getTail(struct strnode* cur);
		struct strnode* partition(struct strnode *head, struct strnode *end, struct strnode **newHead, struct strnode **newEnd);
		struct strnode* quickSortRecur(struct strnode *head, struct strnode *end);
};

#endif
