#include "VirtualButtonModule.h"

const std::string VirtualButtonModule::name()
{
    return "VirtualButton";
}

const std::string VirtualButtonModule::version()
{
    return "0.0dev";
}

void VirtualButtonModule::setup()
{
    for (uint8_t i = 0; i < BTN_ChannelCount; i++)
    {
        _channels[i] = new VirtualButtonChannel(i);
        _channels[i]->setup();
    }
}

void VirtualButtonModule::loop()
{
    for (uint8_t i = 0; i < BTN_ChannelCount; i++)
        _channels[i]->loop();
}

void VirtualButtonModule::processInputKo(GroupObject& iKo)
{
    for (uint8_t i = 0; i < BTN_ChannelCount; i++)
        _channels[i]->processInputKo(iKo);
}

void VirtualButtonModule::processAfterStartupDelay()
{
    for (uint8_t i = 0; i < BTN_ChannelCount; i++)
        _channels[i]->readStatus();
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