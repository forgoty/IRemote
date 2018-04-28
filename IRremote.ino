#include <IRremoteESP8266.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFiMulti.h>

//IR-codes for Samsung TV(LTE310EX)
#define VOLUME_DOWN 0xE0E0D02F
#define VOLUME_UP 0xE0E0E01F
#define POWER 0xE0E040BF
/*#define PAUSE 0xE0E052AD
#define PREV 0xE0E0A25D
#define NEXT 0xE0E012ED
#define PLAY 0xE0E0E21D
#define STOP 0xE0E0629D*/

MDNSResponder mdns;
ESP8266WiFiMulti WiFiMulti;

// Wi-Fi settings
const char* ssid = "your_ssid";
const char* password = "your_password";

IPAddress ip(192,168,100,20);
IPAddress gateway(192,168,100,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

IRsend irsend(14); //an IR emitter led is connected to GPIO pin 4

void setup() {
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network
    WiFiMulti.addAP(ssid, password);

    Serial.println();
    Serial.println();
    Serial.print("Wait for WiFi... ");

    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(500);

    if (mdns.begin("esp8266", WiFi.localIP())) {
        Serial.println("MDNS responder started");
    }

    server.on("/", [](){
        server.send(200, "text/html", webPage());
    });

    server.on("/socketPower", [](){
        irsend.sendSAMSUNG(POWER, 32);
        Serial.println("Power Pressed");
        server.send(200, "text/html", webPage());
        delay(500);
    });

    server.on("/socketVolumeUp", [](){
        irsend.sendSAMSUNG(VOLUME_UP, 32);
        Serial.println("Volume Up Pressed");
        server.send(200, "text/html", webPage());
        delay(500);
    });

    server.on("/socketVolumeDown", [](){
        irsend.sendSAMSUNG(VOLUME_DOWN, 32);
        Serial.println("Volume Down Pressed");
        server.send(200, "text/html", webPage());
        delay(500);
    });

    server.begin();
    Serial.println("HTTP server started");
    irsend.begin();
}


void loop() {
    server.handleClient();
}


String webPage() {
    String web;
    web += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/> <meta charset=\"utf-8\"><title>Samsung TV Remote</title><style>button{color:black;padding: 10px 27px;}</style></head>";
    web += "<h1 style=\"text-align: center;font-family: Open sans;font-weight: 100;font-size: 20px;\">Samsung TV Remote</h1><div>";
    web += "<p style=\"text-align: center;margin-top: 0px;margin-bottom: 5px;\">Samsung TV Remote</p>";
    web += "<div style=\"text-align: center;margin: 5px 0px;\"> <a href=\"socketPower\"><button>Power</button></a>&nbsp;</div>";
    web += "<div style=\"text-align: center;margin: 5px 0px;\"> <a href=\"socketVolumeUp\"><button>Volume Up</button></a>&nbsp;<a href=\"socketVolumeDown\"><button>Volume Down</button></a></div>";
    return(web);
}



