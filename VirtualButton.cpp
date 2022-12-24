#include "VirtualButton.h"

uint32_t VirtualButton::calcParamIndex(uint16_t iParamIndex)
{
  return iParamIndex + mIndex * BTN_ParamBlockSize + BTN_ParamBlockOffset;
}

uint16_t VirtualButton::calcKoNumber(uint8_t iKoIndex)
{
  return iKoIndex + mIndex * BTN_KoBlockSize + BTN_KoOffset;
}

int8_t VirtualButton::calcKoIndex(uint16_t iKoNumber)
{
  int16_t lResult = (iKoNumber - BTN_KoOffset);
  // check if channel is valid
  if ((int8_t)(lResult / BTN_KoBlockSize) == mIndex)
    lResult = lResult % BTN_KoBlockSize;
  else
    lResult = -1;
  return (int8_t)lResult;
}

GroupObject *VirtualButton::getKo(uint8_t iKoIndex)
{
  return &knx.getGroupObject(calcKoNumber(iKoIndex));
}

VirtualButton::VirtualButton(uint8_t iIndex)
{
  this->mIndex = iIndex;
}

void VirtualButton::setup()
{
  // Global Params
  mParams.mode = knx.paramByte(calcParamIndex(BTN_BTNMode));
  mParams.lock = (knx.paramByte(calcParamIndex(BTN_BTNLock)) & BTN_BTNLockMask) >> BTN_BTNLockShift;

  mParams.outputShort = knx.paramByte(calcParamIndex(BTN_BTNOutputShort));
  mParams.outputLong = knx.paramByte(calcParamIndex(BTN_BTNOutputLong));
  mParams.outputExtraLong = knx.paramByte(calcParamIndex(BTN_BTNOutputExtraLong));

  mParams.eventShort = knx.paramByte(calcParamIndex(BTN_BTNOutputShortEvent));
  mParams.eventLong = knx.paramByte(calcParamIndex(BTN_BTNOutputLongEvent));
  mParams.eventExtraLong = knx.paramByte(calcParamIndex(BTN_BTNOutputExtraLongEvent));

  mButtonParams[0].inputKo = knx.paramWord(calcParamIndex(BTN_BTNInputA));
  mButtonParams[0].outputShort = knx.paramByte(calcParamIndex(BTN_BTNOutput1ShortDpt1));
  mButtonParams[0].outputLong = knx.paramByte(calcParamIndex(BTN_BTNOutput1LongDpt1));
  mButtonParams[0].outputExtraLong = knx.paramByte(calcParamIndex(BTN_BTNOutput1ExtraLongDpt1));

  mButtonParams[1].inputKo = knx.paramWord(calcParamIndex(BTN_BTNInputB));
  mButtonParams[1].outputShort = knx.paramByte(calcParamIndex(BTN_BTNOutput2ShortDpt1));
  mButtonParams[1].outputLong = knx.paramByte(calcParamIndex(BTN_BTNOutput2LongDpt1));
  mButtonParams[1].outputExtraLong = knx.paramByte(calcParamIndex(BTN_BTNOutput2ExtraLongDpt1));

  // ReactionTimes
  mParams.reactionTimeMultiClick = knx.paramByte(calcParamIndex(BTN_BTNReactionTimeMultiClick));
  if (mParams.reactionTimeMultiClick > 0)
    mParams.reactionTimeMultiClick *= 100;
  else
    mParams.reactionTimeMultiClick = knx.paramByte(BTN_ReactionTimeMultiClick) * 100;

  // For Dim & Jal nutze eine separate Longtime
  if (mParams.outputLong == 7 || mParams.outputLong == 8)
  {
    if (mParams.reactionTimeLong > 0)
      mParams.reactionTimeLong *= 100;
    else
      mParams.reactionTimeLong = knx.paramByte(BTN_ReactionTimeSpecial) * 100;
  }
  else
  {
    if (mParams.reactionTimeLong > 0)
      mParams.reactionTimeLong *= 100;
    else
      mParams.reactionTimeLong = knx.paramByte(BTN_ReactionTimeLong) * 100;
  }

  if (mParams.reactionTimeExtraLong > 0)
    mParams.reactionTimeExtraLong *= 100;
  else
    mParams.reactionTimeExtraLong = knx.paramByte(BTN_ReactionTimeExtraLong) * 100;

  mMultiClickParams[0] = knx.paramByte(calcParamIndex(BTN_BTNM1Dpt1));
  mMultiClickParams[1] = knx.paramByte(calcParamIndex(BTN_BTNM2Dpt1));
  mMultiClickParams[2] = knx.paramByte(calcParamIndex(BTN_BTNM3Dpt1));

  mParams.dynamicStatusThreshold = knx.paramByte(calcParamIndex(BTN_BTNStatusThreshold));
  mParams.dynamicStatusFallback = (getDelayPattern(calcParamIndex(BTN_BTNStatusFallbackBase)));

  mDynamicStatusTimer = millis() + mParams.dynamicStatusFallback;

  // Debug
  // SERIAL_DEBUG.printf("BTN %i inputKo: %i/%i\n\r", mIndex, mButtonParams[0].inputKo, calcParamIndex(BTN_BTNInputKoA));
  // SERIAL_DEBUG.printf("BTN %i outputShort: %i/%i\n\r", mIndex, mButtonParams[0].outputShort, calcParamIndex(BTN_BTNOutput1ShortDpt1));
  // SERIAL_DEBUG.printf("BTN %i outputLong: %i/%i\n\r", mIndex, mButtonParams[0].outputLong, calcParamIndex(BTN_BTNOutput1LongDpt1));
  // SERIAL_DEBUG.printf("BTN %i outputExtraLong: %i/%i\n\r", mIndex, mButtonParams[0].outputExtraLong, calcParamIndex(BTN_BTNOutput1ExtraLongDpt1));
  // SERIAL_DEBUG.printf("BTN %i lock: %i\n\r", mIndex, mParams.lock);
  // SERIAL_DEBUG.printf("BTN %i eventShort: %i\n\r", mIndex, mParams.eventShort);
  // SERIAL_DEBUG.printf("BTN %i eventLong: %i\n\r", mIndex, mParams.eventLong);
  // SERIAL_DEBUG.printf("BTN %i eventExtraLong: %i\n\r", mIndex, mParams.eventExtraLong);
  // SERIAL_DEBUG.printf("BTN %i mMultiClickParams[0]: %i\n\r", mIndex, mMultiClickParams[0]);
  // SERIAL_DEBUG.printf("BTN %i mMultiClickParams[1]: %i\n\r", mIndex, mMultiClickParams[1]);
  // SERIAL_DEBUG.printf("BTN %i mMultiClickParams[2]: %i\n\r", mIndex, mMultiClickParams[2]);
  // SERIAL_DEBUG.printf("BTN %i reactionTimeMultiClick: %i\n\r", mIndex, mParams.reactionTimeMultiClick);
  // SERIAL_DEBUG.printf("BTN %i reactionTimeLong: %i\n\r", mIndex, mParams.reactionTimeLong);
  // SERIAL_DEBUG.printf("BTN %i reactionTimeExtraLong: %i\n\r", mIndex, mParams.reactionTimeExtraLong);
  // SERIAL_DEBUG.printf("BTN %i dynamicStatusThreshold: %i\n\r", mIndex, mParams.dynamicStatusThreshold);
  // SERIAL_DEBUG.printf("BTN %i dynamicStatusFallback: %i\n\r", mIndex, mParams.dynamicStatusFallback);
  // SERIAL_DEBUG.printf("BTN %i mParamMode: %i\n\r", mIndex, mParams.mode);
}

void VirtualButton::loop()
{
  processPressAndHold(0);
  processPressAndHold(1);
  processMultiClick();
  processDynamicStatus();
}

void VirtualButton::processDynamicStatus()
{
  if (mDynamicStatusTimer > 0 && delayCheck(mDynamicStatusTimer, mParams.dynamicStatusFallback))
  {
    uint8_t lValue = getKo(BTN_KoBTNOutput2Status)->value(DPT_Scaling);
    mStatusLong = (lValue < mParams.dynamicStatusThreshold) ? false : true;
    SERIAL_DEBUG.printf("BTN::processDynamicStatus %i/%i/%i: triggered\n\r", mIndex, lValue, mStatusLong);

    mDynamicStatusTimer = 0;
  }
}

void VirtualButton::processInputKo(GroupObject &iKo)
{
  if (mParams.mode == 0)
    return;

  uint16_t lKoNumber = iKo.asap();
  uint8_t lKoIndex = calcKoIndex(lKoNumber);

  // Internal
  if (mButtonParams[0].inputKo > 0 && mButtonParams[0].inputKo == lKoNumber)
  {
    lKoIndex = BTN_KoBTNInput1;
  }
  else if (mButtonParams[1].inputKo > 0 && mButtonParams[1].inputKo == lKoNumber)
  {
    lKoIndex = BTN_KoBTNInput2;
  }

  switch (lKoIndex)
  {
  case BTN_KoBTNInput1:
    processInputKoInput(iKo, 0);
    break;
  case BTN_KoBTNInput2:
    processInputKoInput(iKo, 1);
    break;
  case BTN_KoBTNLock:
    processInputKoLock(iKo);
    break;
  case BTN_KoBTNOutput1Status:
    processInputKoStatus(iKo, 1, mStatusShort);
    break;
  case BTN_KoBTNOutput2Status:
    processInputKoStatus(iKo, 2, mStatusLong);
    break;
  case BTN_KoBTNOutput3Status:
    processInputKoStatus(iKo, 3, mStatusExtraLong);
    break;
  }
}

void VirtualButton::processInputKoStatus(GroupObject &iKo, uint8_t iStatusNumber, bool &oStatus)
{
  SERIAL_DEBUG.printf("BTN::processInputKoStatus %i: %i/%i\n\r", mIndex, iStatusNumber, oStatus);

  // Special for Long DPT3007
  if (iStatusNumber == 2 && mParams.outputLong == 7)
  {
    uint8_t lValue = iKo.value(DPT_Scaling);
    if (lValue == 0 && oStatus)
      oStatus = false;
    if (lValue == 100 && !oStatus)
      oStatus = true;
  }
  else
  {
    bool lValue = iKo.value(DPT_Switch);
    oStatus = lValue;
  }
}

void VirtualButton::processInputKoLock(GroupObject &iKo)
{
  if (mParams.lock == 0)
    return;

  bool lValue = iKo.value(DPT_Switch);

  if (mParams.lock == 1)
    mLock = lValue;
  return;

  if (mParams.lock == 2)
    mLock = !lValue;
  return;

  // Reset
  mButtonState[0].multiClicks = 0;
  mButtonState[0].multiClickTimer = 0;
  mButtonState[0].press = false;
  mButtonState[0].pressLong = false;
  mButtonState[0].pressExtraLong = false;
  mButtonState[0].pressStart = 0;
  mButtonState[1].multiClicks = 0;
  mButtonState[1].multiClickTimer = 0;
  mButtonState[1].press = false;
  mButtonState[1].pressLong = false;
  mButtonState[1].pressExtraLong = false;
  mButtonState[1].pressStart = 0;

  // SERIAL_DEBUG.printf("BTN::processInputKoLock %i: %i\n\r", mIndex, mLock);
}

void VirtualButton::processInputKoInput(GroupObject &iKo, bool iButton)
{
  bool lNewPress = iKo.value(DPT_Switch);
  bool lLastPress = mButtonState[iButton].press;

  // no state change
  if (lLastPress == lNewPress)
    return;

  if (lNewPress)
  {
    processPress(iButton);
  }
  else
  {
    processRelease(iButton);
  }
}

void VirtualButton::processPressAndHold(bool iButton)
{
  // not pressed
  if (!mButtonState[iButton].press)
    return;

  // no long press configured or not dim or jal
  if (mParams.outputLong == 0 && mParams.outputLong != 7 && mParams.outputLong != 8)
    return;

  if (!mButtonState[iButton].pressLong && delayCheck(mButtonState[iButton].pressStart, mParams.reactionTimeLong))
  {
    eventLongPress(iButton);
    mButtonState[iButton].pressLong = true;
  }

  // no extra long press configured
  if (mParams.outputExtraLong == 0)
    return;

  if (!mButtonState[iButton].pressExtraLong && delayCheck(mButtonState[iButton].pressStart, mParams.reactionTimeExtraLong))
  {
    eventExtraLongPress(iButton);
    mButtonState[iButton].pressExtraLong = true;
  }
}

void VirtualButton::processPress(bool iButton)
{
  if (mLock)
    return;

  if (mParams.mode == 3 && iButton == 0)
  {
    mButtonState[0].multiClicks += 1;
    mButtonState[0].multiClickTimer = millis();
    // SERIAL_DEBUG.printf("  BTN%i/%i: mc press %i\n\r", iButton, mButtonState[0].multiClicks);
  }
  else
  {
    eventShortPress(iButton);
  }

  mButtonState[iButton].press = true;
  mButtonState[iButton].pressLong = false;
  mButtonState[iButton].pressExtraLong = false;
  mButtonState[iButton].pressStart = millis();
}

void VirtualButton::processRelease(bool iButton)
{
  if (mButtonState[iButton].pressExtraLong)
  {
    eventExtraLongRelease(iButton);
  }
  else if (mButtonState[iButton].pressLong)
  {
    eventLongRelease(iButton);
  }
  else if (mParams.mode == 3)
  {
    mButtonState[iButton].multiClickTimer = millis();
  }
  else
  {
    eventShortRelease(iButton);
  }

  // Reset MultiClick on Long or ExtraLong
  if (mButtonState[iButton].pressExtraLong || mButtonState[iButton].pressLong)
  {
    mButtonState[iButton].multiClicks = 0;
    mButtonState[iButton].multiClickTimer = 0;
  }

  // Reset all others
  mButtonState[iButton].press = false;
  mButtonState[iButton].pressLong = false;
  mButtonState[iButton].pressExtraLong = false;
  mButtonState[iButton].pressStart = 0;
}

void VirtualButton::processMultiClick()
{
  // no multiclick mode
  if (mParams.mode != 3)
    return;

  // skip during a press
  if (mButtonState[0].press)
    return;

  // skip when no timer started
  if (mButtonState[0].multiClickTimer == 0)
    return;

  if (delayCheck(mButtonState[0].multiClickTimer, mParams.reactionTimeMultiClick))
  {
    eventMultiClick(mButtonState[0].multiClicks);
    mButtonState[0].multiClickTimer = 0;
    mButtonState[0].multiClicks = 0;
  }
}

void VirtualButton::eventMultiClick(uint8_t iClicks)
{
  if (iClicks > BTN_MaxMuliClicks)
    return;

  uint8_t lIndex = iClicks - 1;
  uint8_t lValue = (uint8_t)mMultiClickParams[lIndex];
  SERIAL_DEBUG.printf("  BTN%i/%i: MultiClick %i clicks - output %i value %i\n\r", mIndex, 0, iClicks, mParams.outputShort, lValue);

  // if special - dpt1
  if (mParams.outputShort == 1)
  {
    // disabled
    if (lValue == 0)
      return;

    bool lValueBool = true;
    if (lValue == 1)
      lValueBool = false;

    getKo(BTN_KoBTNOutput1Multi + lIndex)->value((bool)lValueBool, DPT_Switch);
  }
  else if (mParams.outputShort == 2)
  {
    // disabled
    if (lValue == 0)
      return;

    lValue = (uint8_t)(lValue & 3);
    getKo(BTN_KoBTNOutput1)->value(lValue, DPT_DecimalFactor);
  }
  else if (mParams.outputShort == 4)
  {
    getKo(BTN_KoBTNOutput1)->value(lValue, DPT_DecimalFactor);
  }
  else if (mParams.outputShort == 5)
  {
    getKo(BTN_KoBTNOutput1)->value(lValue, DPT_Scaling);
  }
  else if (mParams.outputShort == 6)
  {
    if (lValue >= 100)
    {
      lValue = (uint8_t)((lValue - 101) | 0x80);
    }
    else
    {
      lValue = (uint8_t)(lValue - 1);
    }
    getKo(BTN_KoBTNOutput1)->value(lValue, DPT_DecimalFactor);
  }
}
void VirtualButton::eventShortPress(bool iButton)
{
  if (mParams.eventShort != 1)
    return;

  SERIAL_DEBUG.printf("  BTN%i/%i: short press\n\r", mIndex, iButton);

  writeSwitchOutput(mParams.outputShort, mButtonParams[iButton].outputShort, mStatusShort, BTN_KoBTNOutput1);
}

void VirtualButton::eventLongPress(bool iButton)
{
  if (mParams.eventLong != 1 && mParams.outputLong != 7 && mParams.outputLong != 8)
    return;

  SERIAL_DEBUG.printf("  BTN%i/%i: long press\n\r", mIndex, iButton);
  if (mParams.outputLong == 7)
  {
    dim(iButton, false);
  }
  else if (mParams.outputLong == 8)
  {
    /* JAL */
  }
  else
  {
    writeSwitchOutput(mParams.outputLong, mButtonParams[iButton].outputLong, mStatusLong, BTN_KoBTNOutput2);
  }
}

void VirtualButton::eventExtraLongPress(bool iButton)
{
  if (mParams.eventExtraLong != 1)
    return;

  SERIAL_DEBUG.printf("  BTN%i/%i: extra long press\n\r", mIndex, iButton);

  writeSwitchOutput(mParams.outputExtraLong, mButtonParams[iButton].outputExtraLong, mStatusExtraLong, BTN_KoBTNOutput3);
}

void VirtualButton::eventShortRelease(bool iButton)
{
  if (mParams.eventShort != 0)
    return;

  SERIAL_DEBUG.printf("  BTN%i/%i: short release\n\r", mIndex, iButton);
  writeSwitchOutput(mParams.outputShort, mButtonParams[iButton].outputShort, mStatusShort, BTN_KoBTNOutput1);
}

void VirtualButton::eventLongRelease(bool iButton)
{
  if (mParams.eventLong != 0 && mParams.outputLong != 7 && mParams.outputLong != 8)
    return;

  SERIAL_DEBUG.printf("  BTN%i/%i: long release\n\r", mIndex, iButton);
  if (mParams.outputLong == 7)
  {
    dim(iButton, true);
  }
  else if (mParams.outputLong == 8)
  {
    /* JAL */
  }
  else
  {
    writeSwitchOutput(mParams.outputLong, mButtonParams[iButton].outputLong, mStatusLong, BTN_KoBTNOutput2);
  }
}

void VirtualButton::eventExtraLongRelease(bool iButton)
{
  SERIAL_DEBUG.printf("  BTN%i/%i: extra long release\n\r", mIndex, iButton);
  if (mParams.eventExtraLong != 0)
    return;

  writeSwitchOutput(mParams.outputExtraLong, mButtonParams[iButton].outputExtraLong, mStatusExtraLong, BTN_KoBTNOutput3);
}

void VirtualButton::writeSwitchOutput(uint8_t iOutput, uint8_t iValue, bool &oStatus, uint8_t iKoOutput)
{
  SERIAL_DEBUG.printf("  BTN%i WRITE %i/%i/%i/%i\n\r", mIndex, iOutput, iValue, oStatus, iKoOutput);

  switch (iOutput)
  {
  case 1:
    // DPT1

    // disabled
    if (iValue == 0)
      return;

    // aus
    if (iValue == 1)
      oStatus = false;

    // an
    if (iValue == 2)
      oStatus = true;

    // toggle
    if (iValue == 3)
    {
      oStatus = !oStatus;
    }

    getKo(iKoOutput)->value((bool)oStatus, DPT_Switch);
    break;

  case 2:
    // DPT2

    // disabled
    if (iValue == 0)
      return;

    getKo(iKoOutput)->value((uint8_t)(iValue & 3), DPT_DecimalFactor);
    break;

  case 4:
    // DPT5
    getKo(iKoOutput)->value((uint8_t)iValue, DPT_DecimalFactor);
    break;

  case 5:
    // DPT5001
    getKo(iKoOutput)->value((uint8_t)iValue, DPT_Scaling);
    break;

  case 6:
    // disabled
    if (iValue == 0)
      return;

    // DPT18
    if (iValue >= 100)
    {
      iValue = (uint8_t)((iValue - 101) | 0x81);
    }
    else
    {
      iValue = (uint8_t)(iValue - 1);
    }
    getKo(iKoOutput)->value(iValue, DPT_DecimalFactor);
    break;
  }
}

void VirtualButton::dim(bool iButton, bool iRelease)
{
  SERIAL_DEBUG.printf("  BTN%i DIM %i/%i\n\r", mIndex, iButton, iRelease);
  if (!iRelease)
  {
    if (mButtonParams[iButton].outputLong == 1)
      mStatusLong = true;

    if (mButtonParams[iButton].outputLong == 2)
      mStatusLong = false;

    if (mButtonParams[iButton].outputLong == 3)
      mStatusLong = !mStatusLong;
  }

  uint8_t lControl = 0x0;
  if (mStatusLong)
    lControl |= 0x8;

  if (!iRelease)
    lControl |= 1;

  // Start Timer for Status Fallback
  mDynamicStatusTimer = iRelease ? millis() : 0;

  // 1. Press   9  1001 Up
  // 2. Release 8  1000 Stop
  // 3. Press   1  0001 Down
  // 4. Release 0  0000 Stop
  SERIAL_DEBUG.printf("    BTN%i DIMSTATUS %i/%i/%i/%i\n\r", mIndex, lControl, mStatusLong, mDynamicStatusTimer);
  getKo(BTN_KoBTNOutput2)->value(lControl, DPT_DecimalFactor);
}
