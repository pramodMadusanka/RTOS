/*
 * This code was implemented for an Arduino UNO
 * No external hardware components are used
 *
 * To upload this code to arduino,
 * You should first create a folder called "Queue"
 * Then go to the link: https://github.com/pramodMadusanka/RTOS/tree/master/Scheduler/Queue
 * There you will find three files: Queue.cpp, Queue.h, keywords.txt
 * Copy those three files into the folder created above
 * Then zip the folder
 * 
 * Then, you should include the zip file (i.e., Queue.zip) to the Arduino IDE
 * Sketch -> Include Libaray -> Add .ZIP Library... -> then find the zipped file, select and press OK
 * Then you should be able to upload the code to a Arduino board
 * 
 * About the Queue library(in case you need)
 * -The queue linked list adds to the list at the end (the tail)
 * -removes from the list at the start (the head)
 * 
 */
#include "Queue.h"
#define LED 13
int a = 0;

Queue q;

volatile byte funcAddrL, funcAddrH;

void setup() {

    cli(); // disable interrupts
    
    Serial.begin(2000000);
    pinMode(LED, OUTPUT);

    // add the tasks (functions) to the queue
    // q.addTask(name, period, length, address);
    q.addTask('A', 10, 2, funcA);
    q.addTask('B', 10, 1, funcB);
    /*q.addTask('B', 10, 1, funcC);
    q.addTask('B', 10, 1, funcD);
    q.addTask('B', 10, 1, funcE);
    q.addTask('A', 10, 2, funcA);
    q.addTask('B', 10, 1, funcB);
    q.addTask('B', 10, 1, funcC);
    q.addTask('B', 10, 1, funcD);
    q.addTask('B', 10, 1, funcE);
    q.addTask('A', 10, 2, funcA);
    q.addTask('B', 10, 1, funcB);
    q.addTask('B', 10, 1, funcC);
    q.addTask('B', 10, 1, funcD);
    q.addTask('B', 10, 1, funcE);*/

    // setting up the timer1 for periodic interrupts
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;
    OCR1A = 6553; // change this value to change the interrupt frequency
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12) | (1 << CS10);  
    TIMSK1 |= (1 << OCIE1A);
    
    sei(); // enable interrupts
}

void loop() {
    static int k=0;
    /*for(int i = 0; i<2; i++){
        int a = (int)(*(q.dequeue())).taskptr;
        Serial.print("taskptr: ");Serial.println(a);
    }*/
    
    k++;
    Serial.print(k);
    delay(20);

}

void funcA(void* arg){
    Serial.println();
    Serial.print('A');
    //byte p = '5';
    //p++;
    /*for(char i=67; i<70; i++){
      //Serial.print("i = ");
      Serial.print(i);
    }*/
    Serial.print('a');
    Serial.print('b');
    Serial.print('c');
    Serial.println("Function A");
    //Serial.print(p);
    Serial.print(' ');
}

void funcB(void* arg){
    Serial.println("Function B");
    /*for(int i=0; i<3; i++){
        digitalWrite(13, HIGH);
        delay(200);
        digitalWrite(13, LOW);
        delay(200);
    }*/
    Serial.println("ENDB");
}

void funcC(void* arg){
    Serial.println("C");
    for(int i=0; i<3; i++){
        digitalWrite(LED, HIGH);
        delay(50);
        digitalWrite(LED, LOW);
        delay(50);
    }
    Serial.println("ENDC");
}

void funcD(void* arg){
    Serial.println("D");
    for(int i=0; i<3; i++){
        digitalWrite(LED, HIGH);
        delay(10);
        digitalWrite(LED, LOW);
        delay(10);
    }
    Serial.println("ENDD");
}

void funcE(void* arg){
    Serial.println("E");
    for(int i=0; i<3; i++){
        digitalWrite(LED, HIGH);
        delay(100);
        digitalWrite(LED, LOW);
        delay(100);
    }
    Serial.println("ENDE");
}

ISR(TIMER1_COMPA_vect, ISR_NAKED){
  
    static byte flag = 0;
    
    //int funcAddr = (int)((*(q.dequeue())).taskptr);
    int funcAddr;
    
    if(flag==0){
        funcAddr = funcA;
        flag=1;
    }else{
        funcAddr = funcB;
        flag=0;
    }
    
    funcAddrL = funcAddr&0x00FF;
    funcAddrH = (funcAddr>>8)&0x00FF;  
    asm volatile(
        "POP    r0"                   "\n\r"
        "POP    r1"                   "\n\r"
        
        "LDS    r30, (funcAddrL)"     "\n\r"
        "LDS    r31, (funcAddrH)"     "\n\r"
    
        "PUSH   r30"                  "\n\r"
        "PUSH   r31"                  "\n\r"
    
        "reti"                        "\n\r"
    );
}
