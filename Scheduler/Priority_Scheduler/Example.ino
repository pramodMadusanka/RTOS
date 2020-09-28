#include <Queue.h>
Queue readyQueue;
Queue runningQueue;
Queue waitQueue;
volatile byte funcAddrL, funcAddrH;
void funcA(void* arg);
void funcB(void* arg);
void funcC(void* arg);
void initTimer();

void setup() {
    Serial.begin(9600);
    readyQueue.addTask('A', &funcA, 2);
    readyQueue.addTask('B', &funcB, 1);
    readyQueue.addTask('C', &funcC, 3);

    //Serial.println("Unsorted");
    //readyQueue.printQueue();

    readyQueue.quickSort(&(readyQueue.start));

    //Serial.println("Sorted");
    //readyQueue.printQueue();

    initTimer();
    struct strnode* node;
    /*while((node = readyQueue.dequeue())!=0){
        //strnode* node = readyQueue.dequeue();
        int funcAddr = node->taskptr;
        char funcName = node->name;
        Serial.print("Name: "); Serial.print(funcName); Serial.print("\t\tAddr: "); Serial.println(funcAddr);
    }*/

  /*Serial.println((int)readyQueue.dequeue());
  Serial.println((int)readyQueue.dequeue());
  Serial.println((int)readyQueue.dequeue());
  Serial.println((int)readyQueue.dequeue());*/
}

void loop() {

}

void funcA(void* arg){
  while(1){
    digitalWrite(9, HIGH);
    //Serial.println("funcA_HIGH");
    delay(200);
    digitalWrite(9, LOW);
    //Serial.println("funcA_LOW");
    delay(200);
  }
}

void funcB(void* arg){
  while(1){
    digitalWrite(8, HIGH);
    Serial.println("funcB_HIGH");
    delay(200);
    digitalWrite(8, LOW);
    Serial.println("funcB_LOW");
    delay(200);
  }
}

void funcC(void* arg){
  while(1){
    digitalWrite(7, HIGH);
    //Serial.println("funcC_HIGH");
    delay(200);
    digitalWrite(7, LOW);
    //Serial.println("funcC_LOW");
    delay(200);
  }
}

void initTimer(){
    cli();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;
    OCR1A = 20000; // change this value to change the interrupt frequency
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12) | (1 << CS10);  
    TIMSK1 |= (1 << OCIE1A);
    sei();
}

ISR(TIMER1_COMPA_vect, ISR_NAKED){
    wakeup();
    if(runningQueue.start->priority < readyQueue.start->priority)
        asm volatile("reti \n\r");
    
    runningQueue.start->SP = SP;
    struct strnode* tmp = readyQueue.dequeue();
    runningQueue.enqueue(tmp, 0);
    readyQueue.priorityEnqueue(runningQueue.dequeue());
    int funcAddr = (int)(tmp->taskptr);
    
    funcAddrL = funcAddr&0x00FF;
    funcAddrH = (funcAddr>>8)&0x00FF;
    
    asm volatile(
        "PUSH   r0"                     "\n\r"
        "PUSH   r1"                     "\n\r"
        "PUSH   r2"                     "\n\r"
        "PUSH   r3"                     "\n\r"
        "PUSH   r4"                     "\n\r"
        "PUSH   r5"                     "\n\r"
        "PUSH   r6"                     "\n\r"
        "PUSH   r7"                     "\n\r"
        "PUSH   r8"                     "\n\r"
        "PUSH   r9"                     "\n\r"
        "PUSH   r10"                    "\n\r"
        "PUSH   r11"                    "\n\r"
        "PUSH   r12"                    "\n\r"
        "PUSH   r13"                    "\n\r"
        "PUSH   r14"                    "\n\r"
        "PUSH   r15"                    "\n\r"
        "PUSH   r16"                    "\n\r"
        "PUSH   r17"                    "\n\r"
        "PUSH   r18"                    "\n\r"
        "PUSH   r19"                    "\n\r"
        "PUSH   r20"                    "\n\r"
        "PUSH   r21"                    "\n\r"
        "PUSH   r22"                    "\n\r"
        "PUSH   r23"                    "\n\r"
        "PUSH   r24"                    "\n\r"
        "PUSH   r25"                    "\n\r"
        "PUSH   r26"                    "\n\r"
        "PUSH   r27"                    "\n\r"
        "PUSH   r28"                    "\n\r"
        "PUSH   r29"                    "\n\r"
        "PUSH   r30"                    "\n\r"
        "PUSH   r31"                    "\n\r"
    );
    SP = tmp->SP;
    asm volatile(
        "POP   r0"                     "\n\r"
        "POP   r1"                     "\n\r"
        "POP   r2"                     "\n\r"
        "POP   r3"                     "\n\r"
        "POP   r4"                     "\n\r"
        "POP   r5"                     "\n\r"
        "POP   r6"                     "\n\r"
        "POP   r7"                     "\n\r"
        "POP   r8"                     "\n\r"
        "POP   r9"                     "\n\r"
        "POP   r10"                    "\n\r"
        "POP   r11"                    "\n\r"
        "POP   r12"                    "\n\r"
        "POP   r13"                    "\n\r"
        "POP   r14"                    "\n\r"
        "POP   r15"                    "\n\r"
        "POP   r16"                    "\n\r"
        "POP   r17"                    "\n\r"
        "POP   r18"                    "\n\r"
        "POP   r19"                    "\n\r"
        "POP   r20"                    "\n\r"
        "POP   r21"                    "\n\r"
        "POP   r22"                    "\n\r"
        "POP   r23"                    "\n\r"
        "POP   r24"                    "\n\r"
        "POP   r25"                    "\n\r"
        "POP   r26"                    "\n\r"
        "POP   r27"                    "\n\r"
        "POP   r28"                    "\n\r"
        "POP   r29"                    "\n\r"
        "POP   r30"                    "\n\r"
        "POP   r31"                    "\n\r"
        "reti"                         "\n\r"
    );
}



void wakeup(){
    for(struct strnode* node = waitQueue.getStart(); node != NULL; node = node->next){
        if(millis() >= node->wakeupTime)
            priorityEnqueue(waitQueue.dequeue());
    }
}

void priorityEnqueue(struct strnode* node){
    if(!readyQueue.getStart() || readyQueue.getStart()->priority > node->priority)
        readyQueue.addNode(readyQueue.getStart(), node);
    else{
        for(struct strnode* tmp = readyQueue.getStart(); tmp != NULL; tmp = tmp->next){
            if(tmp->next->priority > node->priority || !tmp->next)
                readyQueue.addNode(tmp, node);
        }
    }
}

void sleep(int waitms){
    struct strnode* tmp = runningQueue.dequeue();
    waitQueue.enqueue(tmp, millis()+waitms);
}


//
