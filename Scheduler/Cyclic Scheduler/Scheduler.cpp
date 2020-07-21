#include <stdio.h>
#include "Arduino.h"
#include "Queue.h"
#include "Scheduler.h"

Scheduler::Scheduler(void){
	Queue PCB;
}

char* Scheduler::schedule(int period[], int frame_size, int no_of_tasks){
	int major_cycle = LCM(period, no_of_tasks);
	int no_of_frames = major_cycle/frame_size;
	printf("bitch\n");
	printf("no_of_frames: %d\n", no_of_frames);
	char schedule[60] = {0};
	int submajor_cycle_time = LCM(period, no_of_tasks);
	int tasks_executed_frames = 0;
	struct node* tx;

	while(1){
		printf("-----------------\n");
		tx = dequeue();
		if(tx == NULL){
			int i;
			for(i=0; i<60; i++){
				printf("schedule[%d]\t: %c\n", i, schedule[i]);
			}
			printf("returning\n");
			return schedule;
		}
		if((*tx).checked != 0){
			int place;
			for(place=submajor_cycle_time*((*tx).checked); schedule[place]!=0; place++){
				tasks_executed_frames = place+1; //+1
			}
		}
		if((submajor_cycle_time*((*tx).checked+1)-tasks_executed_frames)>=(*tx).exec_time){
			int place, place_;
			for(place=tasks_executed_frames; place<no_of_frames; place+=(*tx).period){
				for(place_=place; place_<place+(*tx).exec_time; place_++){	
					if(schedule[place_]!=0){
						printf("returning error\n");
						return NULL;
					}
					schedule[place_] = (*tx).name;
					//printf("schedule[place_] : %c\n", schedule[place_]);
				}
			}
			tasks_executed_frames+=(*tx).exec_time;
			if(submajor_cycle_time>(*tx).period){
				submajor_cycle_time = (*tx).period;
			}
		} else {
			printf("enqueued %c\n", (*tx).name);
			enqueue((*tx).name, (*tx).period, (*tx).exec_time, (*tx).checked+1);
		}
	}
}

int Scheduler::gcd(int a, int b){
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

int Scheduler::LCM(int arr[], int n){
    int ans = arr[0];
    for (int i = 1; i < n; i++)
        ans = (((arr[i] * ans)) / (gcd(arr[i], ans)));
    return ans;
}

int main(){
    int period[3] = { 10, 20, 60 };	
    enqueue('A', 10, 4, 0);
    enqueue('B', 20, 6, 0);
    enqueue('C', 60, 5, 0);
    printf("fuck\n");

	char* pat = scheduler(period, 1, 3);

	int i;
	printf("%ld\n", pat);

	/*for(i=0; i<60; i++){
		printf("%c\t", *(pat+i));
	}*/
}

