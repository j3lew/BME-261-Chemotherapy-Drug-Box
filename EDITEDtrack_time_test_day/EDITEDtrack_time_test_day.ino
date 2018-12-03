template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; } //sending to serial output for texting doctor 

//VARIABLES FOR ALARM SYSTEM/TIMING
unsigned long timeNow = 0; 
unsigned long timeLast = 0;
double seconds = 0;
int day = 1;
double warningTime = 12; //2 minutes before alarm = 0.2 min before alarm in a 6 minute day

//for testing, a 24 hour day is 6 minutes long
//for this setup, user will be starting arduino at 12 am

//VARIABLES FOR PYTHON
String morn, aft, eve, bed, person, age, treatment = "";
String patient[7];
//bool noDrug;

//VARIABLES FOR ALL OF FUNCTIONS
//photoresistor set up
const int photoResistor = A0;
const int photo_LED = 10;
int light_Average;
int light_Total = 0;
const int doctorLED = 5;

//thermistor set up
double temperature;
const int thermistor = A3;

//speaker set up
const int speaker = 12;

//timer LED set up
const int timer_LED = 2;

//temperature LED set up
const int temp_LED = 3;

void setup() {
  //SETUP FOR PYTHON
  Serial.begin(9600); //initialize serial COM at 9600 baudrate

  //SETUP FOR FUNCTIONS
  pinMode (photo_LED,OUTPUT);
  pinMode (temp_LED, OUTPUT);
  pinMode (timer_LED, OUTPUT);
  pinMode (speaker, OUTPUT);
  pinMode (doctorLED, OUTPUT);
}

void loop() {
  //FUNCTIONS
  set_Temperature_Light();

  //TIMING/ALARM SYSTEM 
  timeNow = millis()/1000;
  seconds = timeNow - timeLast;

  // 90
  if (seconds == 15) //6 am in seconds: 1 min 30 seconds
  {
//    Serial.println("Fill time1");
    sound_Warning_Alarm();
    patientInfo(day);
  }

  if (true) //ensuring that pills are present for the morning time1
  {
    // 105
    if (seconds == (30 - warningTime))
    {
      set_timer_Light(); 
    }
  
    else if (seconds == 30) //7 am in seconds: 1 min 45 seconds
    {
//      Serial.println("Morning time1");
      sound_Warning_Alarm();
    }
  }

  if (true) //ensuring that pills are present for the after time1
  {
    //135
    if (seconds == (45 - warningTime))
    {
      set_timer_Light();
    }
  
    else if (seconds == 45) //9 am in seconds: 2 min 15 seconds
    {
//      Serial.println("After time1");
      sound_Warning_Alarm();
    }
  }

   if (true) //ensuring that pills are present for the eve time1
   {
    // 255
    if (seconds == (60 - warningTime))
    {
      set_timer_Light();
    }
    
    else if (seconds == 60) //5 pm in seconds: 3 min 35 seconds
    {
//      Serial.println("Even time1");
      sound_Warning_Alarm();
    }
   }

   if (day == 1) //ensuring that pills are present for the bed time1
   {            
    // 330                                                                                                                                                                          
    if (seconds == (75 - warningTime))
    {
      set_timer_Light();
    }
    
    else if (seconds == 75) //11 pm in seconds: 5 min 30 seconds
    {
//      Serial.println("Bed time1");
      sound_Warning_Alarm();
    }
   }

  // 340
    if (seconds == 90) // 11:30 pm in seconds: 5 minutes 40 seconds
    {
      double light = read_Photoresistor();
      if (light < 915) //if this is the light value the photoResistor is reading, we know that pills are present, and patient has not taken pills
      //need to add sizeof morning, before, evening, bed != 0  so that we know box should be full but isn't
      {
        send_Doctor_Message();
      }
    }

//360
    if (seconds == 115) //if time1 can be divided by our 6 minute day, we know we have finished the day, and only 28 days in a drug schedule
    {
      updateDay();
    }
}

//FUNCTIONS
void send_Doctor_Message() {
  //Serial << "Patient " << patient << " did not take their chemotherapy medication for their " << treat << " treatment";
  digitalWrite(doctorLED, HIGH);
  delay(3000);
  digitalWrite(doctorLED, LOW);
  return;
}

double read_Thermistor () {
  const int SAMPLE_NUMBER = 10; //reading of ADC might have noise, so to eliminate noise, sample the ADC pin a few time1s and average samples
  const double BALANCE_RESISTOR = 10000; //using beta equation, must know other resistor in the resistor dividor (measure real resistance one day)
  const double MAX_ADC = 1023; //helps with calculating thermistor's resistance
  const double BETA = 4080; //beta value that corresponds to NTC thermistor 15K 
  const double ROOM_TEMP = 298.15; //typical room temp in Kelvin
  const double RESISTOR_ROOM_TEMP = 15000; //thermistor resistance at 25C in Kelvin

  double rThermistor = 0; //thermistor resistance value
  double tKelvin = 0; //calculated temperature
  double tCelsius = 0; //temp in celsius
  double adcAverage = 0; //average voltage measurement
  int adcSamples [SAMPLE_NUMBER]; //array of the voltage measurements sample
  
  adcAverage = analogRead(thermistor);
  rThermistor = BALANCE_RESISTOR*((MAX_ADC/adcAverage) -1); //calculating thermistor's resistance
  tKelvin = (BETA*ROOM_TEMP)/(BETA + (ROOM_TEMP * log(rThermistor/RESISTOR_ROOM_TEMP))); //calculating temperature in Kelvin using beta value

  tCelsius = tKelvin - 273.15; //convert to celsuis
  return tCelsius;
}

double read_Photoresistor() {
  light_Total = 0;
  digitalWrite (photo_LED, HIGH); 
  for (int i = 0; i < 10; i ++) {
    light_Total += analogRead(photoResistor);
    delay (100);
  }  
  digitalWrite(photo_LED, LOW);
  light_Average = light_Total / 10;
  return light_Average;
}

void sound_Warning_Alarm() {
    tone(speaker,349);    
    // Waits some time1 to turn off
    delay(200);
    //Turns the buzzer off
    noTone(speaker); 
    // Sounds the buzzer at the frequency relative to the note D in Hz   
    tone(speaker,392);             
    delay(200);    
    noTone(speaker); 
    // Sounds the buzzer at the frequency relative to the note E in Hz
    tone(speaker,440);      
    delay(200);
    noTone(speaker);     
    // Sounds the buzzer at the frequency relative to the note F in Hz
    tone(speaker,493);    
    delay(200);    
    noTone(speaker); 
    // Sounds the buzzer at the frequency relative to the note G in Hz
    tone(speaker,523);            
    delay(200);
    noTone(speaker); 
}

void set_Temperature_Light() {
  temperature  = read_Thermistor();
  if (temperature >= 35 || temperature <= 20)
  {
    digitalWrite(timer_LED, LOW);
    digitalWrite(temp_LED, HIGH);
    
  }
  else
  {
    digitalWrite(temp_LED,LOW);
  }
}

void set_timer_Light(){
  digitalWrite(temp_LED, LOW);
  if (temperature < 35 && temperature > 20)
  {
    digitalWrite(temp_LED,LOW);
    for (int i = 0; i < 6; i++) {
    digitalWrite(timer_LED,HIGH);
    delay (1000);
    digitalWrite(timer_LED, LOW);
    delay (1000);
    }
    
  }
  else if (temperature >= 35 || temperature <= 20)
  {
    for (int i = 0; i < 6; i++) {
    digitalWrite(temp_LED,LOW);
    digitalWrite(timer_LED,HIGH);
    delay (1000);
    digitalWrite(timer_LED, LOW);
    digitalWrite(temp_LED,HIGH);
    delay (1000);
  } 
  }
}

//TIMING/ALARM SYSTEM
void updateDay()
{
  timeLast = timeNow; //switch to reset the times, as it is the next day 
  day = day + 1;
}


void patientInfo(int day){ //get drugs and personal info of the day
  String dayString = String(day);
  Serial.println(dayString);
  for(int i = 0; i < 7; i++){
    patient[i] = Serial.readStringUntil(';');
//    if(patient[i].substring(patient[i].length()-3, patient[i].length()) == "N/A"){
//      patient[i] == NULL; // if there is no drug for that time --> recognized by "N/A"
//    }
  }
  person = patient[0];
  age = patient[1];
  treatment = patient[2];
  morn = patient[3];
  aft = patient[4];
  eve = patient[5];
  bed = patient[6];
}
