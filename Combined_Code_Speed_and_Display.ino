// Code Developed by Joshua Abrams (609)410-6460 jmacomputerservices@gmail.com
// 03/25/2019 This is a meant to use 4 sensors to calculate the time and 
// display it to each corresponding track or lane Using two Max7219 displays 
// a library is needed and will display the result of one track with two lanes 
// to two seperate displays and corresponding LED's 

  #include "LedControl.h"                   //Must add this library for MAX7219
                                            //Display one Left side of track
  LedControl lc1=LedControl(12,11,10,1);    // VCC 5v  // GND ground // Din pin 12
                                            // CS pin 10 // Clk pin 11
  
                                            //Display two right side of track
  LedControl lc2=LedControl(9,8,7,1);       // VCC 5v  // GND ground // Din pin 9 
                                            // CS pin 7 // Clk pin 8
    
  #define SENSORPIN1 4                      //arduino digital pin 4   
  #define SENSORPIN2 2                      //arduino digital pin 2
  
  #define SENSORPIN3 3                      //arduino digital pin 3   
  #define SENSORPIN4 5                      //arduino digital pin 5
     
  // variables //
  unsigned int num = 0;                     //variables for display output
  unsigned int one,ten,hundred;             //for decimal place to Max7219

  unsigned int num2 = 0;                    //variables for display output
  unsigned int one2,ten2,hundred2;          //for decimal place to Max7219
  
  int ledone1 = 43;                         // the pin the LED is connected to
  int ledone2 = 2;                          // the pin the LED is connected to
  int ledtwo1 = 46;                         // the pin the LED is connected to
  int ledtwo2 = 49;                         // the pin the LED is connected to


  int sensorState1 = 0;
  int sensorState2 = 0;   
  int sensorState3 = 0;
  int sensorState4 = 0;
  
  unsigned long timeFirst1;           //millis for sensor 1
  unsigned long timeSecond1;          //millis for sensor 2
  
  unsigned long timeFirst2;           //millis for sensor 3
  unsigned long timeSecond2;          //millis for sensor 4

  unsigned long currentTime;          //for turning off the LED's
  int duration = 5000 ;               //5 seconds in milliseconds
  
  float difference1;                  //timeSecond1 - timeFirst1
  float difference2;                  //timeSecond2 - timeFirst2
  float Secondsdifference1;           // divided by 1000 to get Seconds  
  float Secondsdifference2;           // divided by 1000 to get Seconds 
  
  int trigger1 = 0;                   // Sensor 1
  int trigger2 = 0;                   // Sensor 2
  int trigger3 = 0;                   // Sensor 3
  int trigger4 = 0;                   // Sensor 4
  int lightTrigger1 = 0;
  int lightTrigger2 = 0;
     
    void setup() 
  {
    pinMode(ledone1, OUTPUT);       // Declare the LED as an output
    pinMode(ledone2, OUTPUT);       // Declare the LED as an output
    pinMode(ledtwo1, OUTPUT);       // Declare the LED as an output
    pinMode(ledtwo2, OUTPUT);       // Declare the LED as an output
                                    //Setup the Max7219 display 1 
    lc1.shutdown(0,false);
    lc1.setIntensity(0,8);
    lc1.clearDisplay(0);
                                    //Setup the Max7219 display 2 
    lc2.shutdown(0,false);
    lc2.setIntensity(0,8);
    lc2.clearDisplay(0);
    
    Serial.begin(9600);             //for serial display Monitor 
          
                                    // initialize the sensor pin as an input:
    pinMode(SENSORPIN1, INPUT);     
    digitalWrite(SENSORPIN1, HIGH); // turn on the pullup
    
    pinMode(SENSORPIN2, INPUT);     
    digitalWrite(SENSORPIN2, HIGH); // turn on the pullup
    
    pinMode(SENSORPIN3, INPUT);     
    digitalWrite(SENSORPIN3, HIGH); // turn on the pullup
    
    pinMode(SENSORPIN4, INPUT);     
    digitalWrite(SENSORPIN4, HIGH); // turn on the pullup
 ////End of Setup /////////
  }
  
//// Function to determine speed
void speed1()
{
    // subtract end time from start time to get total time
    difference2 = ((timeSecond2 - timeFirst2));
    Secondsdifference2 = difference2 / 1000;
    Serial.print("The difference in seconds is Track 1 LEFT SIDE: ");
    Serial.println(Secondsdifference2);  
// This is how you print out a float value onto a Max7219 by breaking 
// it into its seperate components and  displaying each one seperately 
// in the right digit Seconds Difference should be a number like 2.35 or
// 5.31 seconds  and can display it, this will not work with numbers larger 
// than 9.99 but with some tinkering can be modified to adapt larger numbers
    num = Secondsdifference2 * 100;
    one=num%10;
    ten=(num/10)%10;
    hundred=(num/100)%10;
    
    lc1.setDigit(0,3,(byte)hundred,1);  //lc.setdigit(addr , digits , value , dp);
    lc1.setDigit(0,2,(byte)ten,0);
    lc1.setDigit(0,1,(byte)one,0);
}
//// Function to determine speed
void speed2()
  {
    // subtract end time from start time to get total time
    difference1 = ((timeSecond1 - timeFirst1));
    Secondsdifference1 = difference1 / 1000;                                      
    Serial.print("The difference in seconds is Track 2 RIGHT SIDE: ");
    Serial.println(Secondsdifference1);  
// This is how you print out a float value onto a Max7219 by breaking 
// it into its seperate components and  displaying each one seperately 
// in the right digit Seconds Difference should be a number like 2.35 or
// 5.31 seconds  and can display it, this will not work with numbers larger 
// than 9.99 but with some tinkering can be modified to adapt larger numbers
    num2 = Secondsdifference1 * 100;
    one2=num2%10;
    ten2=(num2/10)%10;
    hundred2=(num2/100)%10;

    lc2.setDigit(0,3,(byte)hundred2,1);  //lc.setdigit(addr , digits , value , dp);
    lc2.setDigit(0,2,(byte)ten2,0);
    lc2.setDigit(0,1,(byte)one2,0);
  }

void loop() 
{
   
    currentTime = millis();
//turn off the light if it's been on for over 5 seconds and reset the trigger 
    if ( currentTime - timeSecond1 >= duration )    
    {
      digitalWrite(ledone1, LOW);
      digitalWrite(ledone2, LOW);
      lightTrigger1 = 0;
      lightTrigger2 = 0; 
    }
//turn off the light if it's been on for over 5 seconds and reset the trigger 
    if ( currentTime - timeSecond2 >= duration )    
    {
      digitalWrite(ledtwo1, LOW);
      digitalWrite(ledtwo2, LOW);
      lightTrigger1 = 0;
      lightTrigger2 = 0;  
    }
    lc1.setChar(0,7,'L',false);                     //permanent lane markers left track
    lc1.setChar(0,6,'1',false);
    lc2.setChar(0,7,'L',false);                     //permanent lane markers right track
    lc2.setChar(0,6,'2',false);
                                                    // Read the state of the IR sensor 1:
    sensorState1 = digitalRead(SENSORPIN1);
                                                    // See if IR beam of sensor 1 has been broken
    if (sensorState1 == LOW) 
      {
        Serial.println("IR Beam 1 Broken");         // For testing what is wrong printed to console
                                                    // Check to make sure both sensors have not triggered
      if (trigger1 == 0 && trigger2 == 0) 
       {
                                                    // Save time when sensor 1 was triggered
        timeFirst1 = millis();
                                                    // Prevent sensor 1 from triggering again
        trigger1 = 1;
        }
      }
                                                    // Read the state of the IR sensor 4:
    sensorState4 = digitalRead(SENSORPIN4);
                                                    // See if IR beam of sensor 2 has been broken
    if (sensorState4 == LOW) 
      {
        Serial.println("IR Beam 2 Broken");         // For testing what is wrong printed to console  
                                                    // Check to make sure sensor 1 has triggered but not sensor2
      if (trigger2 == 0 && trigger1 == 1) 
        {
                                                    // Save time when sensor 2 was triggered
          timeSecond1 = millis();

                                                    // Run speed function
           speed2();
           if ( lightTrigger1 == 0 && lightTrigger2 == 0 )
           {
           digitalWrite(ledone1, HIGH); 
           digitalWrite(ledone2, HIGH);             //turn on the light when you reach the end of the track 
           lightTrigger1 = 1;                       // Light 1 is on
           }                                        // Prevent sensor 2 from triggering again
          trigger2 = 1;
        }
                                                    // Reset both sensors
      trigger1 = 0;
      trigger2 = 0;
  }
    
////Break between relationship between sensor 1 and 2 , and now for 3 and 4 ////////////////////////////
  
                                                  // Read the state of the IR sensor 3:
    sensorState3 = digitalRead(SENSORPIN3);
                                                  // See if IR beam of sensor 1 has been broken
    if (sensorState3 == LOW) 
      {
        Serial.println("IR Beam 3 Broken");       // For testing what is wrong printed to console  
                                                  // Check to make sure both sensors have not triggered
      if (trigger3 == 0 && trigger4 == 0) 
        {
                                                  // Save time when sensor 1 was triggered
        timeFirst2 = millis();
                                                  // Prevent sensor 3 from triggering again
        trigger3 = 1;
        }
      }
                                                  // Read the state of the IR sensor 4:
    sensorState2 = digitalRead(SENSORPIN2);
                                                  // See if IR beam of sensor 2 has been broken
    if (sensorState2 == LOW) 
      {
        Serial.println("IR Beam 4 Broken");       // For testing what is wrong printed to console  
                                                  // Check to make sure sensor 1 has triggered but not sensor2
      if (trigger4 == 0 && trigger3 == 1) 
        {
                                                  // Save time when sensor 2 was triggered
          timeSecond2 = millis();
                                                  // Run speed function
          speed1();
          if ( lightTrigger1 == 0 && lightTrigger2 == 0 )
           {
           digitalWrite(ledtwo1, HIGH); 
           digitalWrite(ledtwo2, HIGH);           //turn on the light when you reach the end of the track 
           lightTrigger2 = 1;                     
           }                                       
          
                                                  // Prevent sensor 4 from triggering again
          trigger4 = 1;
        }
                                                  // Reset both sensors
          trigger3 = 0;
          trigger4 = 0;
      }
////////////////////////////////////////end of void loop //////////////////////////////////////////
}