// Simple button cycling between different modes

#include <Adafruit_NeoPixel.h>


// Digital IO pin connected to the button. This will be driven with a
// pull-up resistor so the switch pulls the pin to ground momentarily.
// On a high -> low transition the button press logic will execute.
#define BUTTON_PIN   6

#define PIXEL_PIN    2

#define PIXEL_COUNT 10  // Number of NeoPixels

//Bounds for each level of concern, 0-33 = no concern, 34 - 66 warning, 67-100 beeg concern
#define lowerQuartile 33
#define upperQuartile 66

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

boolean oldState = LOW;
int     mode     = 0;    // Currently-active animation mode, 0-9


void setup() {
  pinMode(BUTTON_PIN,INPUT);
  Serial.begin(96000);
  strip.begin(); // Initialize NeoPixel strip object (REQUIRED)
  strip.setBrightness(70);
  strip.show();  // Initialize all pixels to 'off'
}

void loop() {
  // Get current button state.
  boolean newState = digitalRead(BUTTON_PIN);

  // Check if state changed from high to low (button press).
  if((newState == HIGH) && (oldState == LOW)) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newState = digitalRead(BUTTON_PIN);
    if(newState == LOW) {      // Yes, still low
      if(++mode > 8) mode = 0; // Advance to next mode, wrap around after #8
      switch(mode) {           // Start the new animation...
        case 0:
          break;
        case 1:
          break;
      }
    }
  }
  oldState = newState;
}

//Method to display current level whenever shook
void showLevel(int concern_level) {
    if (
}
