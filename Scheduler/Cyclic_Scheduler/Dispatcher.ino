/*
 * To upload this code to arduino
 * you should first download Queue folder and Cyclic_Scheduler folder and zip them
 * then include the zip files to the Arduino IDE
 * Sketch -> Include Libaray -> Add .ZIP Library... -> then find the zipped files, select them and press OK
 * Then you should be able to upload the code to a Arduino board (I'm using an Arduino UNO board)
 * 
 * NOTE: This code cannot do the context switching or task switching
 * Thus, the finctions 'fincA()' and 'funcB()' will not be executed
 * they are used only to pass function pointers to 'addTask()' function
 */
 
#include <Scheduler.h>
#define LED 13

Scheduler s;
node** schedule;
static int i=0;
volatile byte funcAddrL, funcAddrH;
void initTimer();
int freeRam();
void funcA(void* arg);
void funcB(void* arg);
void funcC(void* arg);

void setup() {
    Serial.begin(2000000);
    pinMode(9, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(7, OUTPUT);

    //Adding tasks
    //Inside of the Scheduler, there is an object called queue type of Queue
    //The 'addTask()' function from that object should be used to add tasks.
    //I need supervisors help to change this (i.e., to remove the 'queue.' part)
    s.queue.addTask('A', 10, 4, &funcA, 1);
    s.queue.addTask('B', 20, 6, &funcB, 2);
    s.queue.addTask('C', 60, 5, &funcC, 3);

    //calculating the schedule and saving the address of the first node of the schedule to the variable 'schedule'
    //calcSchedule(major_cycle, size_of_the_minor_cycle_(in secounds), minimam_period);
    schedule = s.calcSchedule(s.queue.getMajorCycle(), 1, s.queue.getMinPeriod());

    //printing the name of the task that should be running at each scheduling point
    for(int i=0; i<60; i++){
        Serial.print("["); Serial.print(i); Serial.print("]\t"); Serial.println((*(schedule+i))->name);
    }

    initTimer();
}

void loop() {
    Serial.println("< \t LOOP >");
    delay(200);
    Serial.println("< \t FUCK >");
    delay(200);
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
    //Serial.println("funcB_HIGH");
    delay(200);
    digitalWrite(8, LOW);
    //Serial.println("funcB_LOW");
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
    OCR1A = 10000; // change this value to change the interrupt frequency
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12) | (1 << CS10);  
    TIMSK1 |= (1 << OCIE1A);
    sei();
}

ISR(TIMER1_COMPA_vect, ISR_NAKED){
    //Serial.print("< ");
    if(i>59){
      i=0;
    }
    int funcAddr = (int)((*(schedule+i))->taskptr);
    i++;
    
    if((*(schedule+i-1))==NULL){
        Serial.print(i-1);
        //Serial.println("\t NULL >");
        asm volatile("reti \n\r");
    }
    if((*(schedule+i-1)) == (*(schedule+i-2))){
        Serial.print(i-1);
        //Serial.println("\t SAME >");
        asm volatile("reti \n\r");
    }
        
    funcAddrL = funcAddr&0x00FF;
    funcAddrH = (funcAddr>>8)&0x00FF;
    
    asm volatile(        
        "LDS    r30, (funcAddrL)"     "\n\r"
        "LDS    r31, (funcAddrH)"     "\n\r"
    
        "PUSH   r30"                  "\n\r"
        "PUSH   r31"                  "\n\r"

        //"reti"                        "\n\r"
    );
    Serial.print(i-1);
    //Serial.println("\t FUNC >");
    asm volatile("reti \n\r");
}

/*void taskSleep(int sleepTime){
    //now = millis();
    WAITING_FLAG = 1;
    addNodeToWaitingQueue(dequeuerunningQueue);
}

ISR(){
    if(WAITING_FLAG){
        for all in waiting queue{
            if(millis()>=wakeupTime){
                enqueueToReadyQueue(dequeueFromWaitingQueue());
                scheduleReadyQueue();
            }
        }
    }
}*/
