#ifndef QUEUE_H
#define QUEUE_H

#include <Arduino.h>
#include <stdio.h>

typedef struct strnode{
	char name;
	int period;
	uint8_t exec_time;
	void (*taskptr)(void* arg);
	uint8_t checked;
	struct strnode* next;
}node;

class Queue{
	public:
		Queue(void);
		void enqueue(char name, int period, uint8_t exec_time, void (*taskptr)(void* arg));
		void enqueue(char name, int period, uint8_t exec_time, void (*taskptr)(void* arg), uint8_t checked);
		struct strnode* dequeue();
		void printList();
		int* getPeriods();
		uint8_t getNoOfTasks();
	private:
		struct strnode* tmp;
		struct strnode* start;
		struct strnode* end;
		struct strnode* createNode(char name, int period, uint8_t exec_time, void (*taskptr)(void* arg), uint8_t checked);
};

#endif
