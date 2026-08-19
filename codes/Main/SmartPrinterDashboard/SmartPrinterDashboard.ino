#include <WiFi.h>
#include <CrealityKE.h>

// Wi-Fi credentials
const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// Printer IP addresses
const char* PRINTER_IP[4] =
{
    "192.168.0.157",   // Printer 1
    "192.168.0.158",   // Printer 2
    "192.168.0.159",   // Printer 3
    "192.168.0.160"    // Printer 4
};

// Create one library object for each printer
CrealityKE printer[4];

// 74HCT595 control pins
#define SR_DATA  23
#define SR_CLOCK 18
#define SR_LATCH 5

// Status update interval
const unsigned long STATUS_UPDATE_INTERVAL = 1000;

// Green blink interval for finished state
const unsigned long BLINK_INTERVAL = 500;

unsigned long lastStatusUpdate = 0;
unsigned long lastBlink = 0;

bool blinkState = false;

// Dashboard states
enum DashboardState
{
    STATE_IDLE,
    STATE_PRINTING,
    STATE_PAUSED,
    STATE_FINISHED,
    STATE_FAULT
};

// Temporary KE state mapping
#define KE_STATE_IDLE      0
#define KE_STATE_PRINTING  1
#define KE_STATE_PAUSED    2
#define KE_STATE_FINISHED  3

// LED outputs are active LOW
uint16_t ledOutputs = 0xFFFF;


// Set one LED state
void setLED(uint8_t bit, bool on)
{
    if (on)
        ledOutputs &= ~(1 << bit);
    else
        ledOutputs |= (1 << bit);
}


// Turn all LEDs off
void allLEDsOff()
{
    ledOutputs = 0xFFFF;
}


// Send 16 bits to the two shift registers
void updateShiftRegisters()
{
    digitalWrite(SR_LATCH, LOW);

    shiftOut(
        SR_DATA,
        SR_CLOCK,
        MSBFIRST,
        (ledOutputs >> 8) & 0xFF
    );

    shiftOut(
        SR_DATA,
        SR_CLOCK,
        MSBFIRST,
        ledOutputs & 0xFF
    );

    digitalWrite(SR_LATCH, HIGH);
}


// Convert printer state to dashboard state
DashboardState getDashboardState(uint8_t index)
{
    // Disconnection always has highest priority
    if (!printer[index].connected())
        return STATE_FAULT;

    // Read raw printer state
    int state = printer[index].state();

    // Temporary state mapping
    switch (state)
    {
        case KE_STATE_IDLE:
            return STATE_IDLE;

        case KE_STATE_PRINTING:
            return STATE_PRINTING;

        case KE_STATE_PAUSED:
            return STATE_PAUSED;

        case KE_STATE_FINISHED:
            return STATE_FINISHED;

        default:
            return STATE_FAULT;
    }
}


// Set LEDs for one printer
void updatePrinterLEDs(uint8_t printerIndex)
{
    DashboardState state = getDashboardState(printerIndex);

    // Each printer owns three consecutive outputs
    uint8_t baseBit = printerIndex * 3;

    // Turn this printer's LEDs off first
    setLED(baseBit + 0, false);
    setLED(baseBit + 1, false);
    setLED(baseBit + 2, false);

    switch (state)
    {
        case STATE_IDLE:
            break;

        case STATE_PRINTING:
            setLED(baseBit + 0, true);
            break;

        case STATE_PAUSED:
            setLED(baseBit + 1, true);
            break;

        case STATE_FINISHED:
            if (blinkState)
                setLED(baseBit + 0, true);
            break;

        case STATE_FAULT:
            setLED(baseBit + 2, true);
            break;
    }
}


// Update all twelve status LEDs
void updateAllLEDs()
{
    allLEDsOff();

    for (uint8_t i = 0; i < 4; i++)
    {
        updatePrinterLEDs(i);
    }

    updateShiftRegisters();
}


// Print printer information
void printPrinterStatus(uint8_t index)
{
    Serial.println("----------------------------------------");

    Serial.print("PRINTER ");
    Serial.println(index + 1);

    Serial.print("Connected      : ");
    Serial.println(
        printer[index].connected() ? "YES" : "NO"
    );

    Serial.print("Raw State      : ");
    Serial.println(
        printer[index].state()
    );

    Serial.print("Nozzle         : ");
    Serial.print(
        printer[index].nozzleTemp()
    );

    Serial.print(" / ");
    Serial.println(
        printer[index].targetNozzleTemp()
    );

    Serial.print("Bed            : ");
    Serial.print(
        printer[index].bedTemp()
    );

    Serial.print(" / ");
    Serial.println(
        printer[index].targetBedTemp()
    );

    Serial.print("Progress       : ");
    Serial.print(
        printer[index].progress()
    );

    Serial.println("%");

    Serial.print("Layer          : ");
    Serial.print(
        printer[index].currentLayer()
    );

    Serial.print(" / ");
    Serial.println(
        printer[index].totalLayers()
    );

    Serial.print("Print Time     : ");
    Serial.println(
        printer[index].printTime()
    );

    Serial.print("Remaining Time : ");
    Serial.println(
        printer[index].remainingTime()
    );
}


// Connect ESP32 to Wi-Fi
void connectWiFi()
{
    WiFi.mode(WIFI_STA);

    WiFi.begin(
        WIFI_SSID,
        WIFI_PASSWORD
    );

    Serial.print("Connecting to Wi-Fi");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("Wi-Fi connected");

    Serial.print("ESP32 IP: ");
    Serial.println(WiFi.localIP());
}


// Arduino setup
void setup()
{
    Serial.begin(115200);

    // Configure shift register pins
    pinMode(SR_DATA, OUTPUT);
    pinMode(SR_CLOCK, OUTPUT);
    pinMode(SR_LATCH, OUTPUT);

    // Start with all LEDs off
    allLEDsOff();
    updateShiftRegisters();

    // Connect to Wi-Fi
    connectWiFi();

    Serial.println();
    Serial.println("Starting printer connections...");

    // Start all four printer connections
    for (uint8_t i = 0; i < 4; i++)
    {
        Serial.print("Printer ");
        Serial.print(i + 1);
        Serial.print(" -> ");
        Serial.println(PRINTER_IP[i]);

        printer[i].begin(PRINTER_IP[i]);
    }
}


// Main program loop
void loop()
{
    // Keep all printer connections alive
    for (uint8_t i = 0; i < 4; i++)
    {
        printer[i].loop();
    }

    // Update green blink state
    if (millis() - lastBlink >= BLINK_INTERVAL)
    {
        lastBlink = millis();
        blinkState = !blinkState;
    }

    // Update dashboard
    if (millis() - lastStatusUpdate >= STATUS_UPDATE_INTERVAL)
    {
        lastStatusUpdate = millis();

        Serial.println();
        Serial.println("========================================");
        Serial.println("     MEHRABTRONIC PRINTER DASHBOARD");
        Serial.println("========================================");

        // Print all printer statuses
        for (uint8_t i = 0; i < 4; i++)
        {
            printPrinterStatus(i);
        }

        // Update physical LEDs
        updateAllLEDs();
    }
}