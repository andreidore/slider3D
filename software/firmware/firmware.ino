#include "FiniteStateMachine.h"
#include "CmdMessenger.h"


CmdMessenger cmdMessenger = CmdMessenger(Serial);

enum
{
  kStatus ,
  kStartPlan,
  kEndPlan,
  kAddStepper,
  kAddSleep,


};


typedef struct {
  int type; //0 - sleep, 1 - stepper,
  long step1;
  long step2;
  long step3;
  long sleep;
} Entry;

Entry entries[30];
uint8_t entriesCount;
uint8_t entriesIndex;


long startSleepTime;
long startPumpTime;



void homeStart();
void homeUpdate();
void idleStart();
void idleUpdate();
void planUpdate();
void selectUpdate();
void stepperStart();
void stepperUpdate();
void sleepStart();
void sleepUpdate();



State homeState = State(homeStart, homeUpdate, NULL);
State idleState = State(idleStart, idleUpdate, NULL);
State planState = State(planUpdate);
State selectState = State(selectUpdate);
State stepperState = State(stepperStart, stepperUpdate, NULL);
State sleepState = State(sleepStart, sleepUpdate, NULL);



FSM stateMachine = FSM(homeState);

char logBuffer[100];


void setup()
{


  Serial.begin(57600);
  log("Start init.");

  delay(500);


  // Adds newline to every command
  cmdMessenger.printLfCr();

  // Attach callback methods
  cmdMessenger.attach(unknownCommandCallback);
  cmdMessenger.attach(kStartPlan, startPlanCallback);
  cmdMessenger.attach(kEndPlan, endPlanCallback);
  cmdMessenger.attach(kAddStepper, addStepperCallback);
  cmdMessenger.attach(kAddSleep, addSleepCallback);



  //pinMode(ENDSTOP, INPUT_PULLUP);



  log("End init.");


}

long test;

void loop()
{




  cmdMessenger.feedinSerialData();

  stateMachine.update();



  //delay(100);

  //log("d");



}



void log(char* message)
{
  cmdMessenger.sendCmd(kStatus, message);
  //message[0] = '\0';
}

// callback

void unknownCommandCallback()
{
  cmdMessenger.sendCmd(kStatus, "Command without attached callback");
}


void startPlanCallback()
{
  log("Start plan.");
  entriesCount = 0;
  entriesIndex = 0;

}

void endPlanCallback()
{
  log("End plan.");
  entriesIndex = 0;
  stateMachine.transitionTo(planState);

}

void addStepperCallback()
{

  char* step1String = cmdMessenger.readStringArg();
  long step1 = atoi(step1String);
  char* step2String = cmdMessenger.readStringArg();
  long step2 = atoi(step2String);
  char* step3String = cmdMessenger.readStringArg();
  long step3 = atoi(step3String);


  Entry entry = {1, step1, step2, step3, 0};

  entries[entriesIndex] = entry;
  entriesCount++;
  entriesIndex++;



  log("Add entry.");

  sprintf (logBuffer, "Add stepper Step 1:%ld Step 2:%ld Step 3:%ld  Count:%d",  entry.step1, entry.step2, entry.step3, entriesCount);
  log(logBuffer);
  //log("Type:" + String(entry.type));
  //log("Index:" + String(entry.index));
  //log("Value:" + String(entry.value));

  //log("Entries count:" + String(entriesCount));


}


void addSleepCallback()
{

  char* sleepString = cmdMessenger.readStringArg();
  long sleep = atoi(sleepString);



  Entry entry = {0, 0, 0, 0, sleep};

  entries[entriesIndex] = entry;
  entriesCount++;
  entriesIndex++;



  log("Add entry.");

  sprintf (logBuffer, "Add sleep  3:%ld  Count:%d",  entry.sleep, entriesCount);
  log(logBuffer);
  //log("Type:" + String(entry.type));
  //log("Index:" + String(entry.index));
  //log("Value:" + String(entry.value));

  //log("Entries count:" + String(entriesCount));


}



//State


void homeStart() {
  log("Go home.");



}

void homeUpdate() {

  /*
    if (digitalRead(ENDSTOP) == 1) {
    return;
    }
  */

  log("Find home.");

  stateMachine.transitionTo(idleState);

}


void idleStart() {
  log("Idle.");
}

void idleUpdate() {
  delay(100);
}


void planUpdate() {
  if (entriesCount == 0) {
    //Serial.println("Empty plan.");
    stateMachine.transitionTo(idleState);
    return;
  }

  log("Start plan.");
  /*
    for (int i = 0; i < entriesCount; i++) {

    Entry entry = entries[i];


    log("Type:" + String(entry.type));
    log("Index:" + String(entry.index));
    log("Value:" + String(entry.value));


    }
  */

  stateMachine.transitionTo(selectState);
}


void selectUpdate() {

  log("select next state.");

  if (entriesIndex == entriesCount) {
    log("End plan");
    stateMachine.transitionTo(idleState);
    return;
  }

  Entry entry = entries[entriesIndex];

  //sprintf (logBuffer, "Next type :%d", entry.type);
  //log(logBuffer);
  //log("type :" + String(entry.type));

  if ( entry.type == 0) {
    stateMachine.transitionTo(sleepState);
  } else if ( entry.type == 1) {
    stateMachine.transitionTo(stepperState);
  }

  entriesIndex++;


}




void stepperStart() {

  log("Start stepper.");

  Entry entry = entries[entriesIndex];

  //sprintf (logBuffer, "Stepper index:%d value:%ld", entry.index,entry.value);
  //log(logBuffer);

  //log("Index:" + String(entry.index));
  //log("Value:" + String(entry.value));

  //tic[entry.index].setTargetPosition(entry.value);



}

void stepperUpdate() {
  Entry entry = entries[entriesIndex];
  //if (entry.value == tic[entry.index].getCurrentPosition()) {
  //sprintf (logBuffer, "Mem : %d", freeMemory());
  //log(logBuffer);
  stateMachine.transitionTo(selectState);
  // }


}


void sleepStart() {

  log("Start sleep.");

  Entry entry = entries[entriesIndex];

  //sprintf (logBuffer, "Sleep value %d", entry.value);
  //log(logBuffer);


  startSleepTime = millis();

}

void sleepUpdate() {
  Entry entry = entries[entriesIndex];

  //sprintf (logBuffer, "-----:%d---%ld", millis() - startSleepTime, entry.value);
  //log(logBuffer);


  if (millis() - startSleepTime > entry.sleep) {
    //sprintf (logBuffer, "Mem : %d", freeMemory());
    //log(logBuffer);
    stateMachine.transitionTo(selectState);
  }


}
