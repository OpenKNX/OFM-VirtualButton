#include "knx.h"
#include "OpenKNX.h"
#include "hardware.h"
#include "Logic.h"
#include "HardwareDevices.h"
#include "DummyModule.h"
#include "VirtualButtonModule.h"

void setup()
{
  const uint8_t firmwareRevision = 1;
  openknx.init(firmwareRevision);
  //openknx.addModule(3, new Logic());
  openknx.addModule(6, new DummyModule());
  openknx.addModule(4, new VirtualButtonModule());
  openknx.addModule(9, new DummyModule());
  openknx.setup();
}

void loop()
{
  openknx.loop();
}
