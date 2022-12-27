#pragma once
#include "knx.h"
#include "Helper.h"
#include "KnxHelper.h"
#include "knxprod.h"
#include "hardware.h"

#define BTN_MaxMuliClicks 3
#define BTN_DPT1 1
#define BTN_DPT2 2
#define BTN_DPT5 4
#define BTN_DPT5001 5
#define BTN_DPT18 6
#define BTN_DPT3007 7
#define BTN_DPT3008 8
#define BTN_DPT7 3

struct sMultiClickParams
{
  bool active = false;
  uint16_t output;
};

struct sVirtualButtonParams
{
  bool outputShortPressActive = false;
  bool outputShortReleaseActive = false;
  uint16_t outputShortPress;
  uint16_t outputShortRelease;

  bool outputLongPressActive = false;
  bool outputLongReleaseActive = false;
  uint16_t outputLongPress;
  uint16_t outputLongRelease;

  bool outputExtraLongPressActive = false;
  bool outputExtraLongReleaseActive = false;
  uint16_t outputExtraLongPress;
  uint16_t outputExtraLongRelease;

  uint16_t inputKo;
  uint16_t output2Short = 0;
  uint16_t output2Long = 0;
  uint16_t output2ExtraLong = 0;
};

struct sVirtualButtonGlobalParams
{
  uint8_t mode = 0;
  uint8_t lock = 0;
  uint8_t outputShortDpt = 0;
  uint8_t outputLongDpt = 0;
  uint8_t outputExtraLongDpt = 0;

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
  // void dim(bool iButton, bool iRelease);
  void writeOutput(uint8_t iOutputDPT, uint16_t iOutputKo, uint16_t iOutputValue, bool &oStatus);
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
      sVirtualButtonState()};
  sVirtualButtonParams mButtonParams[2] = {
      sVirtualButtonParams(),
      sVirtualButtonParams()};
  sVirtualButtonGlobalParams mParams;
  sMultiClickParams mMultiClickParams[3];

public:
  VirtualButton(uint8_t iIndex);
  ~VirtualButton();

  void setup();
  void loop();
  void processInputKo(GroupObject &iKo);
};