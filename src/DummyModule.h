#include "OpenKNX.h"

class DummyModule : public OpenKNX::Module
{

  public:
    DummyModule();
    ~DummyModule();

    const char* name() override;
    void writeFlash() override;
    void readFlash(const uint8_t* data, const uint16_t size) override;
    uint16_t flashSize() override;
};