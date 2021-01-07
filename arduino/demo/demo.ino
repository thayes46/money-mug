// Simple button cycling between different modes

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Digital IO pin connected to the button. This will be driven with a
// pull-up resistor so the switch pulls the pin to ground momentarily.
// On a high -> low transition the button press logic will execute.
#define BUTTON_PIN   6

#define PIXEL_PIN    2

#define PIXEL_COUNT 10  // Number of NeoPixels

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
int     mode         = 0;    // Currently-active animation mode, 0-9
short   colorScheme  = 0;  // How the user selects the colors of the lEDs
int     concern      = 0;

/*
 * 0 = 0-2 RED  3-6 YELLOW 7-9 GREEN
 * 1 = 0-2 PINK 3-6 PURPLE 7-9 BLUE
 * 2 = 0-2 RED 3 ORANGE 4-5 YELLOW 6-7 GREEN 8 BLUE 9 PURPLE
 */

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(96000);
  strip.begin(); // Initialize NeoPixel strip object (REQUIRED)
  strip.setBrightness(50);
  showLevel(0, 2);
  strip.show();  // Initialize all pixels to 'off'
}

void loop() {
  boolean newState = digitalRead(BUTTON_PIN);
  // Check if button was pressed
  if((newState == LOW) && (oldState == HIGH)) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newState = digitalRead(BUTTON_PIN);
    if(newState == LOW) {      // Yes, still low
      if(++mode > 4) mode = 0; // Advance to next mode, wrap around after #8
      switch(mode) {           // Start the new animation...
        case 0:
          //Listen to serial and respond accordingly
          colorScheme = 0; // Default colors
          while (digitalRead(BUTTON_PIN) == newState) {
            //fetch concern from serial

            //temp just to show something:
              showLevel(50, 2);

            
//            showLevel(concern, colorScheme);
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
            
            //fetch data from serial
            
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
      }
    }
  }
  oldState = newState;
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
