//=====[Libraries]=============================================================


#include "mbed.h"
#include "arm_book_lib.h"
#include "servo_motor.h"


//=====[Declaration of private defines]========================================


#define DUTY_MIN 0.025
#define DUTY_MAX 0.067
#define PERIOD 0.02


//=====[Declaration of private data types]=====================================


//=====[Declaration and initialization of public global objects]===============


PwmOut servo(PF_9);




//=====[Declaration of external public global variables]=======================


//=====[Declaration and initialization of public global variables]=============


//=====[Declaration and initialization of private global variables]============


//=====[Declarations (prototypes) of private functions]========================
void initMotor();
void mainMotor();
//=====[Implementations of public functions]===================================




//=====[Implementations of private functions]==================================




void initMotor()
{
servo.period(PERIOD); // 20ms period
servo.write(DUTY_MIN);
}


void mainMotor()
{


servo.write(DUTY_MIN);
delay(2000);
servo.write(DUTY_MAX);
delay(2000);
servo.write(DUTY_MIN);
delay(2000);
}
