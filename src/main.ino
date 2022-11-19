#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

#define LED_PIN     0
#define NUM_LEDS    100
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

    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

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

    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    
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
    
}

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
