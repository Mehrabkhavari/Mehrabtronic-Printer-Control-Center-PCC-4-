#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>

using namespace websockets;

const char* ssid = "STARLINK-5FF7";
const char* password = "25493351";

WebsocketsClient ws;

float nozzleTemp = 0;
float bedTemp = 0;

unsigned long lastHeartbeat = 0;

void onMessage(WebsocketsMessage message)
{
    Serial.print("RX: ");
    Serial.println(message.data());

    JsonDocument doc;

    if (deserializeJson(doc, message.data()) != DeserializationError::Ok)
        return;

    if (doc["nozzleTemp"].is<const char*>())
    {
        nozzleTemp = atof(doc["nozzleTemp"]);
    }

    if (doc["bedTemp0"].is<const char*>())
    {
        bedTemp = atof(doc["bedTemp0"]);
    }

    Serial.print("Nozzle: ");
    Serial.print(nozzleTemp);

    Serial.print("   Bed: ");
    Serial.println(bedTemp);
}

void setup()
{
    Serial.begin(115200);

    WiFi.begin(ssid,password);

    while(WiFi.status()!=WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi OK");

    ws.onMessage(onMessage);

    if(ws.connect("192.168.0.156",9999,"/"))
    {
        Serial.println("WebSocket Connected");
    }
}

void loop()
{
    ws.poll();

    if(millis()-lastHeartbeat>5000)
    {
        lastHeartbeat=millis();

        ws.send("{\"ModeCode\":\"heart_beat\",\"msg\":\"ESP32\"}");
    }
}