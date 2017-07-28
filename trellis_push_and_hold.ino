*
 * use the trellis library set individual buttons to momentary or latching,
 * providing accessible arrays to hold variable for each button. Also provides
 * press and hold functionality for trellis buttons.
 * No delays are used, except in the startup button pattern. This frees up the
 * microcontroller to do things.
 *  --mortonkopf--
 */
#include <Wire.h>
#include "Adafruit_Trellis.h"

Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0);

#define NUMTRELLIS 1
#define numKeys (NUMTRELLIS * 16)

unsigned long lastDebounceTime0=0,newDebounceTime0=0;

//===========variables for button states==============//
int buttonState[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};     // current state of the button
int lastButtonState[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // previous state of the button
unsigned long startPressed[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};    // the time button was pressed
unsigned long endPressed[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};      // the time button was released
unsigned long timeHold[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};        // the time button is hold
unsigned long timeReleased[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};    // the time button is released

/* 
 * Momentary pairs are uses to increase and decrease a value = 0+4 (=tMV 0), 1+5 (tMV 1), 2+6, (tMV 2), 3+7, 16+20, 17+21, 18+22, 19+23
 * they alter the value in trellisMomentValue[]
 */

//used to map across trellis button number into on/off state for easy access
//int trellisOnOff[]={ 
//  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
//};

int trellisMomentValue[]={
  5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5
};

//store incremented values in momentary array, and store on/off values in latched array
//reversed arrangement
int momentary[]={
0,1,2,3,4,5,6,7,
16,17,18,19,20,21,22,23};

int latched[] ={
31,30,29,28,15,14,13,12,
27,26,25,24,11,10,9,8
};

void setup() {
  Serial.begin(31250);
  Serial.println("Trellis Demo");

  trellis.begin(0x71);
  trellis.setBrightness(15);
  // light up all the LEDs in order
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.setLED(i);
    trellis.writeDisplay();    
    delay(20);
  }
  // then turn them off
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.clrLED(i);
    trellis.writeDisplay();    
    delay(20);
  }
}


void loop() {
  doTrellis();
}


//============================trellis button pad read=========================//
void doTrellis(){
  newDebounceTime0 =millis();
  if ((newDebounceTime0 - lastDebounceTime0 ) > 20){
      lastDebounceTime0 = newDebounceTime0;
  
 // If a button was just pressed or released...
 if (trellis.readSwitches()) {

 for (uint8_t i=0; i<numKeys; i++) {
  if (trellis.justPressed( momentary[i])) {// if it was pressed, turn it on
    trellis.setLED(momentary[i]);

   // Do within the trellis.justpressed { } - this part is the hold variation
          startPressed[i] = millis();
          timeReleased[i] = startPressed[i] - endPressed[i];    
    }//end of trellis.justpressed
    
  if (trellis.justReleased(momentary[i])) {// if it was released, turn it off
    trellis.clrLED(momentary[i]);
    
    //below added within justreleased for long hold
          endPressed[i] = millis();
          timeHold[i] = endPressed[i] - startPressed[i]; 
          
          if (timeHold[i]  <= 399) {
            Serial.print("momentary "); Serial.print(momentary[i]); Serial.print("  short press, changes paired buttons ");
            //can add a function here similar to below, or use the caseTest() example
            // go and do something with this ==> 
            caseTest(i); //uncomment to increase or decrease the array value for the momentary button pairs
          }

          if (timeHold[i] >= 400 && timeHold[i] < 1500) {
              Serial.print("Button held for 400ms - goDoSomething(); with button: ");
              Serial.println(i);
              goDoThisMedium(i);
              //go and do something with this = doThisThing(); 
          }

          if (timeHold[i] >= 1500) {
              Serial.print("Button held for more than 1500ms - goDoAnotherThing();with button: ");
              Serial.println(i);
              goDoThisLong(i);//example of a funtion that relates to long press for this particular button
              //go and do something with this = doThisOtherThing();
          }
    
    }//end of trellis.justreleased
    
 }//end of for loop for momentary buttons
 
    for(int bottom=0;bottom<16;bottom++){
      if(trellisMomentValue[bottom] <= 1){ trellisMomentValue[bottom] = 1;} 
      }      
    for(int top=0;top<16;top++){ //constrain the variable
      if(trellisMomentValue[top] >= 9){ trellisMomentValue[top] = 9;} 
      }
      
//===== end of momentary check, now check if latch allocated buttons have been pressed ===>

for (uint8_t i=0; i<numKeys; i++) {
    if (trellis.justPressed(latched[i])) {
    // Alternate the LED
    if (trellis.isLED(latched[i])){
      trellis.clrLED(latched[i]);
       Serial.print("latched "); Serial.print(latched[i]);Serial.println("  off");
  // insert a function here such as  trellisOnOff[i]=0;
      }//button latch OFF
    else
  { trellis.setLED(latched[i]);//button latch ON
   Serial.print("latched "); Serial.print(latched[i]);Serial.println("  on");
 //  insert a funciton here such as trellisOnOff[i]=1;
        }
      }
    }
//====all button checking done, now update the display with the results ==>
    
    trellis.writeDisplay();
for(int i=0;i<numKeys;i++){
  lastButtonState[i] = buttonState[i];
}
   }//end of readswitches
  }//end of debouce time
  
}//end of do trellis

//=======================two example functions for two hold lengths========//
/*this function is called for a medium press for a given button.
 * you can pass the button number variable to the function
 */
void goDoThisMedium(int i){
    Serial.print("i am in a meduim-press hold function for button: ");
    Serial.println(i);
}

/*this function is called for a long press for a given button.
 * you can pass the button number variable to the function
 */
void goDoThisLong(int i){
    Serial.print("i am in a long-press hold function for button: ");
    Serial.println(i);
}

//==============case code for momentary button pairs========//
void caseTest(int i) {
switch(i) {
   case 0:    
trellisMomentValue[0]=trellisMomentValue[0]-1;
Serial.print("0 and 4 down to: ");
 Serial.println(trellisMomentValue[0]);
      break;
    case 1: 
trellisMomentValue[1]=trellisMomentValue[1]-1;
Serial.print("1 and 5 down to: ");
 Serial.println(trellisMomentValue[1]);
      break;
      case 2:
trellisMomentValue[2]=trellisMomentValue[2]-1;
Serial.print("2 and 6 down to: ");
 Serial.println(trellisMomentValue[2]);
      break;
    case 3: 
trellisMomentValue[3]=trellisMomentValue[3]-1;
Serial.print("3 and 7 down to: ");
 Serial.println(trellisMomentValue[3]);
      break;
      case 4:
trellisMomentValue[0]=trellisMomentValue[0]+1;
Serial.print("0 and 4 up to: ");
 Serial.println(trellisMomentValue[0]);
      break;
    case 5: 
trellisMomentValue[1]=trellisMomentValue[1]+1;
Serial.print("1 and 5 up to: ");
 Serial.println(trellisMomentValue[1]);
      break;
      case 6:
trellisMomentValue[2]=trellisMomentValue[2]+1;
Serial.print("2 and 6 up to: ");
 Serial.println(trellisMomentValue[2]);
      break;
    case 7: 
trellisMomentValue[3]=trellisMomentValue[3]+1;
Serial.print("3 and 7 up to: ");
 Serial.println(trellisMomentValue[3]);
      break;      
    }
}
