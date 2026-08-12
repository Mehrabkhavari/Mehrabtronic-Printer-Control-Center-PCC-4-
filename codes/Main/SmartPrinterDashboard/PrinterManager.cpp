#include "PrinterManager.h"

PrinterManager::PrinterManager()
{
}

void PrinterManager::begin(const PrinterInfo printers[PRINTER_COUNT])
{
    for (uint8_t i = 0; i < PRINTER_COUNT; i++)
    {
        _info[i] = printers[i];

        _printers[i].begin(_info[i].ip);
    }
}

void PrinterManager::loop()
{
    for (uint8_t i = 0; i < PRINTER_COUNT; i++)
    {
        _printers[i].loop();
    }
}

CrealityKE& PrinterManager::get(uint8_t index)
{
    return _printers[index];
}

bool PrinterManager::validIndex(uint8_t index) const
{
    return index < PRINTER_COUNT;
}