#include "VirtualButtonModule.h"

VirtualButtonModule::VirtualButtonModule()
{}
VirtualButtonModule::~VirtualButtonModule()
{}

const char* VirtualButtonModule::name()
{
    return "VirtualButtonModule";
}

void VirtualButtonModule::setup()
{
    for (uint8_t i = 0; i < BTN_ChannelCount; i++)
    {
        mVirtualButtons[i] = new VirtualButton(i);
        mVirtualButtons[i]->setup();
    }
}

void VirtualButtonModule::loop()
{
    for (uint8_t i = 0; i < BTN_ChannelCount; i++)
        mVirtualButtons[i]->loop();
}

void VirtualButtonModule::processInputKo(GroupObject& iKo)
{
    for (uint8_t i = 0; i < BTN_ChannelCount; i++)
        mVirtualButtons[i]->processInputKo(iKo);
}

void VirtualButtonModule::firstLoop()
{
    for (uint8_t i = 0; i < BTN_ChannelCount; i++)
        mVirtualButtons[i]->readStatus();
}

// void VirtualButtonModule::writeFlash()
// {
//     for (size_t i = 0; i < flashSize(); i++)
//     {
//         //openknx.flash.writeByte(0xd0 + i);
//     }
// }

// void VirtualButtonModule::readFlash(const uint8_t* data, const uint16_t size)
// {
//     // printHEX("RESTORE:", data,  len);
// }

// uint16_t VirtualButtonModule::flashSize()
// {
//     return 10;
// }