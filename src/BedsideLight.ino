/*

   Board: ATTiny 24/45/85
*/



#include <ADCTouch.h> //https://github.com/martin2250/ADCTouch/blob/master/examples/Buttons/Buttons.ino

/* ---------------------------------
           Settings
   ---------------------------------
*/
// definitions for ATTiny85
#define pinTouch        A2       // Pin3 (PB4)  Pin with Toucharea
#define ledPin      0       // LED 0 = D0  =Pin5 (PB0)
#define buzzerPin   3       //Buzzer   3=D3=Pin2 (PB3)

int ref0;                   //reference values to remove offset
unsigned long myTime = 0;
int myCount = 0;

/*
   Treshold defines the time, when next change will become effective
*/
int buttonTreshold = 30;
int statusTreshold1 = 5;
int statusTreshold2 = 25;
int statusTreshold3 = 35;
int statusTreshold4 = 45;
int statusTreshold5 = 55;

int statusLED = 0;          // store status of LED

/* ---------------------------------
            Setup
   ---------------------------------
*/
void setup()
{
  // define pin usage
  pinMode(ledPin, OUTPUT);      // LED
  pinMode(buzzerPin, OUTPUT);   // Buzzer

  ref0 = ADCTouch.read(pinTouch, 500);    //create reference values to account for the capacitance of the pad


}


/* ---------------------------------
            Mainloop
   ---------------------------------
*/
void loop()
{


  int value0 = ADCTouch.read(pinTouch);   //read from touch area

  value0 -= ref0;                         //remove offset

  delay(50);
  digitalWrite(buzzerPin, HIGH); //turn off buzzer


  // analyze how long touch event appears and react accordingly
  if (value0 > buttonTreshold)
  {
    myCount++;

    if (myCount == statusTreshold1 || myCount == statusTreshold2 || myCount == statusTreshold3 || myCount == statusTreshold4 || myCount == statusTreshold5) {
      digitalWrite(buzzerPin, LOW);
    }
    if (myCount > statusTreshold2 && myCount < statusTreshold3) {
      statusLED = 3;    // Dimm1
    }
    if (myCount > statusTreshold3 && myCount < statusTreshold4) {
      statusLED = 4;    // Dimm2
    }
    if (myCount > statusTreshold4 && myCount < statusTreshold5) {
      statusLED = 5;    // Dimm3
    }
    if (myCount > statusTreshold5 ) { //zurücksetzten auf ersten Dimmerzustand
      statusLED = 3;    // Dimm3
      myCount = statusTreshold2;
    }
  }

  else {

    if (myCount > statusTreshold1 && myCount < statusTreshold2) {
      if (statusLED != 1) {
        statusLED = 1;    // Light on
      }
      else {
        statusLED = 2;    //Light off
      }
    }
    myCount = 0;
  }


  switch (statusLED) {
    case 1:
      analogWrite(ledPin, 255);
      break;
    case 2:
      analogWrite(ledPin, 0);
      break;
    case 3:
      analogWrite(ledPin, 40);
      break;
    case 4:
      analogWrite(ledPin, 100);
      break;
    case 5:
      analogWrite(ledPin, 180);
      break;
  }
}
