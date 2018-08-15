/*
  @author  Lucas Martins Mendes @ csi
  @date qui ago  9 17:36:03 -03 2018
  @
*/

/*---- Include ----*/

#include <Adafruit_NeoPixel.h>

/*---- Constants ----*/
#define PIN 6 /*!< communication pin*/

#define LED_N 24 /*!< total number of LED's on strip/disc*/
#define THRESH 120.0
const String opts =
  "s r g b - set color\n"
  "c - clear color\n"
  "r - rainbow\n";
  
/*----- Library initialization*/
/*
 Parameter 1 = number of pixels in strip
  Parameter 2 = pin number (most are valid)
  Parameter 3 = pixel type flags, add together as needed:
   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 driver)
   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
*/
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_N, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{

  Serial.begin(115200);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

char cmd;
String arg;
int r,g,b;

void loop()
{
  if(Serial.available())/*wait for input*/
    {
      arg=Serial.readString();
      sscanf(arg.c_str(),"%c %i %i %i",&cmd,&r,&g,&b);
      switch(cmd)
        {
        case 's':
          if(r>THRESH||g>THRESH||b>THRESH)
            {
              Serial.println("warning: some power sources may overload on full power, do you want to continue?[y/n]");
              char c;
              do
                {
                  c= Serial.read();
                  if(c=='n')
                    {
                      break;
                    }
                }while(c!='y'&&c!='n');
            }
          color_wash(LED_N,r,g,b);
          break;
        case 'c':
          color_wash(LED_N,0.0,0.0,0.0);
          break;
        case 'r':
          for(float j=0;j<(2*M_PI);j+=M_PI/100)
            {
              #define AMPL 50
              for(int i=0; i<LED_N;i++)
                {
                  strip.setPixelColor(i,
                                      AMPL+AMPL*sin(j    +2*M_PI*(i/(float)LED_N)),
                                      AMPL+AMPL*sin(j*3.0+2*M_PI*(i/(float)LED_N)),
                                      AMPL+AMPL*sin(j*5.0+2*M_PI*(i/(float)LED_N))
                                      );
                }
              strip.show();
              delay(10);
            }
          color_wash(LED_N,0,0,0);
          break;
        default:
          Serial.println("Ivalid option\n" + opts);
          break;
        }
    }
  else
    {
      delay(100); 
    }
}

/*---- Function Definition ------*/
void color_wash(unsigned int n,float r,float g,float b)
{
  for(int i=0;i<n;i++)
    {
      strip.setPixelColor(i,r,g,b);
    }
  strip.show();
}


