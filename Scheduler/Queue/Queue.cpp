#include <stdio.h>
#include "Arduino.h"
#include "Queue.h"

Queue::Queue(void){
	start = NULL;
	end   = NULL;
}

struct strnode* Queue::createNode(char name, int period, uint8_t exec_time, void (*taskptr)(void* arg), uint8_t checked){
    tmp=(struct strnode*)malloc(sizeof(struct strnode));
    tmp->name=name;
    tmp->period=period;
    tmp->exec_time=exec_time;
    tmp->taskptr=taskptr;
    tmp->checked=checked;
    tmp->next=NULL;

    return tmp;
}

void Queue::addTask(char name, int period, uint8_t exec_time, void (*taskptr)(void* arg)){
    if(start==NULL){
        start=createNode(name, period, exec_time, taskptr, 0);
        end=start;
    } else {
        end->next=createNode(name, period, exec_time, taskptr, 0);
        end=end->next;
    }
}

void Queue::enqueue(char name, int period, uint8_t exec_time, void (*taskptr)(void* arg), uint8_t checked){
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

int* Queue::getPeriods(){
	uint8_t i = 0;
	int* periods = (int*)malloc(sizeof(int)*getNoOfTasks());
	for(tmp=start; tmp!=NULL; tmp=tmp->next){
		*(periods+i) = tmp->period;
		i++;
	}
	return periods;
}

uint8_t Queue::getNoOfTasks(){
	uint8_t noOfTasks=0;
	for(tmp=start; tmp!=NULL; tmp=tmp->next)
		noOfTasks++;
	return noOfTasks;
}

int Queue::GCD(int a, int b){
	if (b == 0)
		return a;
	return GCD(b, a % b);
}

int Queue::getMajorCycle(){
	int* periods = getPeriods();
	int ans = *periods;
	for (int i = 1; i < getNoOfTasks(); i++)
		ans = (((*(periods+i) * ans)) / (GCD(*(periods+i), ans)));
	free(periods);
	return ans;
}

int Queue::getMinPeriod(){
	int minPeriod = start->period;
	for(tmp=start->next; tmp!=NULL; tmp=tmp->next){
		if(minPeriod > tmp->period)
			minPeriod = tmp->period;
	}
	return minPeriod;
}
