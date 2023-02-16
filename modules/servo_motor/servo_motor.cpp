//=====[Libraries]=============================================================


#include "mbed.h"
#include "arm_book_lib.h"
#include "servo_motor.h"


//=====[Declaration of private defines]========================================


#define DUTY_MIN 0.025 // low limit that sets the motor to position 0
#define DUTY_MAX 0.067 // voltage value that rotates the motor 90 degrees from the original position
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




void initMotor() // initializes the motor
{
servo.period(PERIOD); // 20ms period
servo.write(DUTY_MIN);
}


void mainMotor() // function that moves the gate
{


servo.write(DUTY_MIN); // start at position 0
delay(2000); // wait 2 seconds
servo.write(DUTY_MAX); // rotate 90 degrees
delay(2000); // wait 2 seconds
servo.write(DUTY_MIN); // go back to position 0
delay(2000); // wait 2 seconds
}
