// include the library for LCD
#include <math.h>
#include <LiquidCrystal.h>

// Variables
String data = "";
String morning, after, evening, bed, patient, age, treatment = "";
String infoForDay[7];
unsigned int time;

// Time variables
unsigned long timeNow = 0; 
unsigned long timeLast = 0;
int seconds = 0;
int day = 1; // initial day that is incremented by 1 for every 30 seconds ("day") 

const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600); //initialize serial COM at 9600 baudrate

  // update current time
  currentTime();

  //find drugs in day
  getData(day); 
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Display sentence on lcd
//  String test = "test";
//  int str_len = test.length() + 1;
//  char test1[str_len];
//  test.toCharArray(test1, str_len);
//  lcd.print(test1);
  lcd.print("My name is Bill!");
  lcd.setCursor(0, 1);
  lcd.print("I will help you take your pills!");
  lcd.display();
  delay(1000);

  // scroll to the right
  for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(500);
  }
  delay(1000);
  
  // call instructionLCD
  instructionLCD();
}

void loop() {
}

void currentTime(){
  //TIMING/ALARM SYSTEM IN LOOP
  timeNow = millis()/1000;
  seconds = timeNow - timeLast;
}

//TIMING/ALARM SYSTEM
void updateDay()
{
  timeLast = timeNow; //switch to reset the times, as it is the next day 
  if(day < 28){
    day = day + 1;
  }
  else{
    day = 1;
    setup();
  }
  getData(day);
  instructionLCD();
} 

void getData(int day) { //get drugs and personal info of the day
  String dayString = String(day);
  Serial.println(day);
  for ( int i = 0; i < 7; i++) {
    infoForDay[i] = Serial.readStringUntil(';');
  }

  // Test
  //  for( int c = 0; c < 7; c++){
  //    Serial.println(patient[c]);
  //  }
  
  patient = infoForDay[0];
  age = infoForDay[1];
  treatment = infoForDay[2];
  morning = infoForDay[3];
  after = infoForDay[4];
  evening = infoForDay[5];
  bed = infoForDay[6];
}

void instructionLCD() {
  lcd.clear();
  // converting string to char array
  int str_len = patient.length() + 1;
  char patientLCD[str_len];
  patient.toCharArray(patientLCD, str_len);
  
  lcd.setCursor(0, 0);
  lcd.print("Day" + String(day) + ":Hi " + String(patientLCD) + "!");
  lcd.setCursor(0, 1);
  lcd.print("Press R-M,U-A,L-E,D-B,S-I");
  lcd.display();

  // scroll right left right
  scroll(str_len);
}

void morningLCD() {
  lcd.clear();

  // converting string to char array
  int str_len = morning.length() + 1;
  char morningDisplay[str_len];
  morning.toCharArray(morningDisplay, str_len);

  lcd.setCursor(0, 0);
  lcd.print(String(morningDisplay));
  lcd.setCursor(0, 1);
  lcd.print("Press R-M,U-A,L-E,D-B,S-I");
  lcd.display();

  // scroll right left right
  scroll(str_len);
}

void afterLCD() {
  lcd.clear();
  
  // converting string to char array
  int str_len = after.length() + 1;
  char afterDisplay[str_len];
  after.toCharArray(afterDisplay, str_len);

  lcd.setCursor(0, 0);
  lcd.print(after);
  lcd.setCursor(0, 1);
  lcd.print("Press R-M,U-A,L-E,D-B,S-I");
  lcd.display();
  
  // scroll right left right
  scroll(str_len);
}

void eveLCD() {
  lcd.clear();

  // converting string to char array
  int str_len = evening.length() + 1;
  char eveningDisplay[str_len];
  evening.toCharArray(eveningDisplay, str_len);

  lcd.setCursor(0, 0);
  lcd.print(evening);
  lcd.setCursor(0, 1);
  lcd.print("Press R-M,U-A,L-E,D-B,S-I");
  lcd.display();

  // scroll right left right
  scroll(str_len);
}

void bedLCD() {
  lcd.clear();

  // converting string to char array
  int str_len = bed.length() + 1;
  char bedDisplay[str_len];
  bed.toCharArray(bedDisplay, str_len);

  lcd.setCursor(0, 0);
  lcd.print(bed);
  lcd.setCursor(0, 1);
  lcd.print("Press R-M,U-A,L-E,D-B,S-I");
  lcd.display();

  // scroll right left right
  scroll(str_len);
}

void scroll(int str_len){
  delay(1000);
  int minLength = min(str_len, 24);

  boolean infiniteLoop = true;
  while(infiniteLoop){ // loop to scroll to the right
    // scroll to the right
    for (int positionCounter = 0; positionCounter < minLength; positionCounter++) {
      delay(200);
      // scroll one position left:
      lcd.scrollDisplayLeft();

      // updating current time and update day
      currentTime();
      if(seconds == 140){
        updateDay();
      }
      
      // check for button
      button();
      // wait a bit:
    }
    lcd.setCursor(0, 0);
  }
}

void button(){
  // reading the button
  int button = analogRead(0);
  if (button < 50) { // right button
    morningLCD();
  }
  else if(button < 195){ // up button
    afterLCD();
  }
  else if(button < 380){ // down button 
    bedLCD();
  }
  else if(button < 555){ // left button
    eveLCD();
  }
  else if(button < 790) { // select button
    instructionLCD(); 
  }
}
