/*

  This program was initially used to demo the functionality of the arduino with an Ultrasonic sensor.
  When an object comes within a distance from the Ultrasonic module, this is noted on an LCD Display.
    A counter is then updated.

*/

#include <LiquidCrystal.h>
#include <SR04.h>
#define TRIG_PIN 12
#define ECHO_PIN 11

// Creates the Ultrasonic sensor based on its pins on the board
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long dist;
long tempDist;
int counter;

LiquidCrystal lcd(1, 2, 4, 5, 6, 7);

void setup() {

  // Sets some variables up so that we have a count of how many objects we have and sets the LCD to print necessarily
  counter = 20;
  lcd.begin(16, 2);
  lcd.print("CARPARK = ");
  lcd.print(counter);
  delay(1000);
}

void loop() {
  
  // This loop goes continuously

  lcd.clear();
  lcd.print("CARPARK = ");
  lcd.print(counter);
  
  lcd.setCursor(0, 1);

  dist = sr04.Distance();

  // Prints the detected distance to the LCD
  lcd.print(dist);
  lcd.print(" cm away");

  delay(200);

  // If an object is detected between 30cm and 60cm, it goes into a detection check
  if(dist > 30 && dist < 60 && counter > 0){
    detect();
  }

  delay(1000);
}

void detect(){

  // Decrements the number available 
  counter = counter-1;
  
  // Gets the distance from the Ultrasonic sensor to the nearest object
  tempDist = sr04.Distance();

  // While an object is in between 30cm and 60cm away, it continuinly checks and changes to display the distance away the object is
  while(tempDist > 30 && tempDist < 60){
    lcd.clear();
    lcd.print("CARPARK = ");
    lcd.print(counter);
    lcd.setCursor(0, 1);
    lcd.print("Approaching-");
    lcd.print(tempDist);
    delay(100);
    tempDist = sr04.Distance();
  }
  
}

// A method used to clear the LCD diaplay
void clearPrint(){
  lcd.setCursor(0, 0);
  lcd.print("");
  lcd.setCursor(0, 1);
  lcd.print("");
}

