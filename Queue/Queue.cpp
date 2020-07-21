#include <stdio.h>
#include "Arduino.h"
#include "Queue.h"

Queue::Queue(void){
	start = NULL;
	end   = NULL;
}

struct strnode* Queue::createNode(char name, int period, int exec_time, void (*taskptr)(void* arg), int checked){
    tmp=(struct strnode*)malloc(sizeof(struct strnode));
    tmp->name=name;
    tmp->period=period;
    tmp->exec_time=exec_time;
    tmp->taskptr=taskptr;
    tmp->checked=checked;
    tmp->next=NULL;

    return tmp;
}

void Queue::enqueue(char name, int period, int exec_time, void (*taskptr)(void* arg), int checked){
    if(start==NULL){
        start=createNode(name, period, exec_time, taskptr, checked);
        end=start;
    } else {
        end->next=createNode(name, period, exec_time, taskptr, checked);
        end=end->next;
    }
}

struct strnode* Queue::dequeue(){
    tmp = NULL;
	if(start == NULL){
		return tmp;
	}
    tmp=start;

    if(start==end){
        start=NULL;
        end=NULL;
    } else{
        start=start->next;
    }
    return tmp;
}

/*void Queue::printList(){
    for(tmp=start; tmp!=NULL; tmp=tmp->next){
        Serial.println(tmp->taskptr);
    }
}*/

int* Queue::getPeriods(){
	int i;
	int periods[getNoOfTasks()];
	for(tmp=start; tmp!=NULL; tmp=tmp->next)
		periods[i] = tmp->period;
	return periods;
}

int Queue::getNoOfTasks(){
	int noOfTasks=0;
	for(tmp=start; tmp!=NULL; tmp=tmp->next)
		noOfTasks++;
	return noOfTasks;
}
