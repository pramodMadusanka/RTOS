/*
 * This code was implemented for an Arduino UNO
 * It uses the built-in LED (pin 13) in funcA() and funcB()
 * No external hardware components are used
 *
 * To upload this code to arduino,
 * You should first download Queue folder and zip it
 * Then, you should include the zip files to the Arduino IDE
 * Sketch -> Include Libaray -> Add .ZIP Library... -> then find the zipped file, select and press OK
 * Then you should be able to upload the code to a Arduino board
 * 
 * The queue linked list adds to the list at the end (the tail)
 * removes from the list at the start (the head)
 * and it works perfectly
 * in section 1 it uses the dequeue() function and prints the addresses of two functions: funcA() and funcB()
 * 
 * when the address of the funcA manually assigned to funcAddr of ISR (by looking at the output of the section 1)(the Line 2),
 * the code compiles without errors
 * when the section 1 and line 2 are commented out and line 1 is uncomented, it gives an error while compiling
 * 
 * even when the funcAddr is hardcoded with the address of the funcA, it reraly executs the whole function
 * it only executes the Serial.println("A") most of the time
 * most of the times, it does not execute the while loop which is responsible for blinking of the LED
 * 
 */

#include "Queue.h"
#define LED 13
int a = 0;

void (*func)(void* arg);

Queue q;

void setup() {
    cli();
    Serial.begin(2000000);
    pinMode(LED, OUTPUT);

    // add the tasks (functions) to the queue
    // q.addTask(name, period, length, address);
    q.addTask('A', 10, 2, &funcA);
    q.addTask('B', 10, 1, &funcB);

    // setting up the timer1 for periodic interrupts
    TCCR1A = 0;// set entire TCCR1A register to 0
    TCCR1B = 0;// same for TCCR1B
    TCNT1  = 0;//initialize counter value to 0
    // set compare match register for 1hz increments
    OCR1A = 65535;// = (16*10^6) / (1*1024) - 1 (must be <65536)
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS12 and CS10 bits for 1024 prescaler
    TCCR1B |= (1 << CS12) | (1 << CS10);  
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
    
    sei();
}

void loop() {
    
    for(int i = 0; i<2; i++){                       //<--section 1 (the entire for loop)
        int a = (int)(*(q.dequeue())).taskptr;
        Serial.print("taskptr: ");Serial.println(a);
    }
    
    Serial.println("code will print this untill first interrupt occures");
    delay(500);

}

void funcA(void* arg){
    Serial.println("A");
    while(1){
        digitalWrite(LED, HIGH);
        delay(100);
        digitalWrite(LED, LOW);
        delay(100);
    }
}

void funcB(void* arg){
    Serial.println("B");
    while(1){
        digitalWrite(13, HIGH);
        delay(200);
        digitalWrite(13, LOW);
        delay(200);
    }
}

ISR(TIMER1_COMPA_vect, ISR_NAKED){
    //int funcAddr = (int)(*(q.dequeue())).taskptr;    //<--Line 1
    int funcAddr = 590;                                //<--Line 2
    int funcAddrL = funcAddr&0x00FF;
    int funcAddrH = (funcAddr>>8)&0x00FF;  
    asm volatile(
        "POP    r0"                   "\n\r"
        "POP    r1"                   "\n\r"
        
        "LDI    r30, %[PCL]"          "\n\r"
        "LDI    r31, %[PCH]"          "\n\r"
    
        "PUSH   r30"                  "\n\r"
        "push   r31"                  "\n\r"
    
        "reti"                        "\n\r"
        
        ::[PCL] "" (funcAddrL),
          [PCH] "" (funcAddrH):
    );
}
