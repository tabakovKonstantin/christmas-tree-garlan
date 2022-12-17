#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

#define LED_PIN     0
#define NUM_LEDS    200
#define BRIGHTNESS  50
#define LED_TYPE    SK6812
#define COLOR_ORDER RGB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

#include <vector>


AsyncWebServer server(80);

const char* ssid = "A1_CA2F";
const char* password = "48575443202C3FAA";

const char* PARAM_MESSAGE = "message";

void changePalette(std::vector<std::pair<CRGBPalette16, TBlendType>> palettes, CRGBPalette16 *currentPalette, TBlendType *currentBlending);

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

std::vector<std::pair<CRGBPalette16, TBlendType>>  palettes;
CRGBPalette16 currentPalette;
TBlendType    currentBlending;


extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;


void setup() {

    FastLED.clear(true);
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );

    // for( int i = 0; i < NUM_LEDS; ++i) {
    //     leds[i] = CRGB::ForestGreen;
    //     FastLED.show();
    // }
    

    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        for( int i = 0; i < NUM_LEDS; ++i) {
            leds[i] = CRGB::Red;
            FastLED.show();
        }
        //return;
    }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    for( int i = 0; i < NUM_LEDS; ++i) {
      leds[i] = CRGB::Green;
      FastLED.show();
    }

    delay( 3000 );
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        // ChangePalettePeriodically();
        changePalette(palettes, &currentPalette, &currentBlending);
        request->send(200, "text/plain", "Changed");
    });

    // Send a GET request to <IP>/get?message=<message>
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String message;
        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, GET: " + message);
    });

    // Send a POST request to <IP>/post with a form field message set to <message>
    server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
        String message;
        if (request->hasParam(PARAM_MESSAGE, true)) {
            message = request->getParam(PARAM_MESSAGE, true)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, POST: " + message);
    });

    server.onNotFound(notFound);

    server.begin();

    
    initPalettes();
    currentPalette = OceanColors_p;
    currentBlending = LINEARBLEND;
}


void loop()
{
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    
    FillLEDsFromPaletteColors( startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}               

void initPalettes()
{
    palettes.push_back(std::pair<CRGBPalette16, TBlendType>(OceanColors_p, LINEARBLEND));
    palettes.push_back(std::pair<CRGBPalette16, TBlendType>(CloudColors_p, LINEARBLEND));
    palettes.push_back(std::pair<CRGBPalette16, TBlendType>(LavaColors_p, LINEARBLEND));
    palettes.push_back(std::pair<CRGBPalette16, TBlendType>(ForestColors_p, LINEARBLEND));
    palettes.push_back(std::pair<CRGBPalette16, TBlendType>(PartyColors_p, LINEARBLEND));
    palettes.push_back(std::pair<CRGBPalette16, TBlendType>(RainbowColors_p, LINEARBLEND));
    palettes.push_back(std::pair<CRGBPalette16, TBlendType>(RainbowStripeColors_p, LINEARBLEND));
    palettes.push_back(std::pair<CRGBPalette16, TBlendType>(RainbowStripeColors_p, NOBLEND));
    palettes.push_back(std::pair<CRGBPalette16, TBlendType>(HeatColors_p, NOBLEND));
    palettes.push_back(std::pair<CRGBPalette16, TBlendType>(HeatColors_p, LINEARBLEND));
    palettes.push_back(std::pair<CRGBPalette16, TBlendType>(myRedWhiteBluePalette_p, NOBLEND));
    palettes.push_back(std::pair<CRGBPalette16, TBlendType>(myRedWhiteBluePalette_p, LINEARBLEND));
    palettes.push_back(std::pair<CRGBPalette16, TBlendType>(SetupPurpleAndGreenPalette(), NOBLEND));
    palettes.push_back(std::pair<CRGBPalette16, TBlendType>(SetupPurpleAndGreenPalette(), LINEARBLEND));
    palettes.push_back(std::pair<CRGBPalette16, TBlendType>(SetupBlackAndWhiteStripedPalette(), NOBLEND));
    palettes.push_back(std::pair<CRGBPalette16, TBlendType>(SetupBlackAndWhiteStripedPalette(), LINEARBLEND));
    
}

CRGBPalette16 SetupBlackAndWhiteStripedPalette()
{
  CRGBPalette16 currentPalette1 =  CRGBPalette16();
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette1, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette1[0] = CRGB::White;
  currentPalette1[4] = CRGB::White;
  currentPalette1[8] = CRGB::White;
  currentPalette1[12] = CRGB::White;

  return currentPalette1;

}

// This function sets up a palette of purple and green stripes.
CRGBPalette16 SetupPurpleAndGreenPalette()
{
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;
  
  return CRGBPalette16( 
    green,  green,  black,  black,
    purple, purple, black,  black,
    green,  green,  black,  black,
    purple, purple, black,  black );
}


const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Gray, // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};

void changePalette(std::vector<std::pair<CRGBPalette16, TBlendType>> palettes, CRGBPalette16 *currentPalette, TBlendType *currentBlending)
{
    for (uint8_t i = 0; i < palettes.size(); i++)
    {
		std::pair<CRGBPalette16, TBlendType> curr = palettes[i];
        if(curr.first == *currentPalette && curr.second == *currentBlending)
        {
            uint8_t nextItemIdex = i + 1;
            if(nextItemIdex == palettes.size())
            {
                nextItemIdex = 0;
            } 

            std::pair<CRGBPalette16, TBlendType> next = palettes[nextItemIdex];
            *currentPalette = next.first;
            *currentBlending = next.second;
            
            return;
        }
    }  
}
