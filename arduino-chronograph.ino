//libraries for display
#include <Arduino.h>   
#include <U8g2lib.h>
#include <SoftwareSerial.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define IR_REC_ONE_PIN 0      //pins for IR Gate
#define IR_REC_TWO_PIN 1      //pins for IR gate
#define GATE_DISPLACEMENT 2   //distance between gate, in inches
#define IR_GATE_TRIP_VAL 90   //value at which the IR gate is considered "blocked", or "tripped"

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE);   //display object

double firstTripTime, secondTripTime, chronoReading = 0, i = 0;    //keep track of timing between IR gate breakage
boolean hasFirstTripped = false;    //flag to ensure proper timing
char chronoValsToPrint[4];    //what's displayedo in the display

void setup () { 
    Serial.begin(9600);
    Serial.println("hello world!");
       
  u8g2.begin();   //begin necessary functions for the dispaly

}

void loop () {
   i++;
   
//  chrono();   //check and do chrono
//  display();
}

//display text to screen
void display () {
    Serial.println("in display!");

    sprintf(chronoValsToPrint, "%04d", i);    //double to char*   
    u8g2.setFont(u8g2_font_ncenB10_tr);   //select font
    
    u8g2.clearBuffer(); 
    u8g2.drawStr(0,10,chronoValsToPrint);    // write something to the internal memory
    u8g2.sendBuffer(); 
    delay(1000);
//    u8g2.firstPage();   //keep track of pages
//    do {
//      u8g2.drawUTF8(0, 24, chronoValsToPrint);    //draw text at certain coordiantes
//    } while ( u8g2.nextPage() );    //keep track of pages
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
  double timeElapsed = (secondTripTime - firstTripTime) * 1000000;
  double displacement = GATE_DISPLACEMENT / 12;

  chronoReading = displacement/timeElapsed;
}


