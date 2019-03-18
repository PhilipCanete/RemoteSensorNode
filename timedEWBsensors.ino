#include <TimerOne.h>
#include <Wire.h>
#include <Adafruit_AM2315.h>
#include "definitions.h"


volatile int secondsTimer;  //Incremented by ISR
int seconds = 0;            //Initialize non volatile seconds timer
bool clearTimer = false;    //Signals to clear timer every ### seconds
Adafruit_AM2315 am2315;

void setup() {
  Timer1.initialize(1000000);   //Initialize timer to interrupt at 1 second per period
  Timer1.attachInterrupt(isr);  //Go to interrupt service routine every period
  am2315.begin();                //Initialize adafruit am2315 library
  Serial.begin(9600);
}

void loop() {  
  noInterrupts();    //Must turn off interrupts while accessing volatile variables
    seconds = secondsTimer;   //Set non voltile seconds timers
    if(clearTimer)  //This loop clears the timer and the timer signal when set in the loop
      {
        secondsTimer = 0;
        Timer1.restart();
        clearTimer = false;
      } 
  interrupts();      //Turn on interrupts after accessing variable

  
 if(seconds == 5) {  //Change this to change seconds between data logs
        Serial.print("Windspeed: "); Serial.println(windspeed());
        Serial.print("Humidity: "); Serial.println(am2315.readHumidity());
        Serial.print("Temperature: "); Serial.println(am2315.readTemperature());
        
        //insert data logging here
        clearTimer = true;
        seconds = 0;
    }
}

void isr() {
  secondsTimer++;
}

int windspeed() {
  int sensorValue = 0; //Variable stores the value direct from the analog pin
  float sensorVoltage = 0; //Variable that stores the voltage (in Volts) from the anemometer being sent to the analog pin
  float windSpeed = 0; // Wind speed in meters per second (m/s)
  
  sensorValue = analogRead(sensorPin); //Get a value between 0 and 1023 from the analog pin connected to the anemometer
  sensorVoltage = sensorValue * voltageConversionConstant; //Convert sensor value to actual voltage
  
  //Convert voltage value to wind speed using range of max and min voltages and wind speed for the anemometer
  if (sensorVoltage == voltageMin){
   windSpeed = 0; //Check if voltage is below minimum value. If so, set wind speed to zero.
  }else {
    windSpeed = (sensorVoltage - voltageMin)*windSpeedMax/(voltageMax - voltageMin); //For voltages above minimum value, use the linear relationship to calculate wind speed.
  }
}

//todo
//1. add temp+humidity code
//2. add wind speed code
//3. add fona808 code
//4. make time between data logs a variable
//5. turn insantaneous sensing into average
