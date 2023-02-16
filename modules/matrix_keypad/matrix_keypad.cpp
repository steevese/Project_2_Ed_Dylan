//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"


#include "matrix_keypad.h"


#include "date_and_time.h"
#include "display.h"


#include "servo_motor.h"
#include "user_interface.h"


//=====[Declaration of private defines]========================================


#define MATRIX_KEYPAD_NUMBER_OF_ROWS    4
#define MATRIX_KEYPAD_NUMBER_OF_COLS    4
#define DEBOUNCE_KEY_TIME_MS        40


//=====[Declaration of private data types]=====================================


typedef enum {
   MATRIX_KEYPAD_SCANNING,
   MATRIX_KEYPAD_DEBOUNCE,
   MATRIX_KEYPAD_KEY_HOLD_PRESSED
} matrixKeypadState_t;


//=====[Declaration and initialization of public global objects]===============


DigitalOut keypadRowPins[MATRIX_KEYPAD_NUMBER_OF_ROWS] = {PB_3, PB_5, PC_7, PA_15};
DigitalIn keypadColPins[MATRIX_KEYPAD_NUMBER_OF_COLS]  = {PB_12, PB_13, PB_15, PC_6};


//=====[Declaration of external public global variables]=======================


//=====[Declaration and initialization of public global variables]=============


bool correctCodeState = false; // Public variable to be used in Motor file
char correctCode[4]   = {'1', '2', '3', '4'}; // Correct Code

//=====[Declaration and initialization of private global variables]============


static matrixKeypadState_t matrixKeypadState;
static int timeIncrement_ms = 0;
static char enteredCode[4];
static int countCodeKeys = 0;
static int attemptedPassword = 0;


//=====[Declarations (prototypes) of private functions]========================


static char matrixKeypadScan();
static void matrixKeypadReset();
static void displayEnteredCode(char keypadChar);// Writes the code to the LCD
static void resetCode(); //Empties the enteredCode array
static void messageDisplay();
bool isCorrectCodeState();




//=====[Implementations of public functions]===================================




void matrixKeypadInit( int updateTime_ms )
{
   timeIncrement_ms = updateTime_ms;
   matrixKeypadState = MATRIX_KEYPAD_SCANNING;
   int pinIndex = 0;
   for( pinIndex=0; pinIndex<MATRIX_KEYPAD_NUMBER_OF_COLS; pinIndex++ ) {
       (keypadColPins[pinIndex]).mode(PullUp);
   }
}


char matrixKeypadUpdate()
{
   static int accumulatedDebounceMatrixKeypadTime = 0;
   static char matrixKeypadLastKeyPressed = '\0';
   char keyDetected = '\0';
   char keyReleased = '\0';
   switch( matrixKeypadState ) {
   case MATRIX_KEYPAD_SCANNING:
       keyDetected = matrixKeypadScan();
       if( keyDetected != '\0' ) {
            displayEnteredCode(keyDetected);// Writes the entered matrix keypad character to the LCD
            delay(500);
            matrixKeypadLastKeyPressed = keyDetected;
            accumulatedDebounceMatrixKeypadTime = 0;
            matrixKeypadState = MATRIX_KEYPAD_DEBOUNCE;
       }
       break;
   case MATRIX_KEYPAD_DEBOUNCE:
       if( accumulatedDebounceMatrixKeypadTime >=
           DEBOUNCE_KEY_TIME_MS ) {
           keyDetected = matrixKeypadScan();
           if( keyDetected == matrixKeypadLastKeyPressed ) {
               matrixKeypadState = MATRIX_KEYPAD_KEY_HOLD_PRESSED;
           } else {
               matrixKeypadState = MATRIX_KEYPAD_SCANNING;
               // writeOnLCDCharToString(keyDetected); ///////////////
           }
       }
       accumulatedDebounceMatrixKeypadTime =
           accumulatedDebounceMatrixKeypadTime + timeIncrement_ms;
       break;
   case MATRIX_KEYPAD_KEY_HOLD_PRESSED:
       keyDetected = matrixKeypadScan();
       if( keyDetected != matrixKeypadLastKeyPressed ) {
           if( keyDetected == '\0' ) {
               keyReleased = matrixKeypadLastKeyPressed;
           }
           matrixKeypadState = MATRIX_KEYPAD_SCANNING;
       }
       break;
   default:
       matrixKeypadReset();
       break;
   }
   return keyReleased;
}
//=====[Implementations of private functions]==================================
static char matrixKeypadScan()
{
   int row = 0;
   int col = 0;
   int i = 0;
   char matrixKeypadIndexToCharArray[] = {
       '1', '2', '3', 'A',
       '4', '5', '6', 'B',
       '7', '8', '9', 'C',
       '*', '0', '#', 'D',
   };
  
   for( row=0; row<MATRIX_KEYPAD_NUMBER_OF_ROWS; row++ ) {
       for( i=0; i<MATRIX_KEYPAD_NUMBER_OF_ROWS; i++ ) {
           keypadRowPins[i] = ON;
       }
       keypadRowPins[row] = OFF;
       for( col=0; col<MATRIX_KEYPAD_NUMBER_OF_COLS; col++ ) {
           if( keypadColPins[col] == OFF ) {
          
               return matrixKeypadIndexToCharArray[
                   row*MATRIX_KEYPAD_NUMBER_OF_ROWS + col];
           }
       }
   }
   return '\0';
}
static void displayEnteredCode(char keypadChar){ // Displays entered code and checks if code is correct or incorrect
   char codeString[10];
   bool key1 = false; // these 4 keys represent the 4 presses of the key pad
   bool key2 = false;
   bool key3 = false;
   bool key4 = false;
    if ( attemptedPassword < 4 ) {// Cuts code off after a certain number of tries (says 4 but we wrote the code to only allow 3)
        if ( countCodeKeys < 4 ){
        enteredCode[countCodeKeys] = keypadChar;
        displayCharPositionWrite ( 0,0 );
        displayStringWrite("Code:           ");
      
       }
       for (int i = 0; i < 4; i++){
       char keyPadChar = enteredCode[i];
       sprintf(codeString, "%c", keyPadChar);
       displayCharPositionWrite ( i+5,0); // Writes the character at given position
       displayStringWrite(codeString);
       }
  
       countCodeKeys++;
        if (countCodeKeys == 4){
            attemptedPassword++; // ads an attemt after every time 4 keys are pressed
      
            for (int i = 0; i < 4; i++){
   //check to see if each entered character equals the corresponding Correct Code Character
                if (enteredCode[i] == correctCode[i] ){
                    if (i == 0){
                        key1 = true;
                    }
                    if (i == 1){
                        key2 = true;
                    }
                    if (i == 2){
                        key3 = true;
                    }
                    if (i == 3){
                        key4 = true;
                    }
                } 
            }
            if(key1&&key2&&key3&&key4){ // if all 4 keys match the code is correct
                correctCodeState = true;
                for (int i =0; i < 4; i++){
                    enteredCode[i] = 0;
                }
                displayCharPositionWrite ( 0,0 );
                displayStringWrite("Correct!        ");
                displayCharPositionWrite ( 0,1 );
                displayStringWrite("Gate is opening  ");
                attemptedPassword = 0;
                mainMotor();
            }
                 
            else { // if any of the keys do not match it is false and resets all the keys
                correctCodeState = false;
                bool key1 = false;
                bool key2 = false;
                bool key3 = false;
                bool key4 = false;

                
            }
            if (correctCodeState == false){
                for (int i =0; i < 4; i++){
                    enteredCode[i] = 0;
                    }
                if (attemptedPassword == 1){ // first attempt
                    displayCharPositionWrite ( 0,0 );
                    displayStringWrite("Incorrect!     ");
                    displayCharPositionWrite ( 0,1 );
                    displayStringWrite("Two more Tries  ");
                    countCodeKeys = 0;
                    delay(2000);
                    displayCharPositionWrite ( 0,0 );
                    displayStringWrite("Code:           ");
                      
                }
                if (attemptedPassword == 2){ // second attempt
                    displayCharPositionWrite ( 0,0 );
                    displayStringWrite("Incorrect!     ");
                    displayCharPositionWrite ( 0,1 );
                    displayStringWrite("One more Try   ");
                    countCodeKeys = 0;
                    delay(2000);
                    displayCharPositionWrite ( 0,0 );
                    displayStringWrite("Code:           ");
                }
                if (attemptedPassword == 3){ // third and final attempt
                    attemptedPassword++; // this is what makes the final attempt over the attempt limit and lock out the code
                    displayCharPositionWrite ( 0,0 );
                    displayStringWrite("Incorrect!     ");
                    displayCharPositionWrite ( 0,1 );
                    displayStringWrite("System Disabled   "); 
                }
            }
        }
                
    }
}

 static void matrixKeypadReset()
{
   matrixKeypadState = MATRIX_KEYPAD_SCANNING;
}
