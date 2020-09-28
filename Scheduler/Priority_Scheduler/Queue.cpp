#include <stdio.h>
#include "Arduino.h"
#include "Queue.h"

Queue::Queue(void){
	start = NULL;
	end   = NULL;
}

struct strnode* Queue::createNode(char name, void (*taskptr)(void* arg), uint8_t priority, int wakeupTime){
    tmp=(struct strnode*)malloc(sizeof(struct strnode));
    tmp->name=name;
    tmp->taskptr=taskptr;
    tmp->priority=priority;
	tmp->wakeupTime=wakeupTime;
    tmp->next=NULL;

    return tmp;
}

void Queue::addTask(char name, void (*taskptr)(void* arg), uint8_t priority){
    if(start==NULL){
        start=createNode(name, taskptr, priority, 0);
        end=start;
    } else {
        end->next=createNode(name, taskptr, priority, 0);
        end=end->next;
    }
}

void Queue::enqueue(struct strnode* node, int wakeupTime){
	node->wakeupTime = wakeupTime;
    if(start==NULL){
        start=node;
        end=start;
    } else {
        end->next=node;
        end=end->next;
    }
}

struct strnode* Queue::dequeue(){
    tmp=start;
    if(start==end){
        start=NULL;
        end=NULL;
    } else{
        start=start->next;
    }
    return tmp;
}

void Queue::printQueue(){
	for(struct strnode* tmp = start; tmp != NULL; tmp = tmp->next){
		Serial.println(tmp->name);
	}
}

struct strnode* Queue::getStart(){
	return start;
}

void Queue::addPCB(struct strnode* head, struct strnode* PCB){
	if(head == start){
		PCB->next = head;
		start = PCB;
	} else if(head == end){
		head->next = PCB;
		end = PCB;
	} else {
		PCB->next = head->next;
		head->next = PCB;
	}
}

struct strnode* Queue::getTail(struct strnode* cur){ 
    while (cur != NULL && cur->next != NULL) 
        cur = cur->next; 
    return cur; 
} 

struct strnode* Queue::partition(struct strnode *head, struct strnode *end, struct strnode **newHead, struct strnode **newEnd){ 
    struct strnode *pivot = end; 
    struct strnode* prev = NULL;
    struct strnode* cur = head;
    struct strnode* tail = pivot;
  
    while (cur != pivot){ 
        if (cur->priority < pivot->priority){ 
            if ((*newHead) == NULL) 
                (*newHead) = cur; 
  
            prev = cur;  
            cur = cur->next; 
        } else {
            if (prev) 
                prev->next = cur->next;
                
            struct strnode *tmp = cur->next; 
            cur->next = NULL; 
            tail->next = cur; 
            tail = cur; 
            cur = tmp; 
        } 
    } 
    if ((*newHead) == NULL) 
        (*newHead) = pivot; 
  
    (*newEnd) = tail; 
    return pivot; 
} 

struct strnode* Queue::quickSortRecur(struct strnode *head, struct strnode *end){ 
    if (!head || head == end) 
        return head; 
  
    struct strnode *newHead = NULL, *newEnd = NULL; 
    
    struct strnode *pivot = partition(head, end, &newHead, &newEnd); 
    if (newHead != pivot){ 
        struct strnode *tmp = newHead; 
        while (tmp->next != pivot) 
            tmp = tmp->next; 
        tmp->next = NULL; 
        newHead = quickSortRecur(newHead, tmp); 
        tmp = getTail(newHead); 
        tmp->next = pivot; 
    } 
    pivot->next = quickSortRecur(pivot->next, newEnd);
    return newHead; 
}

void Queue::quickSort(struct strnode** headRef){ 
    *headRef = quickSortRecur(*headRef, getTail(*headRef)); 
    return; 
}

//===========================================================================================
/*void delayTask(int waitms){
    tmp = dequeue();
    waitQueue.enqueue(tmp, millis()+waitms);
    WAITFLAG = 1;
}

void wakeup(){
    for(struct strnode* node = waitQueue.getStart(); node != NULL; node = node->next){
        if(millis() >= node->wakeupTime)
            priorityEnqueue(waitQueue.dequeue());
    }

    //if()
}*/
