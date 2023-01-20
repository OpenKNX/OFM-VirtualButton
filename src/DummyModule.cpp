#include "DummyModule.h"

DummyModule::DummyModule()
{}
DummyModule::~DummyModule()
{}

const char* DummyModule::name()
{
    return "DummyModule";
}

void DummyModule::writeFlash()
{
    for (size_t i = 0; i < 10; i++)
    {
        openknx.flash.writeByte((0xb0 + i));
    }
}

void DummyModule::readFlash(const uint8_t* data, const uint16_t size)
{
    //printHEX("RESTORE:", data,  size);
}

uint16_t DummyModule::flashSize()
{
    return 20;
}
