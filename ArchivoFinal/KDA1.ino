#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>

#define MINPRESSURE 100
#define MAXPRESSURE 1000
#define LANDSCAPE 3

#define TOUCH_ORIENTATION  LANDSCAPE

#define LOWFLASH (defined(__AVR_ATmega328P__) && defined(MCUFRIEND_KBV_H_))

#include "bitmap_RGB.h"
#include <pt.h>
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY    0x8410
#define ORANGE  0xE880
const int XP=6,XM=A2,YP=A1,YM=7; //240x320 ID=0x5408
const int TS_LEFT=143,TS_RT=930,TS_TOP=918,TS_BOT=143;
uint8_t perder=0;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
#define SD_CS 10    // Set the chip select line to whatever you use

Adafruit_GFX_Button on_btn, off_btn;
int valor,b2, valor2, b4;
int varcielo,cielox, cieloy;
int pixel_x, pixel_y;     //Touch_getXY() updates global vars
bool Touch_getXY(void)
{
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   //because TFT control pins
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        pixel_x = map(p.y, 918, 143, 0, 320); //.kbv makes sense to me
        pixel_y = map(p.x, 930, 143, 0, 240);
    }
    return pressed;
}

struct pt caminar;
struct pt caminar2;
struct pt bloque1;
struct pt bloque2p;

void setup()
{
  Serial.begin(9600);
  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(3);
  
  inicio();
  cielo();
  on_btn.initButton(&tft,  50, 102, 50, 40, BLACK, GREY, WHITE, "P1", 2);
  off_btn.initButton(&tft, 50, 180, 50, 40, BLACK, GREY, WHITE, "P2", 2);
  on_btn.drawButton(false);
  off_btn.drawButton(false);
  static int S1;
  S1 = 0;
  perder=0;



  PT_INIT(&caminar);
  PT_INIT(&caminar2);
  PT_INIT(&bloque1);
  PT_INIT(&bloque2p);

  //    invertmap(betty_1_bmp + 62, 8960);
  //    while (1);
}


void loop(void)
{
  static int x = 320;
  static int saltarP1 = 0;
  static int saltarP2 = 0;
  static uint8_t   j = 90;
  static uint8_t j2 = 165;

  static uint8_t sube = 1;
  static uint8_t sube2 = 1;
   bool down = Touch_getXY();

//Botones Touch----------------------------------------------------------------------------------------------
    on_btn.press(down && on_btn.contains(pixel_x, pixel_y));
    off_btn.press(down && off_btn.contains(pixel_x, pixel_y));
    if (on_btn.justReleased())
        on_btn.drawButton();
    if (off_btn.justReleased())
        off_btn.drawButton();
    if (on_btn.justPressed()) {
        on_btn.drawButton(true);
        saltarP1=1;
        switch (perder){
          case 0:
          break;
          case 1:
          perder=0;
          tft.fillRect(110, 40, 130, 25, BLACK);
          break;
        }
    }
    if (off_btn.justPressed()) {
        off_btn.drawButton(true);
        saltarP2=1;
        switch (perder){
          case 0:
          break;
          case 1:
          perder=0;
          tft.fillRect(110, 40, 130, 25, BLACK);
          break;
        }
    }

//Salto P1----------------------------------------------------------------------------------------------------
   if (j > 45 &&saltarP1==1&&sube==1) {
      j = j - 5;
      tft.drawRGBBitmap(150, j, PL4A, 26, 32);
      tft.fillRect(150, j + 31, 26, 6, BLACK);
    }
   else if (j == 45 &&saltarP1==1&&sube==1) {
      sube = 0;
    }
    else if (j < 90 &&saltarP1==1&&sube==0) {
      j = j + 5;
      tft.drawRGBBitmap(150, j, PL4A, 26, 32);
      tft.fillRect(150, j - 5, 26, 5, BLACK);
    }
    else if (j == 90 &&saltarP1==1&&sube==0) {
      sube = 1;
      saltarP1=0;
    }
//Caminar P1--------------------------------------------------------------------------------------------------
  if (saltarP1==0&&perder==0){
  caminarP1(&caminar);
  }

//Salto P2----------------------------------------------------------------------------------------------------
   if (j2 > 125 &&saltarP2==1&&sube2==1) {
      j2 = j2 - 5;
      tft.drawRGBBitmap(150, j2, P4LH4, 25, 34);
      tft.fillRect(150, j2 + 33, 26, 7, BLACK);
    }
   else if (j2 == 125 &&saltarP2==1&&sube2==1) {
      sube2 = 0;
    }
    else if (j2 < 165 &&saltarP2==1&&sube2==0) {
      j2 = j2 + 5;
      tft.drawRGBBitmap(150, j2, P4LH4, 25, 34);
      tft.fillRect(150, j2 - 5, 26, 5, BLACK);
    }
    else if (j2 == 165 &&saltarP2==1&&sube2==0) {
      sube2 = 1;
      saltarP2=0;
    }
//Caminar P2------------------------------------------------------------------------------------------------
  if(saltarP2==0&&perder==0){
  caminarP2(&caminar2);
  }
//Condicion de ganar o perder-------------------------------------------------------------------------------

  if(saltarP1==0 && valor==1&&b2==148){
      tft.setCursor(110, 40); //setCursor(int16_t x, int16_t y)
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      tft.println("Gana P2");
      perder=1;
  }
  if(saltarP2==0 && valor2==1&&b4==148){
      tft.setCursor(110, 40); //setCursor(int16_t x, int16_t y)
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      tft.println("Gana P1");
      perder=1;
  }
//Generar Bloques de caida aleatorios-----------------------------------------------------------------------
if (perder ==0){
  bloqueNormal(&bloque1);
  bloque2(&bloque2p);
}

  

}
void bloqueNormal(struct pt *pt) {
  PT_BEGIN(pt);
  static int Probs1[10] = {1, 1, 1, 1, 0, 0, 0, 0, 0, 0};
  static long t = 0;
  static int x;
  static int generate;
  x = 320;
  do {
    if (x >= -70 && Probs1[generate] == 1) {
      tft.fillRect(x, 122, 35, 1, WHITE);
      tft.fillRect(x, 123, 35, 2, CYAN);
      x = x - 4; 
      valor=0;   
    }
    else if (x >= -70 && Probs1[generate] == 0) {
      tft.fillRect(x, 122, 45, 4, BLACK);
      tft.fillRect(x + 45, 122, 35, 1, WHITE);
      tft.fillRect(x + 45, 123, 35, 2, CYAN);
      x = x - 4;
      valor=1;
      b2=x;
    }
    else {
      x = 320;
      generate = random(0, 11);
    }
   
    t = millis();
    PT_WAIT_WHILE(pt, (millis() - t) <20);
    Serial.println(valor);
    Serial.println(b2);
  } while (1);
  PT_END(pt);
}

void bloque2(struct pt *pt) {
  PT_BEGIN(pt);
  static int Probs2[10] = {1, 1, 1, 1, 1, 1, 1, 0, 0, 0};
  static long t = 0;
  static int x = 340;
  static int generate2;
  do {
    if (x >= -90 && Probs2[generate2] == 1) {
      tft.fillRect(x, 200, 35, 1, WHITE);
      tft.fillRect(x, 201, 35, 2, BLUE);
      x = x - 4;
      valor2=0;
    }
    else if (x >= -90 && Probs2[generate2] == 0) {
      tft.fillRect(x, 200, 45, 3, BLACK);
      tft.fillRect(x + 45, 200, 35, 1, WHITE);
      tft.fillRect(x + 45, 201, 35, 2, BLUE);
      x = x - 4;
      b4=x;
      valor2=1;
    }
    else {
      x = 340;
      generate2 = random(0, 11);
    }
    t = millis();
    PT_WAIT_WHILE(pt, (millis() - t) < 20);
  } while (1);
  PT_END(pt);
}

void caminarP1(struct pt *pt) {
  PT_BEGIN(pt);
  static long t = 0;
  static uint8_t   j = 90;
  static uint8_t sube = 1;
  static int S1 = 0;
  static int n = 0;
  do {
      if (n == 0) {
        tft.drawRGBBitmap(150, j, PL1A, 26, 32);
        t = millis();
        PT_WAIT_WHILE(pt, ((millis() - t) < 50));
        n = 2;
      }
      if ( n == 2) {
        tft.drawRGBBitmap(150, j, PL3A, 26, 32);
        t = millis();
        PT_WAIT_WHILE(pt, ((millis() - t) < 50));
        n = 0;
      }
    
    //Saltar------------------------------------

    }while (1);
  PT_END(pt);
}

void caminarP2(struct pt *pt) {
  PT_BEGIN(pt);
  static int boton2 = 0;
  static uint8_t sube2 = 1;
  static int S2 = 0;
  static long t = 0;
  uint8_t j2;
  j2 = 165;
  while (true) {
    //Caminar-----------------------------------
    if (boton2 == 0 && S2 == 0) {
      tft.drawRGBBitmap(150, j2, PL1H, 25, 34);
      t = millis();
      PT_WAIT_WHILE(pt, (millis() - t) < 50);
      tft.drawRGBBitmap(150, j2, PL3H, 25, 35);
      t = millis();
      PT_WAIT_WHILE(pt, (millis() - t) < 50);
    }
  }
  PT_END(pt);
}

void inicio() {
  tft.fillScreen(BLACK);
  tft.setCursor(60, 1); //setCursor(int16_t x, int16_t y)
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.println("P-1    vs");

  tft.setCursor(200, 1); //setCursor(int16_t x, int16_t y)
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.println("  P-2");


}


void cielo(){
  varcielo = 0;
  while (varcielo < 50) {
  cielox=random(0, 320);
  cieloy=random(0, 240);
  varcielo++;
  tft.setCursor(cielox, cieloy); //setCursor(int16_t x, int16_t y)
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.println(".");
}

  
  delay(1000);
}
