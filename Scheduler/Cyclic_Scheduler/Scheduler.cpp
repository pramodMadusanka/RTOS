#include <stdio.h>
#include "Arduino.h"
#include "Queue.h"
#include "Scheduler.h"

node** Scheduler::calcSchedule(int major_cycle, float frame_size, int min_period){
	int no_of_frames = major_cycle/frame_size;
	schedule = (node**)malloc(sizeof(node*)*no_of_frames);
	memset(schedule, 0, sizeof(node*)*no_of_frames);
	int submajor_cycle_time = min_period;
	int tasks_executed_frames = 0;
	node* tx;
	while(1){
		tx = queue.dequeue();
		if(tx == NULL)
			return schedule;
		if((*tx).checked != 0){
			int place;
			for(place=submajor_cycle_time*((*tx).checked); *(schedule+place)!=NULL; place++)
				tasks_executed_frames = place+1;
		}
		if((submajor_cycle_time*((*tx).checked+1)-tasks_executed_frames)>=(*tx).exec_time/frame_size){
			int place, place_;
			for(place=tasks_executed_frames; place<no_of_frames; place+=(*tx).period/frame_size){
				for(place_=place; place_<place+((*tx).exec_time)/frame_size; place_++){  
					if(*(schedule+place_)!=NULL)
						return NULL;
					*(schedule+place_) = tx;
				}
			}
			tasks_executed_frames+=((*tx).exec_time)/frame_size;
		} else {
			queue.enqueue((*tx).name, (*tx).period, (*tx).exec_time, (*tx).taskptr, (*tx).checked+1);
		}
	}
}
