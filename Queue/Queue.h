#ifndef QUEUE_H
#define QUEUE_H

#include <Arduino.h>
#include <stdio.h>

typedef struct strnode{
	char name;
	int period;
	int exec_time;
	void (*taskptr)(void* arg);
	int checked;
	struct strnode* next;
	int* periods;
	int noOfTasks;
	int i;
}node;

class Queue{
	public:
		Queue(void);
		void enqueue(char name, int period, int exec_time, void (*taskptr)(void* arg), int checked);
		struct strnode* dequeue();
		void printList();
		int* getPeriods();
		int getNoOfTasks();
	private:
		struct strnode* tmp;
		struct strnode* start;
		struct strnode* end;
		struct strnode* createNode(char name, int period, int exec_time, void (*taskptr)(void* arg), int checked);
};

#endif
