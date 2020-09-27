#ifndef QUEUE_H
#define QUEUE_H

#include <Arduino.h>
#include <stdio.h>

typedef struct PCB{
	uint8_t period;
	uint8_t exec_time;
	void (*taskptr)(void* arg);
	uint8_t checked;
	struct PCB* next;
};

class Queue{
	public:
		Queue(void);
		void addTask(int period, uint8_t exec_time, void (*taskptr)(void* arg));
		void enqueue(int period, uint8_t exec_time, void (*taskptr)(void* arg), uint8_t checked);
		struct PCB* dequeue();
		int getMajorCycle();
		int getMinPeriod();

	private:
		struct PCB* tmp;
		struct PCB* start;
		struct PCB* end;
		struct PCB* createPCB(int period, uint8_t exec_time, void (*taskptr)(void* arg), uint8_t checked);
		int* getPeriods();
		uint8_t getNoOfTasks();
		int GCD(int a, int b);
};

#endif
