#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <Arduino.h>
#include <stdio.h>
#include <Queue.h>

class Scheduler{
	public:
		node** calcSchedule(int major_cycle, float frame_size, int min_period);
		node** schedule;
		Queue queue;

	private:

};

#endif
