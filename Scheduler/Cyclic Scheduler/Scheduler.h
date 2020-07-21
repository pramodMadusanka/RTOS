#ifndef QUEUE_H
#define QUEUE_H

#include <Arduino.h>
#include <stdio.h>

class Scheduler{
	public:
		Scheduler(void);
		char* schedule(int period[], int frame_size, int no_of_tasks);
		Queue PCB;
	private:
		int gcd(int a, int b);
		int LCM(int arr[], int n);
		int major_cycle;
		int no_of_frames;
};

#endif
