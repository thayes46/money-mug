// Simple button cycling between different modes

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#include <Wire.h>
#include "LIS3DHTR.h"
LIS3DHTR<TwoWire> LIS(I2C_MODE);//IIC
#define WIRE Wire

// Digital IO pin connected to the button. This will be driven with a
// pull-up resistor so the switch pulls the pin to ground momentarily.
// On a high -> low transition the button press logic will execute.
#define BUTTON_PIN   6

#define PIXEL_PIN    2

#define PIXEL_COUNT 10  // Number of NeoPixels

#define TOLERANCE 160

//Bounds for each level of concern, 0-33 = no concern, 34 - 66 warning, 67-100 beeg concern
#define lowerQuartile 30
#define upperQuartile 66

#define RED      128, 0, 0
#define YELLOW   128, 128, 0
#define GREEN    0, 128, 0
#define ORANGE   128, 64, 0
#define BLUE     0, 0, 128
#define PURPLE   50, 0, 76
#define PINK     80, 50, 80
#define BLACK    0, 0, 0


// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

boolean oldState     = HIGH;
int     mode         = -1;    // Currently-active animation mode, 0-9
short   colorScheme  = 0;  // How the user selects the colors of the lEDs
int     concern      = 0;

/*
 * 0 = 0-2 RED  3-6 YELLOW 7-9 GREEN
 * 1 = 0-2 PINK 3-6 PURPLE 7-9 BLUE
 * 2 = 0-2 RED 3 ORANGE 4-5 YELLOW 6-7 GREEN 8 BLUE 9 PURPLE
 */

//Variable setup for serial
const byte buffSize = 40;
char inputBuffer[buffSize];
const char startMarker = '<';
const char endMarker = '>';
byte bytesRecvd = 0;
bool readInProgress = false;
bool newDataFromPC = false;
char messageFromPC[buffSize] = {0};
unsigned long curMillis;
unsigned long prevReplyToPCmillis = 0;
unsigned long replyToPCinterval = 1000;

//Variable setup for Accelerometer Jazz
bool moving = false;
int  oldAcceleration = 0;
int  newAcceleration = 0;
char concernStr[6];


void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(115200);
  strip.begin(); // Initialize NeoPixel strip object (REQUIRED)
  strip.setBrightness(50);
  strip.show();  // Initialize all pixels to 'off'
  LIS.begin(WIRE); //IIC init
  delay(100);
  LIS.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);
  newAcceleration = readAcceleration();
//  Serial.println("<Arduino is ready>");
}

void loop() {
//   /*
  boolean newState = digitalRead(BUTTON_PIN);
  // Check if button was pressed
  if((newState == LOW) && (oldState == HIGH)) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newState = digitalRead(BUTTON_PIN);
    if(newState == LOW) {      // Yes, still low
      if(++mode > 5) mode = 0; // Advance to next mode, wrap around after #8
      switch(mode) {           // Start the new animation...
        case 0:
          clearPixels();
          //Listen to serial and respond accordingly
          colorScheme = 0; // Default colors
          while (digitalRead(BUTTON_PIN) == newState) {
            
            int index = 0;
            if (Serial.available()){
              do {
              char data = Serial.read();
//              Serial.print("<received data: "); Serial.print(data); Serial.println(">");
              concernStr[index] = data;
              index++;
              } while (Serial.available());
              concernStr[4] = '\0';            
            }
            //wait for pick and place
            newAcceleration = readAcceleration();
            if (newAcceleration > oldAcceleration + TOLERANCE) {
              while (readAcceleration() > oldAcceleration + TOLERANCE){};
              //fetch concern from serial
//              Serial.print("Latest data: "); Serial.print(concernStr);
              String concernString = "";
              for (int i = 1; i < 4; i++) {
                concernString.concat(concernStr[i]);
              }
//              Serial.println(concernString);
              concern = concernString.toInt();
//              Serial.print(concern);
              showLevel(concern, colorScheme); 
              delay(5000);
              clearPixels();
            }            
          }
          break;
        case 1:
          //Demonstrate the different levels with default colors
          colorScheme = 0;
          while (digitalRead(BUTTON_PIN) == newState) {
            concern += 5;
            if (concern > 100) concern = 0;
            delay(100);
            showLevel(concern, colorScheme);
          }          
          break;
        case 2:
          //Demonstrate alternative color scheme cycling levels
          colorScheme = 1;
          while (digitalRead(BUTTON_PIN) == newState) {
            concern += 5;
            if (concern > 100) concern = 0;
            delay(100);
            showLevel(concern, colorScheme);
          }                   
          break;
        case 3:
          //Demonstrate Rainbow color scheme
          colorScheme = 2;
          while (digitalRead(BUTTON_PIN) == newState) {
            //incriment concern cyclically
            concern += 5;
            if (concern > 100) concern = 0;
            delay(100);
            showLevel(concern, colorScheme);
          }                   
          break;
        case 4:
          //demo with per-pixel granularity
          colorScheme = 2;
          while (digitalRead(BUTTON_PIN) == newState) {
            concern += 5;
            if (concern > 100) concern = 0;
            delay(100);          
            int max_led = (int) concern / 10;
            max_led = 10 - max_led; //invert since 0 is good
            int i = 0;
            for (; i < max_led; i++) {
              setLight(i, colorScheme);
            }
            for (;i < strip.numPixels(); i++) {
              strip.setPixelColor(i,BLACK);
            }
            strip.show();
          }
          break;
        case 5:
          //demo with per-pixel granularity
          colorScheme = 2;
          clearPixels();
          while (digitalRead(BUTTON_PIN) == newState) {
            
            newAcceleration = readAcceleration();
            if (newAcceleration > oldAcceleration + TOLERANCE) {
              while (readAcceleration() > oldAcceleration + TOLERANCE){};
              //fetch concern from serial
              listenSerial();            
              int max_led = (int) concern / 10;
              max_led = 10 - max_led; //invert since 0 is good
              int i = 0;
              for (; i < max_led; i++) {
                setLight(i, colorScheme);
              }
              for (;i < strip.numPixels(); i++) {
                strip.setPixelColor(i,BLACK);
              }
              strip.show();
              delay(5000);
              clearPixels();
            }
          }
          break;
      }
    }
  }
  oldState = newState;
//  */
}

//Method to display current level whenever shook
void showLevel(int concern_level, short pattern) {
  int i = 0;
  if (concern_level < lowerQuartile) {
    //all is well
    for (; i < 10; i++) {
      setLight(i, pattern);
    }
  } else if (concern_level < upperQuartile) {
    //mild concern
    for (; i < 7; i++) {
      setLight(i, pattern);
    }
    for (; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, BLACK);
    }
  } else {
    // uh oh
    for (; i < 3; i++) {
      setLight(i, pattern);
    }
    for (; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, BLACK);
    }
  }
  strip.show();
}

//set light according to color scheme
void setLight(int index, short pattern) {
  switch(pattern) {
    case 0:
      //red yellow green
      switch (index) {
        case 0:
        case 1:
        case 2:
          strip.setPixelColor(index, RED);
          break;
        case 3:
        case 4:
        case 5:
        case 6:
          strip.setPixelColor(index, YELLOW);
          break;
        case 7:
        case 8:
        case 9:
          strip.setPixelColor(index, GREEN);
          break;
      }
      break;
    case 1:
      //purple blue green
      switch (index) {
        case 0:
        case 1:
        case 2:
          strip.setPixelColor(index, BLUE);
          break;
        case 3:
        case 4:
        case 5:
        case 6:
          strip.setPixelColor(index, PURPLE);
          break;
        case 7:
        case 8:
        case 9:
          strip.setPixelColor(index, PINK);
          break;
      }
      break;
    case 2:
      //rainbow = 0-2 RED 3 ORANGE 4-5 YELLOW 6-7 GREEN 8 BLUE 9 PURPLE
      switch (index) {
        case 0:
        case 1:
        case 2:
          strip.setPixelColor(index, RED);
          break;
        case 3:
          strip.setPixelColor(index, ORANGE);
          break;
        case 4:
        case 5:
          strip.setPixelColor(index, YELLOW);
          break;
        case 6:
        case 7:
          strip.setPixelColor(index, GREEN);
          break;
        case 8:
          strip.setPixelColor(index, BLUE);
          break;
        case 9:
          strip.setPixelColor(index, PURPLE);
          break;
      }
      break;
  }
}

void listenSerial() {
//  char str[5];
  int index = 0;
  while (Serial.available()) {
    //should be {'x','0','1','2','\0'}
    char data = Serial.read();
    char str[2];
    str[0] = data;
    str[1] = '\0';
    Serial.print(str);
  }
//  str[4] = '\0';
  
}


////method to grab data from serial
//void getDataFromPC() {
//  // receive data from PC and save it into inputBuffer
//  while(Serial.available() > 0) {
//    char x = Serial.read();
//    Serial.print(x);
//      // the order of these IF clauses is significant
//    if (x == endMarker) {
//      readInProgress = false;
//      newDataFromPC = true;
//      inputBuffer[bytesRecvd] = 0;
//      parseData();
//    }    
//    if(readInProgress) {
//      inputBuffer[bytesRecvd] = x;
//      bytesRecvd ++;
//      if (bytesRecvd == buffSize) {
//        bytesRecvd = buffSize - 1;
//      }
//    }
//    if (x == startMarker) { 
//      bytesRecvd = 0; 
//      readInProgress = true;
//    }
//  }
//}
//
//
////Reading buffer
//void parseData() {
//    
//  char * strtokIndx; // this is used by strtok() as an index
//  
//  strtokIndx = strtok(inputBuffer,",");      // get the first part - the string
//  strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC
//
//  strtokIndx = strtok(NULL, ",");
//  concern = atoi(strtokIndx); // convert this part to a integer
//
//}

void clearPixels() {
  for (int i = 0; i < 10; i++) {
    strip.setPixelColor(i, BLACK);
  }
  strip.show();
}

int readAcceleration() {
  int xValue = (int)(LIS.getAccelerationX() *1023.0);
  int yValue = (int)(LIS.getAccelerationY() *1023.0);
  int zValue = (int)(LIS.getAccelerationZ() *1023.0);
  int norm = sqrt(
    yValue * yValue //y value is only one consistently under 100 for "stable" state
  );
  return norm;  
}
