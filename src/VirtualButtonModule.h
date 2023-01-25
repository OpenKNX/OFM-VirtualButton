#include "VirtualButton.h"
#include "hardware.h"

class VirtualButtonModule : public OpenKNX::Module
{

  public:
    VirtualButtonModule();
    ~VirtualButtonModule();

    void loop();
    void setup();
    void firstLoop();
    void processInputKo(GroupObject& iKo);

    const char* name() override;
    // void writeFlash() override;
    // void readFlash(const uint8_t* data, const uint16_t size) override;
    // uint16_t flashSize() override;

  private:
    VirtualButton* mVirtualButtons[BTN_ChannelCount];
};