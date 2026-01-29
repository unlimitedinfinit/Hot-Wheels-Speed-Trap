/*
 * PROJECT: Arduino Hot Wheels Speed Trap (Version 2.0)
 * AUTHOR: Joshua Abrams
 * ORIGINAL DATE: 03/25/2019
 * UPDATED: January 2026
 * * DESCRIPTION:
 * This system uses 4 IR Break Beam sensors to calculate the time and speed of toy cars 
 * across two independent lanes (Left/Track 1 and Right/Track 2). Results are displayed 
 * on two separate MAX7219 LED Matrix displays.
 * * HARDWARE CONNECTIONS:
 * ---------------------
 * SENSORS (Digital Inputs):
 * - Lane 1 Start: Pin 4
 * - Lane 1 End:   Pin 2
 * - Lane 2 Start: Pin 3
 * - Lane 2 End:   Pin 5
 * * DISPLAYS (MAX7219):
 * - Left Display (Lane 1): DIN=12, CLK=11, CS=10
 * - Right Display (Lane 2): DIN=9, CLK=8, CS=7
 * * LED INDICATORS (Winner/Status):
 * - Lane 1 LEDs: Pins 43 & 2
 * - Lane 2 LEDs: Pins 46 & 49
 * * INSTRUCTIONS:
 * 1. Install the "LedControl" library in Arduino IDE (Sketch > Include Library > Manage Libraries).
 * 2. Connect hardware as listed above.
 * 3. Upload this sketch.
 * 4. Ensure sensors are aligned (IR beams connected).
 */

#include "LedControl.h" // Must add this library for MAX7219

// ==========================================
//              PIN DEFINITIONS
// ==========================================

// Display 1 (Left Side / Track 1)
#define DISP1_DIN 12
#define DISP1_CLK 11
#define DISP1_CS  10

// Display 2 (Right Side / Track 2)
#define DISP2_DIN 9
#define DISP2_CLK 8
#define DISP2_CS  7

// Sensors
#define SENSORPIN1 4 // Lane 1 Start
#define SENSORPIN2 2 // Lane 1 End
#define SENSORPIN3 3 // Lane 2 Start
#define SENSORPIN4 5 // Lane 2 End

// Status LEDs
#define LED_ONE_1  43
#define LED_ONE_2  2
#define LED_TWO_1  46
#define LED_TWO_2  49

// ==========================================
//              GLOBAL VARIABLES
// ==========================================

// Initialize Display Objects
// LedControl(DataIn, Clk, CS, NumberOfDevices)
LedControl lc1 = LedControl(DISP1_DIN, DISP1_CLK, DISP1_CS, 1);
LedControl lc2 = LedControl(DISP2_DIN, DISP2_CLK, DISP2_CS, 1);

// Variables for display output math
unsigned int num = 0;
unsigned int one, ten, hundred;
unsigned int num2 = 0;
unsigned int one2, ten2, hundred2;

int sensorState1 = 0;
int sensorState2 = 0;
int sensorState3 = 0;
int sensorState4 = 0;

// Timing Variables (Lane 1)
unsigned long timeFirst1;  // millis for sensor 1 (Start)
unsigned long timeSecond1; // millis for sensor 2 (End)

// Timing Variables (Lane 2)
unsigned long timeFirst2;  // millis for sensor 3 (Start)
unsigned long timeSecond2; // millis for sensor 4 (End)

unsigned long currentTime; // for turning off the LEDs
int duration = 5000;       // 5 seconds in milliseconds to keep lights on

float difference1;        // raw time difference
float difference2;
float Secondsdifference1; // final seconds (Track 2)
float Secondsdifference2; // final seconds (Track 1)

// State Machine Triggers
int trigger1 = 0; // Sensor 1 triggered?
int trigger2 = 0; // Sensor 2 triggered?
int trigger3 = 0; // Sensor 3 triggered?
int trigger4 = 0; // Sensor 4 triggered?
int lightTrigger1 = 0;
int lightTrigger2 = 0;

// ==========================================
//              SETUP
// ==========================================
void setup() {
  // Configure LED pins
  pinMode(LED_ONE_1, OUTPUT);
  pinMode(LED_ONE_2, OUTPUT);
  pinMode(LED_TWO_1, OUTPUT);
  pinMode(LED_TWO_2, OUTPUT);

  // Setup the Max7219 display 1 (Left)
  lc1.shutdown(0, false); // Wake up display
  lc1.setIntensity(0, 8); // Set brightness (0-15)
  lc1.clearDisplay(0);    // Clear screen

  // Setup the Max7219 display 2 (Right)
  lc2.shutdown(0, false);
  lc2.setIntensity(0, 8);
  lc2.clearDisplay(0);

  Serial.begin(9600); // Start serial monitor for debugging

  // Initialize sensors with Internal Pullups
  // This ensures inputs aren't floating when the beam isn't broken
  pinMode(SENSORPIN1, INPUT);
  digitalWrite(SENSORPIN1, HIGH); 

  pinMode(SENSORPIN2, INPUT);
  digitalWrite(SENSORPIN2, HIGH);

  pinMode(SENSORPIN3, INPUT);
  digitalWrite(SENSORPIN3, HIGH);

  pinMode(SENSORPIN4, INPUT);
  digitalWrite(SENSORPIN4, HIGH);
}

// ==========================================
//           HELPER FUNCTIONS
// ==========================================

/* * Function: displayRaceTime
 * -------------------------
 * Takes a raw time in seconds and a display object (lc1 or lc2).
 * Breaks the float (e.g., 2.35) into digits and pushes them to the screen.
 * NOTE: This works best for times < 9.99 seconds.
 */
void displayRaceTime(LedControl &lc, float timeSeconds) {
  unsigned int calcNum = timeSeconds * 100;
  
  unsigned int digitOne = calcNum % 10;
  unsigned int digitTen = (calcNum / 10) % 10;
  unsigned int digitHundred = (calcNum / 100) % 10;

  // lc.setDigit(addr, digit_position, value, decimal_point_boolean);
  lc.setDigit(0, 3, (byte)digitHundred, 1); // 1 = true for decimal point
  lc.setDigit(0, 2, (byte)digitTen, 0);
  lc.setDigit(0, 1, (byte)digitOne, 0);
}

// Function to determine speed for Track 1 (Left Side)
void calculateSpeedTrack1() {
  // Subtract end time from start time
  difference2 = (timeSecond2 - timeFirst2);
  Secondsdifference2 = difference2 / 1000;
  
  Serial.print("Track 1 (LEFT) Time: ");
  Serial.println(Secondsdifference2);

  // Use our new helper function to display
  displayRaceTime(lc1, Secondsdifference2);
}

// Function to determine speed for Track 2 (Right Side)
void calculateSpeedTrack2() {
  // Subtract end time from start time
  difference1 = (timeSecond1 - timeFirst1);
  Secondsdifference1 = difference1 / 1000;

  Serial.print("Track 2 (RIGHT) Time: ");
  Serial.println(Secondsdifference1);

  // Use our new helper function to display
  displayRaceTime(lc2, Secondsdifference1);
}

// ==========================================
//              MAIN LOOP
// ==========================================
void loop() {
  currentTime = millis();

  // --- TIMER RESET LOGIC ---
  // Turn off the light if it's been on for over 5 seconds and reset the triggers
  
  // Track 2 Lights Reset
  if (currentTime - timeSecond1 >= duration) {
    digitalWrite(LED_ONE_1, LOW);
    digitalWrite(LED_ONE_2, LOW);
    lightTrigger1 = 0;
    // Note: lightTrigger2 was reset here in original, keeping strict logic
    // but typically you'd separate them. Kept original logic flow.
    lightTrigger2 = 0; 
  }

  // Track 1 Lights Reset
  if (currentTime - timeSecond2 >= duration) {
    digitalWrite(LED_TWO_1, LOW);
    digitalWrite(LED_TWO_2, LOW);
    lightTrigger1 = 0;
    lightTrigger2 = 0;
  }

  // --- STATIC DISPLAY MARKERS ---
  lc1.setChar(0, 7, 'L', false); // "L" for Lane/Left
  lc1.setChar(0, 6, '1', false); // "1"
  lc2.setChar(0, 7, 'L', false);
  lc2.setChar(0, 6, '2', false); // "2"

  // ==========================================
  //        TRACK 1 LOGIC (Sensors 1 & 4)
  //        (Note: Original code mapped Pins 1/4 to this logic block)
  // ==========================================
  
  // Read Start Sensor (Pin 4)
  sensorState1 = digitalRead(SENSORPIN1);
  
  if (sensorState1 == LOW) {
    Serial.println("IR Beam 1 Broken (Start)");
    
    // Check if race has already started
    if (trigger1 == 0 && trigger2 == 0) {
      timeFirst1 = millis(); // Save Start Time
      trigger1 = 1;          // Lock start trigger
    }
  }

  // Read End Sensor (Pin 5)
  sensorState4 = digitalRead(SENSORPIN4);
  
  if (sensorState4 == LOW) {
    Serial.println("IR Beam 2 Broken (Finish)");
    
    // Ensure start was triggered, but end hasn't been yet
    if (trigger2 == 0 && trigger1 == 1) {
      timeSecond1 = millis(); // Save Finish Time
      calculateSpeedTrack2(); // Calculate & Display
      
      // Turn on Finish Lights if not already on
      if (lightTrigger1 == 0 && lightTrigger2 == 0) {
        digitalWrite(LED_ONE_1, HIGH);
        digitalWrite(LED_ONE_2, HIGH);
        lightTrigger1 = 1;
      }
      trigger2 = 1; // Lock finish trigger
    }
    // Reset triggers for next race
    trigger1 = 0;
    trigger2 = 0;
  }

  // ==========================================
  //        TRACK 2 LOGIC (Sensors 3 & 2)
  // ==========================================

  // Read Start Sensor (Pin 3)
  sensorState3 = digitalRead(SENSORPIN3);
  
  if (sensorState3 == LOW) {
    Serial.println("IR Beam 3 Broken (Start)");
    
    if (trigger3 == 0 && trigger4 == 0) {
      timeFirst2 = millis(); // Save Start Time
      trigger3 = 1;          // Lock start trigger
    }
  }

  // Read End Sensor (Pin 2)
  sensorState2 = digitalRead(SENSORPIN2);
  
  if (sensorState2 == LOW) {
    Serial.println("IR Beam 4 Broken (Finish)");
    
    if (trigger4 == 0 && trigger3 == 1) {
      timeSecond2 = millis(); // Save Finish Time
      calculateSpeedTrack1(); // Calculate & Display

      // Turn on Finish Lights
      if (lightTrigger1 == 0 && lightTrigger2 == 0) {
        digitalWrite(LED_TWO_1, HIGH);
        digitalWrite(LED_TWO_2, HIGH);
        lightTrigger2 = 1;
      }
      trigger4 = 1; // Lock finish trigger
    }
    // Reset triggers for next race
    trigger3 = 0;
    trigger4 = 0;
  }
}