#include <WiFi.h>

#include "PrinterManager.h"


// ============================================================
// WiFi Configuration
// ============================================================

const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";


// ============================================================
// Printer Configuration
// ============================================================

PrinterInfo printers[PRINTER_COUNT] =
{
    { "192.168.1.101", "Printer 1" },
    { "192.168.1.102", "Printer 2" },
    { "192.168.1.103", "Printer 3" },
    { "192.168.1.104", "Printer 4" }
};


// ============================================================
// Printer Manager
// ============================================================

PrinterManager printerManager;


// ============================================================
// Status Update Timer
// ============================================================

unsigned long lastStatusPrint = 0;

const unsigned long STATUS_INTERVAL = 1000;


// ============================================================
// WiFi
// ============================================================

void connectWiFi()
{
    Serial.print("Connecting to WiFi");

    WiFi.mode(WIFI_STA);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);

        Serial.print(".");
    }

    Serial.println();

    Serial.println("WiFi connected");

    Serial.print("ESP32 IP: ");

    Serial.println(WiFi.localIP());
}


// ============================================================
// Print Printer Status
// ============================================================

void printPrinterStatus(uint8_t index)
{
    CrealityKE& printer = printerManager.get(index);

    Serial.print("P");

    Serial.print(index + 1);

    Serial.print(" | ");


    // -------------------------
    // Connection
    // -------------------------

    if (!printer.connected())
    {
        Serial.println("OFFLINE");

        return;
    }


    // -------------------------
    // Progress
    // -------------------------

    Serial.print("Progress: ");

    Serial.print(printer.progress());

    Serial.print("%");


    // -------------------------
    // Nozzle
    // -------------------------

    Serial.print(" | Nozzle: ");

    Serial.print(printer.nozzleTemp(), 1);

    Serial.print(" C");


    // -------------------------
    // Bed
    // -------------------------

    Serial.print(" | Bed: ");

    Serial.print(printer.bedTemp(), 1);

    Serial.print(" C");


    // -------------------------
    // Remaining Time
    // -------------------------

    Serial.print(" | Remaining: ");

    Serial.print(printer.remainingTime());

    Serial.print(" s");


    // -------------------------
    // State
    // -------------------------

    Serial.print(" | State: ");

    Serial.println(printer.state());
}


// ============================================================
// Setup
// ============================================================

void setup()
{
    Serial.begin(115200);

    delay(1000);


    Serial.println();

    Serial.println("========================================");

    Serial.println(" Mehrabtronic Printer Control Center");

    Serial.println(" PCC-4 - Core Test v0.1");

    Serial.println("========================================");


    // Connect WiFi

    connectWiFi();


    // Start Printer Manager

    Serial.println();

    Serial.println("Starting printer connections...");

    printerManager.begin(printers);

    Serial.println("Printer manager started.");
}


// ============================================================
// Main Loop
// ============================================================

void loop()
{
    // Process all printer connections

    printerManager.loop();


    // Print status every second

    if (millis() - lastStatusPrint >= STATUS_INTERVAL)
    {
        lastStatusPrint = millis();


        Serial.println();

        Serial.println("------------- PRINTER STATUS -------------");


        for (uint8_t i = 0; i < PRINTER_COUNT; i++)
        {
            printPrinterStatus(i);
        }


        Serial.println("------------------------------------------");
    }
}