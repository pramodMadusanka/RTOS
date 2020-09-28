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
    Serial.print("< ");
    if(runningQueue.start != 0){
        //Serial.print((int)runningQueue.getStart());
        Serial.println("\t SAME >");
        asm volatile("reti \n\r");
    }
    struct strnode* tmp = readyQueue.dequeue();
    runningQueue.enqueue(tmp, 0);
    int funcAddr = (int)(tmp->taskptr);
            
    funcAddrL = funcAddr&0x00FF;
    funcAddrH = (funcAddr>>8)&0x00FF;
    
    asm volatile(        
        "LDS    r30, (funcAddrL)"     "\n\r"
        "LDS    r31, (funcAddrH)"     "\n\r"
    
        "PUSH   r30"                  "\n\r"
        "PUSH   r31"                  "\n\r"

        //"reti"                        "\n\r"
    );
    Serial.println("\t FUNC >");
    asm volatile("reti \n\r");
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
    waitQueue.enqueue(tmp millis()+waitms);
}
