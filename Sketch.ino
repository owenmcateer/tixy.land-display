#include <math.h>

// Matrix settings
const uint8_t kMatrixWidth = 15;
const uint8_t kMatrixHeight = 15;
#define LED_PIN     D4
#define COLOR_ORDER GRB
#define CHIPSET     WS2812B
#define BRIGHTNESS  64
const bool kMatrixSerpentineLayout = true;

// Matrix setup
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_ESP8266_DMA
#include <FastLED.h>

// FastLED check
#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001008)
#warning "Requires FastLED 3.1.8 or later; check github for latest code."
#endif

// Setup Matric display
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* const leds( leds_plus_safety_pixel + 1);
unsigned long previousMillis = 0;
int animation = 0;
int fps = 0;
int animationDelay = 12000;


/**
 * Setup
 */
void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.println('Hello');

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
  FastLED.clear();
}


/**
 * Main loop tick
 */
void loop() {
  unsigned long milli = millis();
  double t = milli / 1000.0;

  for( double x = 0; x < kMatrixWidth; x++) {
    for( double y = 0; y < kMatrixHeight; y++) {
      processFrame(t, x, y);
    }
  }
  FastLED.show();

  // FPS reporting
  fps++;
  unsigned long currentMillis = milli;
  if (currentMillis - previousMillis >= animationDelay) {
    animation++;
    previousMillis = currentMillis;
    Serial.print(fps / 3);
    Serial.println("fps");

    Serial.print("Animation: ");
    Serial.println(animation);
    fps = 0;
  }
}


/**
 * FastLED helper fucntion
 */
uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;

  if( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }

  if( kMatrixSerpentineLayout == true) {
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }

  return i;
}


/**
 * Process pixel value
 */
void processFrame(double t, double x, double y) {
  double i = (y * 16) + x;
  double frame = constrain(code(t, i, x, y), -1, 1) * 255;

  if (frame >= 0) {
    leds[XY(x, y)].setRGB(frame, frame, frame);
  }
  else {
    leds[XY(x, y)].setRGB(frame * -1, 0, 0);
  }
}


/**
 * Animation code
 */
float code(double t, double i, double x, double y) {
  switch (animation) {
    /**
     * Motus Art
     * @motus_art
     */
    case 0:
      // Plasma
      return sin(x+t)+sin(y+t)+sin(x+y+t)/3;
      break;

    case 1:
      // Up&Down
      return sin(cos(x)*y/8+t);
      break;

    case 2:
      return atan(x-(t*sin(t/7.5)))+sin(y/2);
      break;

    case 24:
      return sin(i/5+(t));
      break;

    case 25:
      return abs(sin(t))*32>x+y;
      break;


    /**
     * tixy.land website
     */
    case 3:
      // Emitting rings
      return sin(t-sqrt(pow((x-7.5), 2)+pow((y-6), 2)));
      break;

    case 4:
      // Rotation
      return sin(PI*2*atan((y-8)/(x-8))+5*t);
      break;

    case 5:
      // Vertical fade
      return sin(y/8+t);
      break;

    case 6:
      // Smooth noise
      return cos(t + i + x * y);
      break;

    case 7:
      // Waves
      return sin(x/2) - sin(x-t) - y+6;
      break;

    case 8:
      // Drop
      return fmod(8*t,13) - hypot(x-7.5, y-7.5);
      break;

    case 9:
      // Binary clock
      return (int)(t*10) & (1<<(int)x) && y==8;
      break;

    case 10:
      // Ripples @thespite
      return sin(t-sqrt(x*x+y*y));
      break;

    case 11:
      // Bloop bloop bloop @v21
      return (x-8)*(y-8) - sin(t)*64;
      break;


    /**
     * Reddit
     */
     case 12:
      // lurkerurke https://www.reddit.com/r/programming/comments/jpqbux/minimal_16x16_dots_coding_environment/gbgcwsn/
      return sin((x-7.5)*(y-7.5)/5*t+t);
      break;

    case 13:
      // SN0WFAKER https://www.reddit.com/r/programming/comments/jpqbux/minimal_16x16_dots_coding_environment/gbgk7c0/
      return sin(atan((y-7.5)/(x-7.5))+t*6);
      break;

    case 14:
      // LeadingNegotiation9 https://www.reddit.com/r/programming/comments/jpqbux/minimal_16x16_dots_coding_environment/gbjcoho/
      return pow(cos(((int)y^(int)x)+t), cos((x>y)+t));
      break;

    case 15:
      // detunized https://www.reddit.com/r/programming/comments/jpqbux/minimal_16x16_dots_coding_environment/gbgk30l/
      return sin(y/8+t)+x/16-0.5;
      break;

    case 16:
      // Andres_A https://www.reddit.com/r/programming/comments/jpqbux/minimal_16x16_dots_coding_environment/gbgzdnj/
      return 1-hypot(sin(t)*9-x,cos(t)*9-y)/9;
      break;


    /**
     * @akella
     * https://twitter.com/akella/status/1323549082552619008
     */
    case 17:
      return sin(6*atan2(y-8,x)+t);
      break;


    /**
     * Paul Malin
     * https://twitter.com/P_Malin/
     */
    case 18:
      // Parallax Scrolling Checkerboard https://twitter.com/P_Malin/status/1323609539648905218
      return ((int)((x-8)/y+t*5)&1^(int)(1/y*8)&1)*y/5;
      break;

    case 19:
      // Matrix Rain https://twitter.com/P_Malin/status/1323583013880553472
      return 1-fmod((x*x-y+t*(fmod(1+x*x,5.0))*3.0),16.0)/16.0;
      break;

    case 20:
      // Burst https://twitter.com/P_Malin/status/1323605999274594304
      return -4/((x-8)*(x-8)+(y-8)*(y-8)-fmod(t,1)*200);
      break;

    case 21:
      // Rays
      return sin(atan2(x,y)*5+t*5);
      break;

    case 22:
      // Starfield https://twitter.com/P_Malin/status/1323702220320313346
      return !((int)(x+t*50/(fmod(y*y,5.9)+1))&15)/(fmod(y*y,5.9)+1);
      break;

    case 23:
      // Hungry Pacman https://twitter.com/aemkei/status/1325185870173974530
      return hypot(x-=t*5,y-=8)<6&(x<y|y<-x);
      break;

    default:
      animation = 0;
      break;
  }
}