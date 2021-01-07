
#include <Adafruit_NeoPixel.h>

#define PIN        6
#define DELAYVAL 500
#define NUMPIXELS 10


const byte buffSize = 40;
char inputBuffer[buffSize];
const char startMarker = '<';
const char endMarker = '>';
byte bytesRecvd = 0;
boolean readInProgress = false;
boolean newDataFromPC = false;

char messageFromPC[buffSize] = {0};
int red = 0;
int green = 0;
int blue = 0;
int concernValue = 0;
unsigned long curMillis;

unsigned long prevReplyToPCmillis = 0;
unsigned long replyToPCinterval = 1000;
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//=============

void setup() {
  Serial.begin(9600);
  pixels.begin();
    // flash LEDs so we know we are alive
  for (byte n = 0; n < NUMPIXELS; n++) {
     pixels.setPixelColor(n, pixels.Color(0, 100, 100));
     pixels.show();
     delay(DELAYVAL);
  }
  delay(500); // delay() is OK in setup as it only happens once
  
  for (byte n = 0; n < NUMPIXELS; n++) {
     pixels.setPixelColor(n, pixels.Color(0, 0, 100));
     pixels.show();
     delay(DELAYVAL);
  }
  
  pixels.clear();
    // tell the PC we are ready
  Serial.println("<Arduino is ready>");
}

//=============

void loop() {
  curMillis = millis();
  getDataFromPC();
  updateNeoPixelColors();
  replyToPC();
//  lightUp();

}

//=============

void getDataFromPC() {

    // receive data from PC and save it into inputBuffer
    
  if(Serial.available() > 0) {

    char x = Serial.read();

      // the order of these IF clauses is significant
      
    if (x == endMarker) {
      readInProgress = false;
      newDataFromPC = true;
      inputBuffer[bytesRecvd] = 0;
      parseData();
    }
    
    if(readInProgress) {
      inputBuffer[bytesRecvd] = x;
      bytesRecvd ++;
      if (bytesRecvd == buffSize) {
        bytesRecvd = buffSize - 1;
      }
    }

    if (x == startMarker) { 
      bytesRecvd = 0; 
      readInProgress = true;
    }
  }
}

//=============
 
void parseData() {

    // split the data into its parts
    
  char * strtokIndx; // this is used by strtok() as an index
  
  strtokIndx = strtok(inputBuffer,",");      // get the first part - the string
  strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC
  
  // strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  // red = atoi(strtokIndx);     // convert this part to an integer
  
  // strtokIndx = strtok(NULL, ","); 
  // green = atoi(strtokIndx);     // convert this part to a integer

  // strtokIndx = strtok(NULL, ",");
  // blue = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  concernValue = atoi(strtokIndx); // convert this part to a integer

}

//=============

void replyToPC() {

  if (newDataFromPC) {
    newDataFromPC = false;
    Serial.print("<Msg ");
    Serial.print(messageFromPC);
    Serial.print(" Red ");
    Serial.print(red);
    Serial.print(" Green ");
    Serial.print(green);
    Serial.print(" Green ");
    Serial.print(blue);
    Serial.print(" Blue ");

    Serial.print(" Time ");
    Serial.print(curMillis >> 9); // divide by 512 is approx = half-seconds
    Serial.println(">");
  }
}

//============

void updateNeoPixelColors() {

   // this illustrates using different inputs to call different functions
  if (strcmp(messageFromPC, "update") == 0) {
     updateNeo();
  }
  
  
}

//=============

void updateNeo() {

  for (byte n = 0; n < NUMPIXELS; n++) {
     pixels.setPixelColor(n, pixels.Color(red, green, blue));
     pixels.show();
     delay(DELAYVAL);
  }
}

//=============

//void

//=============


//=============

