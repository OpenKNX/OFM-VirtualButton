#pragma once
#include "knx.h"
#include "Helper.h"
#include "KnxHelper.h"
#include "knxprod.h"
#include "hardware.h"

#define BTN_MaxMuliClicks 3

struct sVirtualButtonParams
{
  uint8_t outputShort;
  uint8_t outputLong;
  uint8_t outputExtraLong;
  uint16_t inputKo;
};

struct sVirtualButtonGlobalParams
{
  uint8_t mode = 0;
  uint8_t lock = 0;
  uint8_t outputShort = 0;
  uint8_t outputLong = 0;
  uint8_t outputExtraLong = 0;
  bool eventShort = false;
  bool eventLong = false;
  bool eventExtraLong = false;
  uint16_t reactionTimeMultiClick;
  uint16_t reactionTimeLong;
  uint16_t reactionTimeExtraLong;
  uint8_t dynamicStatusThreshold = 0;
  uint32_t dynamicStatusFallback = 0;
};

struct sVirtualButtonState
{
  bool press = false;
  bool pressLong = false;
  bool pressExtraLong = false;
  uint32_t pressStart = 0;
  uint32_t multiClickTimer = 0;
  int8_t multiClicks = 0;
};

class VirtualButton
{

private:
  uint32_t calcParamIndex(uint16_t iParamIndex);
  uint16_t calcKoNumber(uint8_t iKoIndex);
  int8_t calcKoIndex(uint16_t iKoNumber);
  GroupObject *getKo(uint8_t iKoIndex);

  void processInputKoInput(GroupObject &iKo, bool iButton);
  void processInputKoLock(GroupObject &iKo);
  void processPress(bool iButton);
  void processRelease(bool iButton);
  void processPressAndHold(bool iButton);
  void processMultiClick();
  void eventMultiClick(uint8_t iClicks);
  void eventShortPress(bool iButton);
  void eventLongPress(bool iButton);
  void eventExtraLongPress(bool iButton);
  void eventShortRelease(bool iButton);
  void eventLongRelease(bool iButton);
  void eventExtraLongRelease(bool iButton);
  void dim(bool iButton, bool iRelease);
  void writeSwitchOutput(uint8_t iOutput, uint8_t iValue, bool &oStatus, uint8_t iKoOutput);
  void processInputKoStatus(GroupObject &iKom, uint8_t iStatusNumber, bool &oStatus);
  void processDynamicStatus();

  uint8_t mIndex = 0;
  uint8_t mLock = 0;
  bool mStatusShort = false;
  bool mStatusLong = false;
  bool mStatusExtraLong = false;
  u_int32_t mDynamicStatusTimer = 0;

  sVirtualButtonState mButtonState[2] = {
    sVirtualButtonState(),
    sVirtualButtonState()
  };
  sVirtualButtonParams mButtonParams[2] = {
    sVirtualButtonParams(),
    sVirtualButtonParams()
  };
  sVirtualButtonGlobalParams mParams;
  uint8_t mMultiClickParams[3];


public:
  VirtualButton(uint8_t iIndex);
  ~VirtualButton();

  void setup();
  void loop();
  void processInputKo(GroupObject &iKo);
};