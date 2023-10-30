#include "VirtualButtonModule.h"
#include "ModuleVersionCheck.h"

const std::string VirtualButtonModule::name()
{
    return "VirtualButton";
}

const std::string VirtualButtonModule::version()
{
    return MODULE_VirtualButton_Version;
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
    uint8_t processed = 0;
    do
        _channels[_currentChannel]->loop();
    while (openknx.freeLoopIterate(BTN_ChannelCount, _currentChannel, processed));
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

VirtualButtonModule openknxVirtualButtonModule;