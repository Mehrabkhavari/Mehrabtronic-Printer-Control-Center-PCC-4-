#ifndef PRINTER_MANAGER_H
#define PRINTER_MANAGER_H

#include <Arduino.h>
#include <CrealityKE.h>

#define PRINTER_COUNT 4

struct PrinterInfo
{
    const char* ip;
    const char* name;
};

class PrinterManager
{
public:

    PrinterManager();

    void begin(const PrinterInfo printers[PRINTER_COUNT]);

    void loop();

    CrealityKE& get(uint8_t index);

    bool validIndex(uint8_t index) const;

private:

    CrealityKE _printers[PRINTER_COUNT];

    PrinterInfo _info[PRINTER_COUNT];
};

#endif