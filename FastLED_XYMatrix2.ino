// 2018/8/29 modified from FastLED example.
// Simple Scroll LED Pattern Array

#include <FastLED.h>

#define LED_PIN  8
#define COLOR_ORDER GRB
#define CHIPSET     WS2812B

#define BRIGHTNESS 32  // max brightness
#define FRAMES_PER_SECOND  120

// Params for width and height
const uint8_t kMatrixWidth = 30;
const uint8_t kMatrixHeight = 1;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* const leds( leds_plus_safety_pixel + 1);

// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = true;
// Set 'kMatrixSerpentineLayout' to false if your pixels are 
// laid out all running the same way, like this:
//
//     0 >  1 >  2 >  3 >  4
//                         |
//     .----<----<----<----'
//     |
//     5 >  6 >  7 >  8 >  9
//                         |
//     .----<----<----<----'
//     |
//    10 > 11 > 12 > 13 > 14
//                         |
//     .----<----<----<----'
//     |
//    15 > 16 > 17 > 18 > 19
//
// Set 'kMatrixSerpentineLayout' to true if your pixels are 
// laid out back-and-forth, like this:
//
//     0 >  1 >  2 >  3 >  4
//                         |
//                         |
//     9 <  8 <  7 <  6 <  5
//     |
//     |
//    10 > 11 > 12 > 13 > 14
//                        |
//                        |
//    19 < 18 < 17 < 16 < 15
//
// Bonus vocabulary word: anything that goes one way 
// in one row, and then backwards in the next row, and so on
// is call "boustrophedon", meaning "as the ox plows."

uint16_t XYM( uint8_t x, uint8_t y,  uint8_t m)
{
  uint16_t i;
  
  if( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + ((x + m) % kMatrixWidth);
  }
  if( kMatrixSerpentineLayout == true) {
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - ((x + m) % kMatrixWidth);
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + ((x + m) % kMatrixWidth);
    }
  }
  return i;
}

uint16_t XYsafe( uint8_t x, uint8_t y)
{
  if( x >= kMatrixWidth) return -1;
  if( y >= kMatrixHeight) return -1;
  return XYM(x,y,0);
}

//PROGMEMはなしで。
const byte ledFrame[ kMatrixWidth * kMatrixHeight * 3] = {
0x80, 0x80, 0x80, 
0xFF, 0xFF, 0xFF, 
0x80, 0x80, 0x80, 
0xFF, 0xFF, 0xFF, 
0x80, 0x80, 0x80, 
0xFF, 0xFF, 0xFF, 
0x00, 0x00, 0x00, 
0xFF, 0x00, 0x00, 
0x00, 0x00, 0x00, 
0xFF, 0x00, 0x00, 
0x00, 0x00, 0x00, 
0xFF, 0x00, 0x00, 
0x00, 0x00, 0x00, 
0x00, 0x70, 0xC0, 
0x00, 0x00, 0x00, 
0x00, 0x70, 0xC0, 
0x00, 0x00, 0x00, 
0x00, 0x70, 0xC0, 
0x00, 0x00, 0x00, 
0x00, 0xB0, 0x50, 
0x00, 0x00, 0x00, 
0x00, 0xB0, 0x50, 
0x00, 0x00, 0x00, 
0x00, 0xB0, 0x50, 
0x00, 0x00, 0x00, 
0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 
0xFF, 0xFF, 0x00, 
0x00, 0x00, 0x00, 
0xFF, 0xFF, 0x00
};

void setup() {
  Serial.begin(115200);
  delay(3000); // 3 second delay for safe recovery
 
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness( BRIGHTNESS );
}

float mPos = 0;     // ずらした地点
float mSpeed = 0.1; // ずらし速度 0なら動かない。マイナスなら逆転

void loop()
{

  for( byte y = 0; y < kMatrixHeight; y++) {
    for( byte x = 0; x < kMatrixWidth; x++) {
       leds[ XYM(x, y, mPos)] = CRGB ( ledFrame [  ( (y * kMatrixWidth) + x ) * 3], 
           ledFrame [ ( ( (y * kMatrixWidth) + x ) * 3 ) + 1],
           ledFrame [  ( ( (y * kMatrixWidth) + x ) * 3 ) + 2] );
    }
  }

  mPos = mPos + mSpeed;
  if ( mPos >= kMatrixWidth) mPos = 0;

  FastLED.show();   // send the 'leds' array out to the actual LED strip
  FastLED.delay(1000/FRAMES_PER_SECOND);    // insert a delay to keep the framerate modest
}


