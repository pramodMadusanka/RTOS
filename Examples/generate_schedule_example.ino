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
void funcA(void* arg);
void funcB(void* arg);

void setup() {
    Serial.begin(9600);
    pinMode(LED, OUTPUT);

    //Adding tasks
    //Inside of the Scheduler, there is an object called queue type of Queue
    //The 'addTask()' function from that object should be used to add tasks.
    //I need supervisors help to change this (i.e., to remove the 'queue.' part)
    s.queue.addTask('A', 10, 4, &funcA);
    s.queue.addTask('B', 20, 6, &funcB);
    s.queue.addTask('C', 60, 5, &funcB);

    //calculating the schedule and saving the address of the first node of the schedule to the variable 'schedule'
    //calcSchedule(major_cycle, size_of_the_minor_cycle_(in secounds), minimam_period);
    node** schedule = s.calcSchedule(s.queue.getMajorCycle(), 0.5, s.queue.getMinPeriod());

    //printing the name of the task that should be running at each scheduling point
    for(int i=0; i<120; i++){
        Serial.print("["); Serial.print(i); Serial.print("]\t"); Serial.println((*(schedule+i))->name);
    }
}

void loop() {
    
}

void funcA(void* arg){
    digitalWrite(LED, HIGH);
    delay(2000);
    digitalWrite(LED, LOW);
    delay(2000);
}

void funcB(void* arg){
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
}
