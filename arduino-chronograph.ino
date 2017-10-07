#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define IR_REC_ONE_PIN 0      //pins for IR Gate
#define IR_REC_TWO_PIN 1      //pins for IR gate
#define GATE_DISPLACEMENT 0.22916666666f   //distance between gate, in feet
#define IR_GATE_TRIP_VAL 60   //value at which the IR gate is considered "blocked", or "tripped"

//U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE);   //display object

Adafruit_SSD1306 display(4);

double firstTripTime, secondTripTime, chronoReading = 0;    //keep track of timing between IR gate breakage
boolean hasFirstTripped = false;    //flag to ensure proper timing
char chronoValsToPrint[4];    //what's displayedo in the display

void setup () { 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  displayVals();       //display vals
}

void loop () {
  chrono();   //check and do chrono
}

//display text to screen
void displayVals () {    
    display.clearDisplay();
    display.setTextSize(4);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print((String)chronoReading);
    display.display();
}

//chrono timing and trip checking
void chrono () {    
  if (hasFirstTripped == false && (map(analogRead(IR_REC_ONE_PIN), 0, 1024, 0, 100) > IR_GATE_TRIP_VAL) ) {   
    firstTripTime = micros();
    hasFirstTripped = true;
  }
  
  if (hasFirstTripped == true && (map(analogRead(IR_REC_TWO_PIN), 0, 1024, 0, 100) > IR_GATE_TRIP_VAL) ) {   
    secondTripTime = micros();
    hasFirstTripped = false;
    calculateChronoReadings();
  }
}

//chrono calculations based on time
double calculateChronoReadings () {
  chronoReading = (float)GATE_DISPLACEMENT/((secondTripTime - firstTripTime) / 1000000);
  displayVals();       //display vals
}


