#include <IRremoteESP8266.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <IRsend.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFiMulti.h>

//IR-codes for Samsung TV(LTE310EX)
#define SAMSUNG_VOLUME_DOWN 0xE0E0D02F
#define SAMSUNG_VOLUME_UP 0xE0E0E01F
#define SAMSUNG_POWER 0xE0E040BF

// IR-codes for Panasonic SA PM-03
#define PANASONIC_VOLUME_DOWN 0x008481
#define PANASONIC_VOLUME_UP 0x000401
#define PANASONIC_POWER 0x38BC81

MDNSResponder mdns;
ESP8266WiFiMulti WiFiMulti;

// Wi-Fi settings
const char* ssid = "YOUR SSID HERE";  // Fix for your network
const char* password = "YOUR PASSWORD HERE";  // Fix for your network

IPAddress ip(192, 168, 0, 20);  // Fix for your network
IPAddress gateway(192, 168, 0, 1);  // Fix for your network
IPAddress subnet(255, 255, 255, 0);  // Fix for your network

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

  while (WiFiMulti.run() != WL_CONNECTED) {
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

  server.on("/", []() {
    server.send(200, "text/html", webPage());
  });

  server.on("/socketSamsungPower", []() {
    irsend.sendSAMSUNG(SAMSUNG_POWER, 32);
    Serial.println("Samsung Power Pressed");
    server.send(200, "text/html", webPage());
    delay(500);
  });

  server.on("/socketSamsungVolumeUp", []() {
    irsend.sendSAMSUNG(SAMSUNG_VOLUME_UP, 32);
    Serial.println("Samsung Volume Up Pressed");
    server.send(200, "text/html", webPage());
    delay(500);
  });

  server.on("/socketSamsungVolumeDown", []() {
    irsend.sendSAMSUNG(SAMSUNG_VOLUME_DOWN, 32);
    Serial.println("Samsung Volume Down Pressed");
    server.send(200, "text/html", webPage());
    delay(500);
  });

  server.on("/socketPanasonicPower", []() {
    irsend.sendPanasonic(0x4004, PANASONIC_POWER + 0x5000000);
    Serial.println("Panasonic Power Pressed");
    server.send(200, "text/html", webPage());
    delay(500);
  });

  server.on("/socketPanasonicVolumeUp", []() {
    irsend.sendPanasonic(0x4004, PANASONIC_VOLUME_UP + 0x5000000);
    Serial.println("Panasonic Volume Up Pressed");
    server.send(200, "text/html", webPage());
    delay(500);
  });

  server.on("/socketPanasonicVolumeDown", []() {
    irsend.sendPanasonic(0x4004, PANASONIC_VOLUME_DOWN + 0x5000000);
    Serial.println("Panasonic Volume Down Pressed");
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
  web += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/> <meta charset=\"utf-8\"><title>Nikitas Cinema Remote System</title><style>button{color:black;padding: 10px 27px;}</style></head>";
  web += "<p style=\"text-align: center;margin-top: 0px;margin-bottom: 5px;\">Samsung TV Remote</p>";
  web += "<div style=\"text-align: center;margin: 5px 0px;\"> <a href=\"socketSamsungPower\"><button>Power</button></a>&nbsp;</div>";
  web += "<div style=\"text-align: center;margin: 5px 0px;\"> <a href=\"socketSamsungVolumeUp\"><button>Volume Up</button></a>&nbsp;<a href=\"socketSamsungVolumeDown\"><button>Volume Down</button></a></div>";

  web += "<p style=\"text-align: center;margin-top: 0px;margin-bottom: 5px;\">Panasonic SA PM-03 Remote</p>";
  web += "<div style=\"text-align: center;margin: 5px 0px;\"> <a href=\"socketPanasonicPower\"><button>Power</button></a>&nbsp;</div>";
  web += "<div style=\"text-align: center;margin: 5px 0px;\"> <a href=\"socketPanasonicVolumeUp\"><button>Volume Up</button></a>&nbsp;<a href=\"socketPanasonicVolumeDown\"><button>Volume Down</button></a></div>";
  return (web);
}
