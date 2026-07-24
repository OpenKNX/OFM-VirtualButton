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
    uint8_t active = 0;
    for (uint8_t i = 0; i < BTN_ChannelCount; i++)
    {
        VirtualButtonChannel* ch = new VirtualButtonChannel(i);
        if (ch->isActive())
        {
            _channels[i] = ch;
            _channels[i]->setup();
            active++;
        }
        else
        {
            delete ch;
        }
    }
    logInfoP("Setup completed with %u/%u channels active", active, BTN_ChannelCount);
}

void VirtualButtonModule::loop()
{
    uint8_t processed = 0;
    do
    {
        if (_channels[_currentChannel] != nullptr)
            _channels[_currentChannel]->loop();
    }
    while (openknx.freeLoopIterate(BTN_ChannelCount, _currentChannel, processed));
}

void VirtualButtonModule::processInputKo(GroupObject& iKo)
{
    for (uint8_t i = 0; i < BTN_ChannelCount; i++)
        if (_channels[i] != nullptr) _channels[i]->processInputKo(iKo);
}

void VirtualButtonModule::processAfterStartupDelay()
{
    for (uint8_t i = 0; i < BTN_ChannelCount; i++)
        if (_channels[i] != nullptr) _channels[i]->readStatus();
}

VirtualButtonModule openknxVirtualButtonModule;