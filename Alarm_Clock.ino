#include <LiquidCrystal.h>

// Initialize the LCD Interface (RS, E, D4, D5, D6, D7)- ports on the LCD to wire ports 
LiquidCrystal lcd (2, 3, 4, 5, 6, 7);

//Time variables
int hours = 12; //Default hour
int minutes = 0; //default minutes 
bool isitAM = true; // AM or PM toggle 

//Button Pins
const int setButton = 8; //Button to toggle the Setting mode
const int upButton = 9; //Button used to increment values 
const int downButton = 10; //Button used to decrement values 

//the State variables
int mode = 0; //0: Sets Mode, 1: Sets Hours, 2: Sets Minutes
unsigned long lastUpdateTime = 0; //Tracks the last time the clock updated 

void setup () 
{
  lcd.begin(16, 2); //Initializing the LCD to 16 columns and 2 rows 
  pinMode (setButton, INPUT_PULLUP); //Note to self: INPUT_PULLUP monitors the state of the switch
  pinMode (upButton, INPUT_PULLUP);
  pinMode (downButton, INPUT_PULLUP);
  lcd.print ("Simple Clock :).");
  delay (2000);
  lcd.clear();
}


void loop() {
  handleButtons(); //Handle the button presses 
  if (mode == 0 && millis() - lastUpdateTime >= 60000) { //Note: millis() is a function that returns the number of milliseconds that have passed since the Arduino Board began running current program
    incrementMinutes();
    lastUpdateTime = millis();
  }
  displayTime(); //updating the LCD Display 
}

void handleButtons(){
  if (digitalRead(setButton) == LOW) { //when the button is pressed 
    delay(200); 
    mode = (mode + 1) % 3; //It will cycle throgh all the modes (0 -> 1 -> 2)
  }
  
  if (digitalRead(upButton) == LOW) {
    delay(200);
    if (mode == 1) {
      if (hours == 11) {
        isitAM = !isitAM;
      }
      hours = (hours % 12) + 1; //Increments from 1 to 12 
    } else if (mode == 2) {
      minutes = (minutes + 1) % 60; //Increments from 0 to 59
    }
  }

  if (digitalRead (downButton) == LOW) {
    delay(200);
    if (mode == 1) {
      if (hours == 12) {
        isitAM = !isitAM;
      }
      hours = (hours == 1) ? 12 : hours - 1; //(if you're at 1, then you want to decrement it to 12, otherwise just decrement the value by 1) 
    } else if (mode == 2) {
      minutes = (minutes == 0) ? 59: minutes -1 ; //if you're at 0, then you want it to decrement to 59, otherwise just decrement the value by 1
    }
  }
}

void incrementMinutes () {
  minutes ++;
  if (minutes >=60) {
    minutes = 0;
    hours ++;
    if (hours > 12) {
      hours = 1;
      isitAM = !isitAM; //togles the AM & PM
    }
  }
}

//Displaying the current time on the LCD 
void displayTime() {
  lcd.setCursor (0,0);
  lcd.print ("Time: ");
  if (hours < 10) {
    lcd.print ("0"); //Adding a leading 0
  }
  lcd.print (hours);
  lcd.print (":");
  if (minutes < 10) {
    lcd.print ("0"); //Adding a leading 0
  }
  lcd.print (minutes);

  //displaying the AM & PM 
  lcd.setCursor (12, 0);
  lcd.print (isitAM ? "AM" : "PM");

  //showing the setting mode on the LCD Display 
  lcd.setCursor (0,1);
  if (mode == 0) {
    lcd.print ("Normal Mode");
  } else if (mode == 1) {
    lcd.print ("Set Hours       ");
  } else if (mode == 2) {
    lcd.print ("Set Minutes     ");
  }
}
