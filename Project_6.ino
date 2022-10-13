//Copyright 2021 Pheonix Ecycle LLC. All Rights Reserved
//Project_6.ino
//Powers a fan with an IR remote
//Remote Control Fan
//Original Author: Caleb Stupin
//Revision C - 05/08/21

#include <IRremote.h>

#define irPin 11                        //Define all the pins

#define motorPin 6

#define SEG_A 5
#define SEG_B 7
#define SEG_C 10
#define SEG_D 9
#define SEG_E 8
#define SEG_F 4
#define SEG_G 3

#define BTN_PWR     69                  //Define all the IR Remote codes
#define BTN_UP      9
#define BTN_DOWN    7
#define BTN_0       22
#define BTN_1       12
#define BTN_2       24
#define BTN_3       94

#define FAN_OFF 0                       //These are all the states
#define FAN_LOW 1
#define FAN_MED 2
#define FAN_HIGH 3


int state = 0;
   

   
//Function: setup()
//Description: sets up all the pins and the IR receiver
//Parameters: N/A
//Returns: N/A
//Globals: N/A
void setup() 
{
  pinMode(irPin, INPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(SEG_A, OUTPUT);
  pinMode(SEG_B, OUTPUT);
  pinMode(SEG_C, OUTPUT);
  pinMode(SEG_D, OUTPUT);
  pinMode(SEG_E, OUTPUT);
  pinMode(SEG_F, OUTPUT);
  pinMode(SEG_G, OUTPUT);

  IrReceiver.begin(irPin, ENABLE_LED_FEEDBACK);

  Serial.begin(9600);

  
}

//Function: loop()
//Description: The main loop where the IR remote code comes in
//Parameters: N/A
//Returns: N/A
//Globals: N/A
void loop() 
{
  int keyCode =0;
  int receivedCode =0;

  while (0 == receivedCode)
  {
    if (IrReceiver.decode() )
    {
        keyCode = (int) IrReceiver.decodedIRData.command;

        Serial.print( "IR Code received = ");
        Serial.println(keyCode);

        IrReceiver.resume();

        receivedCode = 1;

        state = getState(keyCode);          //Only go to the state machine functions if the IR receiver gets a new code
        executeState(state);
    }
    
  }

  
}

/*int inputCode()
{
  int keyCode =0;
  int receivedCode =0;

  while (0 == receivedCode)
  {
    if (IrReceiver.decode() )
    {
        keyCode = (int) IrReceiver.decodedIRData.command;

        Serial.print( "IR Code received = ");
        Serial.println(keyCode);

        IrReceiver.resume();

        receivedCode++;

        state = getState(keyCode);
        executeState(state);
    }
    
  }

  
}*/


//Function: getState
//Description: Inputs the code from the IR remote and returns the state
//Parameters: int code
//Returns: int state
//Globals: fanState, lastOnState
int getState(int code)
{
  static int fanState = FAN_OFF;                    //the initial conditions are FAN_OFF.  Since it is static, the value will not reset every iteration of the loop
  static int lastOnState = FAN_LOW;
  Serial.println(lastOnState);                      //This is to check if the state is changing

  switch (fanState)                                 //One switch state considers the fan state, the second switch statement considers the code
  {
    case FAN_OFF:
      switch (code)
      {
        case BTN_PWR:
          fanState = lastOnState;
        break;

        case BTN_UP:
        case BTN_1:
          fanState = FAN_LOW;
        break;

        case BTN_2:
          fanState = FAN_MED;
        break;

        case BTN_3:
          fanState = FAN_HIGH;
        break;

        case BTN_DOWN:
        case BTN_0:
        default:
          fanState = FAN_OFF;
        break;
      }
    break;

    case FAN_LOW:
      switch (code)
      {
        case BTN_0:
        case BTN_DOWN:
        case BTN_PWR:
          lastOnState = FAN_LOW;
          fanState = FAN_OFF;
        break;

        case BTN_UP:
        case BTN_2:
          fanState = FAN_MED;
        break;

        case BTN_3:
          fanState = FAN_HIGH;
        break;

        case BTN_1:
        default:
          fanState = FAN_LOW;
        break;
      }
    break;

    case FAN_MED:
      switch (code)
      {
        case BTN_0:
        case BTN_PWR:
          lastOnState = FAN_MED;
          fanState = FAN_OFF;
        break;

        case BTN_DOWN:
        case BTN_1:
          fanState = FAN_LOW;
        break;

        case BTN_UP:
        case BTN_3:
          fanState = FAN_HIGH;
        break;

        case BTN_2:
        default:
          fanState = FAN_MED;
        break;
      }
    break;

    case FAN_HIGH:
      switch (code)
      {
        case BTN_0:
        case BTN_PWR:
          lastOnState = FAN_HIGH;
          fanState = FAN_OFF;
        break;

        case BTN_DOWN:
        case BTN_2:
          fanState = FAN_MED;
        break;

        case BTN_1:
          fanState = FAN_LOW;
        break;

        case BTN_UP:
        case BTN_3:
        default:
          fanState = FAN_HIGH;
        break;
      }
    break;

    default:
      fanState = FAN_OFF;

    break;
  }
 return fanState; 
}


//Function: executeState
//Description: This function uses a switch statement to drive the 7 segment display and the fan based off of which state is passed
//Parameters: int state
//Returns: N/A
//Globals: N/A
void executeState(int state)                    
{
  switch (state)
  {
    case 0:
      analogWrite(motorPin, 0);
      sevenSeg(0);

    break;

    case 1:
      analogWrite(motorPin, 90);
      sevenSeg(1);
    break;

    case 2:
      analogWrite(motorPin, 150);
      sevenSeg(2);
      
    break;

    case 3:
      analogWrite(motorPin, 210);
      sevenSeg(3);

    break;

    default:
      analogWrite(motorPin, 0);
      sevenSeg(99);

    break;
  }
  
}

//Function: sevenSeg
//Description: Drives the seven segment display using a switch statement for the number to be displayed
//Parameters: int num
//Returns: N/A
//Globals: N/A
void sevenSeg(int num)
{
  switch(num)
  {
    case 0:
      digitalWrite(SEG_A, HIGH);
      digitalWrite(SEG_B, HIGH);
      digitalWrite(SEG_C, HIGH);
      digitalWrite(SEG_D, HIGH);
      digitalWrite(SEG_E, HIGH);
      digitalWrite(SEG_F, HIGH);
      digitalWrite(SEG_G, LOW);
    break;
    
    case 1:
      digitalWrite(SEG_A, LOW);
      digitalWrite(SEG_B, HIGH);
      digitalWrite(SEG_C, HIGH);
      digitalWrite(SEG_D, LOW);
      digitalWrite(SEG_E, LOW);
      digitalWrite(SEG_F, LOW);
      digitalWrite(SEG_G, LOW);
    break;

    case 2:
      digitalWrite(SEG_A, HIGH);
      digitalWrite(SEG_B, HIGH);
      digitalWrite(SEG_C, LOW);
      digitalWrite(SEG_D, HIGH);
      digitalWrite(SEG_E, HIGH);
      digitalWrite(SEG_F, LOW);
      digitalWrite(SEG_G, HIGH);
    break;

    case 3:
      digitalWrite(SEG_A, HIGH);
      digitalWrite(SEG_B, HIGH);
      digitalWrite(SEG_C, HIGH);
      digitalWrite(SEG_D, HIGH);
      digitalWrite(SEG_E, LOW);
      digitalWrite(SEG_F, LOW);
      digitalWrite(SEG_G, HIGH);
    break;

    default:
      digitalWrite(SEG_A, LOW);
      digitalWrite(SEG_B, LOW);
      digitalWrite(SEG_C, LOW);
      digitalWrite(SEG_D, LOW);
      digitalWrite(SEG_E, LOW);
      digitalWrite(SEG_F, LOW);
      digitalWrite(SEG_G, HIGH);
    break;
      
  }
  
}


/*int returnState;
  
  switch(code)
  {
    case BTN_PWR:
      if (prevState != 0)
      {
        
        returnState = 0;
      }
      else
        returnState = prevState;
        
    break;

    case BTN_UP:
      if (state = 0)
        returnState = 1;
      else if (state = 1)
        returnState = 2;
      else if (state = 2)
        returnState = 3;
      else if (state = 3)
        returnState = 3;

    break;

    case BTN_DOWN:
      if (state = 0)
        returnState = 0;
      else if (state = 1)
        returnState = 0;
      else if (state = 2)
        returnState = 1;
      else if (state = 3)
        returnState = 2;

    break;

    case BTN_0:
      returnState = 0;
      
    break;

    case BTN_1:
      returnState = 1;
      
    break;

    case BTN_2:
      returnState = 2;
      
    break;

    case BTN_3:
      returnState = 3;
      
    break;

    default:
      returnState = 0;
    break;
    
  }
  return returnState;*/
