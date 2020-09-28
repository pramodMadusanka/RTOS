#include <stdio.h>
#include "Arduino.h"
#include "Queue.h"

Queue::Queue(void){
	start = NULL;
	end   = NULL;
}

struct strnode* Queue::createPCB(void (*taskptr)(void* arg), uint8_t priority, int wakeupTime){
    tmp=(struct strnode*)malloc(sizeof(struct strnode));
    tmp->taskptr=taskptr;
    tmp->priority=priority;
    tmp->wakeupTime=wakeupTime;
    tmp->SP = nextSP;
    tmp->next=NULL;

    nextSP -= 128; 
    SP = tmp->SP;
    taskptrL = taskptr&0x00FF;
    taskptrH = (taskptr>>8)&0x00FF;
    
    asm volatile(        
        "LDS    r30, (taskptrL)"     "\n\r"
        "LDS    r31, (taskptrH)"     "\n\r"
    
        "PUSH   r30"                  "\n\r"
        "PUSH   r31"                  "\n\r"

        "reti"                        "\n\r"
    );

    return tmp;
}

void Queue::addTask(void (*taskptr)(void* arg), uint8_t priority){
    if(start==NULL){
        start=createNode(name, taskptr, priority, 0);
        end=start;
    } else {
        end->next=createNode(name, taskptr, priority, 0);
        end=end->next;
    }
}

void Queue::enqueue(struct PCB* node, int wakeupTime){
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

struct PCB* Queue::partition(struct PCB *head, struct PCB *end, struct PCB **newHead, struct PCB **newEnd){ 
    struct PCB *pivot = end; 
    struct PCB* prev = NULL;
    struct PCB* cur = head;
    struct PCB* tail = pivot;
  
    while (cur != pivot){ 
        if (cur->priority < pivot->priority){ 
            if ((*newHead) == NULL) 
                (*newHead) = cur; 
  
            prev = cur;  
            cur = cur->next; 
        } else {
            if (prev) 
                prev->next = cur->next;
                
            struct PCB *tmp = cur->next; 
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

struct PCB* Queue::quickSortRecur(struct PCB *head, struct PCB *end){ 
    if (!head || head == end) 
        return head; 
  
    struct PCB *newHead = NULL, *newEnd = NULL; 
    
    struct PCB *pivot = partition(head, end, &newHead, &newEnd); 
    if (newHead != pivot){ 
        struct PCB *tmp = newHead; 
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

void Queue::sortQueue(struct PCB** headRef){ 
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
