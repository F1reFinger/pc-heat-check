#include <Wire.h> 
#include <string.h>
//#include <SPI.h>
#include <String.h>
#include <U8glib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#include "SSD1306Wire.h" 

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32


#define OLED_RESET -1  // GPIO0
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int xcenter=SCREEN_WIDTH/2;                             // center of x-axis

int ycenter= 62/2+10;                          // center of y-axis

int arc=64/2;                             

int angle=0;                                   

char* label[] = {"CPU+GPU","COOLANT","INTAKE", "VOLT"};    // some custom gauge labels

int labelXpos[] = {45, 45, 49, 45};                     // predefined x-position of a gauge label

int w, m,a=10;

int brabo = 0;
int select = 0;
int pin2 = 2;

u8g_uint_t xx=0;

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);
float dois = 0;
float recieved;

void ISR1(){
  if(select == 2){
    select = 0;
    }
    else{
      select++;
    }
}

void setup() {
  pinMode(pin2, INPUT);
  attachInterrupt(digitalPinToInterrupt(pin2), ISR1, FALLING);
  //Wire.begin(21, 22);
  Serial.begin(9600);
  u8g.setFont(u8g_font_chikita);
  u8g.setColorIndex(1);                         // Instructs the display to draw with a pixel on.

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);                     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);                       // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);                       // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
}
// ------------------------------------------------- void gauge() ------------------------------------------

void gauge(uint8_t angle) {
  // draw border of the gauge
  u8g.drawCircle(xcenter,ycenter,arc+6, U8G_DRAW_UPPER_RIGHT);
  u8g.drawCircle(xcenter,ycenter,arc+4, U8G_DRAW_UPPER_RIGHT);
  u8g.drawCircle(xcenter,ycenter,arc+6, U8G_DRAW_UPPER_LEFT);
  u8g.drawCircle(xcenter,ycenter,arc+4, U8G_DRAW_UPPER_LEFT);
  // draw the needle
  float x1=sin(2*angle*2*3.14/360);              // needle position
  float y1=cos(2*angle*2*3.14/360); 
  u8g.drawLine(xcenter, ycenter, xcenter+arc*x1, ycenter-arc*y1);
  u8g.drawDisc(xcenter, ycenter, 5, U8G_DRAW_UPPER_LEFT);
  u8g.drawDisc(xcenter, ycenter, 5, U8G_DRAW_UPPER_RIGHT);
  u8g.setFont(u8g_font_chikita);
  // show scale labels
  u8g.drawStr( 20, 42, "0");                   
  u8g.drawStr( 25, 18, "25");
  u8g.drawStr( 60, 14, "50");
  u8g.drawStr( 95, 18, "75");
  u8g.drawStr( 105, 42, "100");
  // show gauge label
  u8g.setPrintPos(labelXpos[0],32);           
  u8g.print(label[0]);
  // show digital value and align its position
  u8g.setFont(u8g_font_profont22);             
  u8g.setPrintPos(54,60);
  if (w<10){                                    // leading 0 when value less than 10
    u8g.print("0");
  }
  if (w>99) {                                   // position at 100%
    u8g.setPrintPos(47,60);
  }
  u8g.print(w);
}

void loop() {  
    while (Serial.available() != 0){   
       if(select == 1){
         recieved = Serial.read();
         brabo += recieved;
         recieved = Serial.read();
         brabo += recieved;
         brabo = brabo*10;
         w = map(brabo,0,1023,0,100);  // map it between 0 and 100
         m = map(brabo,0,1023,0,90);   // map needle movement
         brabo = 0;
       }
       else if(select == 2){
         recieved = Serial.read();
         dois += recieved;
         recieved = Serial.read();
         dois += recieved;
         dois = dois*100;
         w = map(dois,0,1023,0,100);  // map it between 0 and 100
         m = map(dois,0,1023,0,90);   // map needle movement
         dois = 0;
       }
        
    }
  // show needle and dial
  xx = m;                                      // 135 = zero position, 180 = just before middle, 0 = middle, 45 = max
  if (xx<45){                                   // positie correctie
    xx=xx+135;
  }
  else {
    xx=xx-45;
  }
  {
    u8g.firstPage(); 
    do {             
      gauge(xx);
    }
    while( u8g.nextPage() );
  }
}
