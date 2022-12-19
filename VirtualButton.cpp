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
  bool differenReactionTime = (knx.paramByte(calcParamIndex(BTN_BTNDifferentReactionTime)) & BTN_BTNDifferentReactionTimeMask) >> BTN_BTNDifferentReactionTimeShift;
  if (differenReactionTime)
  {
    mParams.reactionTimeMultiClick = knx.paramByte(calcParamIndex(BTN_BTNReactionTimeMultiClick)) * 100;
    mParams.reactionTimeLong = knx.paramByte(calcParamIndex(BTN_BTNReactionTimeLong)) * 100;
    mParams.reactionTimeExtraLong = knx.paramByte(calcParamIndex(BTN_BTNReactionTimeExtraLong)) * 100;
  }
  else
  {
    mParams.reactionTimeMultiClick = knx.paramByte(BTN_ReactionTimeMultiClick) * 100;
    mParams.reactionTimeLong = knx.paramByte(BTN_ReactionTimeLong) * 100;
    mParams.reactionTimeExtraLong = knx.paramByte(BTN_ReactionTimeExtraLong) * 100;
  }

  mParams.outputShort = knx.paramByte(calcParamIndex(BTN_BTNOutputShort));
  mParams.outputLong = knx.paramByte(calcParamIndex(BTN_BTNOutputLong));
  mParams.outputExtraLong = knx.paramByte(calcParamIndex(BTN_BTNOutputExtraLong));

  mParams.eventShort = knx.paramByte(calcParamIndex(BTN_BTNOutputShortEvent));
  mParams.eventLong = knx.paramByte(calcParamIndex(BTN_BTNOutputLongEvent));
  mParams.eventExtraLong = knx.paramByte(calcParamIndex(BTN_BTNOutputExtraLongEvent));

  mButtonParams[0].outputShort = knx.paramByte(calcParamIndex(BTN_BTNOutput1ShortDpt1));
  mButtonParams[0].outputLong = knx.paramByte(calcParamIndex(BTN_BTNOutput1LongDpt1));
  mButtonParams[0].outputExtraLong = knx.paramByte(calcParamIndex(BTN_BTNOutput1ExtraLongDpt1));

  mButtonParams[1].outputShort = knx.paramByte(calcParamIndex(BTN_BTNOutput2ShortDpt1));
  mButtonParams[1].outputLong = knx.paramByte(calcParamIndex(BTN_BTNOutput2LongDpt1));
  mButtonParams[1].outputExtraLong = knx.paramByte(calcParamIndex(BTN_BTNOutput2ExtraLongDpt1));

  mMultiClickParams[0] = knx.paramByte(calcParamIndex(BTN_BTNM1Dpt1));
  mMultiClickParams[1] = knx.paramByte(calcParamIndex(BTN_BTNM2Dpt1));
  mMultiClickParams[2] = knx.paramByte(calcParamIndex(BTN_BTNM3Dpt1));

  // Debug
  SERIAL_DEBUG.printf("BTN %i outputShort: %i/%i\n\r", mIndex, mButtonParams[0].outputShort, calcParamIndex(BTN_BTNOutput1ShortDpt1));
  SERIAL_DEBUG.printf("BTN %i outputLong: %i/%i\n\r", mIndex, mButtonParams[0].outputLong, calcParamIndex(BTN_BTNOutput1LongDpt1));
  SERIAL_DEBUG.printf("BTN %i outputExtraLong: %i/%i\n\r", mIndex, mButtonParams[0].outputExtraLong, calcParamIndex(BTN_BTNOutput1ExtraLongDpt1));
  SERIAL_DEBUG.printf("BTN %i eventShort: %i\n\r", mIndex, mParams.eventShort);
  SERIAL_DEBUG.printf("BTN %i eventLong: %i\n\r", mIndex, mParams.eventLong);
  SERIAL_DEBUG.printf("BTN %i eventExtraLong: %i\n\r", mIndex,mParams.eventExtraLong);
  SERIAL_DEBUG.printf("BTN %i reactionTimeMultiClick: %i\n\r", mIndex, mParams.reactionTimeMultiClick);
  SERIAL_DEBUG.printf("BTN %i reactionTimeLong: %i\n\r", mIndex, mParams.reactionTimeLong);
  SERIAL_DEBUG.printf("BTN %i reactionTimeExtraLong: %i\n\r", mIndex,mParams.reactionTimeExtraLong);
  SERIAL_DEBUG.printf("BTN %i mParamMode: %i\n\r", mIndex, mParams.mode);
}

void VirtualButton::loop()
{
  processPressAndHold(0);
  processPressAndHold(1);
  processMultiClick();
}

void VirtualButton::processInputKo(GroupObject &iKo)
{
  if (mParams.mode == 0)
    return;

  switch (calcKoIndex(iKo.asap()))
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
    processInputKoStatus(iKo, 1, mParams.statusShort);
    break;
  case BTN_KoBTNOutput2Status:
    processInputKoStatus(iKo, 2, mParams.statusLong);
    break;
  case BTN_KoBTNOutput3Status:
    processInputKoStatus(iKo, 3, mParams.statusExtraLong);
    break;
  }
}

void VirtualButton::processInputKoStatus(GroupObject &iKo, uint8_t iStatusNumber, bool &oStatus) {
  SERIAL_DEBUG.printf("BTN::processInputKoStatus %i: %i/%i\n\r", mIndex, iStatusNumber, oStatus);

  // Special for Long DPT3007
  if (iStatusNumber == 2 && mParams.outputLong == 7) {
    uint8_t lValue = iKo.value(getDPT(VAL_DPT_5001));
    if (lValue == 0 && oStatus)
      oStatus = false;
    if (lValue == 100 && !oStatus)
      oStatus = true;

  } else {
    bool lValue = iKo.value(getDPT(VAL_DPT_1));
    oStatus = lValue;
  }
}

void VirtualButton::processInputKoLock(GroupObject &iKo)
{
  bool lValue = iKo.value(getDPT(VAL_DPT_1));
  // SERIAL_DEBUG.printf("BTN::processInputKoLock %i: %i\n\r", mIndex, lValue);
}

void VirtualButton::processInputKoInput(GroupObject &iKo, bool iButton)
{
  bool lNewPress = iKo.value(getDPT(VAL_DPT_1));
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

  if (!mButtonState[iButton].pressLong && (millis() - mButtonState[iButton].pressStart) > mParams.reactionTimeLong)
  {
    eventLongPress(iButton);
    mButtonState[iButton].pressLong = true;
  }

  // no extra long press configured
  if (mParams.outputExtraLong == 0)
    return;

  if (!mButtonState[iButton].pressExtraLong && (millis() - mButtonState[iButton].pressStart) > mParams.reactionTimeExtraLong)
  {
    eventExtraLongPress(iButton);
    mButtonState[iButton].pressExtraLong = true;
  }
}

void VirtualButton::processPress(bool iButton)
{
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

  if (millis() - mButtonState[0].multiClickTimer > mParams.reactionTimeMultiClick)
  {
    eventMultiClick(mButtonState[0].multiClicks);
    mButtonState[0].multiClickTimer = 0;
    mButtonState[0].multiClicks = 0;
  }
}

void VirtualButton::eventMultiClick(uint8_t iClicks)
{
  SERIAL_DEBUG.printf("  BTN%i/%i: MultiClick %i clicks - output %i\n\r", mIndex, 0, iClicks, mParams.outputShort);
  if (iClicks > BTN_MaxMuliClicks)
    return;

  // if special - dpt1
  if (mParams.outputShort == 1)
  {
    uint8_t lIndex = iClicks - 1;
  SERIAL_DEBUG.printf("    1\n\r", mIndex);

    // disabled
    if (mMultiClickParams[lIndex] == 0)
      return;
  SERIAL_DEBUG.printf("    2\n\r", mIndex);

    bool lValue = true;
    if (mMultiClickParams[lIndex] == 1)
      lValue = false;
  SERIAL_DEBUG.printf("    3 %i\n\r", mIndex, lValue);

    getKo(BTN_KoBTNOutput1Multi + lIndex)->value((bool)lValue, getDPT(VAL_DPT_1));
  }
  else if (mParams.outputShort == 4)
  {
    getKo(BTN_KoBTNOutput1)->value((uint8_t)mParams.outputShort, getDPT(VAL_DPT_5));
  }
  else if (mParams.outputShort == 5)
  {
    getKo(BTN_KoBTNOutput1)->value((uint8_t)mParams.outputShort, getDPT(VAL_DPT_5001));
  }
  else if (mParams.outputShort == 6)
  {
    getKo(BTN_KoBTNOutput1)->value((uint8_t)(mParams.outputShort - 1), getDPT(VAL_DPT_17));
  }
}
void VirtualButton::eventShortPress(bool iButton)
{
  if (mParams.eventShort != 1)
    return;

  SERIAL_DEBUG.printf("  BTN%i/%i: short press\n\r", mIndex, iButton);

  writeSwitchOutput(mParams.outputShort, mButtonParams[iButton].outputShort, mParams.statusShort, BTN_KoBTNOutput1);
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
    writeSwitchOutput(mParams.outputLong, mButtonParams[iButton].outputLong, mParams.statusLong, BTN_KoBTNOutput2);
  }
}
void VirtualButton::eventExtraLongPress(bool iButton)
{
  if (mParams.eventExtraLong != 1)
    return;

  SERIAL_DEBUG.printf("  BTN%i/%i: extra long press\n\r", mIndex, iButton);

  writeSwitchOutput(mParams.outputExtraLong, mButtonParams[iButton].outputExtraLong, mParams.statusExtraLong, BTN_KoBTNOutput3);
}
void VirtualButton::eventShortRelease(bool iButton)
{
  if (mParams.eventShort != 0)
    return;

  SERIAL_DEBUG.printf("  BTN%i/%i: short release\n\r", mIndex, iButton);
  writeSwitchOutput(mParams.outputShort, mButtonParams[iButton].outputShort, mParams.statusShort, BTN_KoBTNOutput1);
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
    writeSwitchOutput(mParams.outputLong, mButtonParams[iButton].outputLong, mParams.statusLong, BTN_KoBTNOutput2);
  }
}
void VirtualButton::eventExtraLongRelease(bool iButton)
{
  SERIAL_DEBUG.printf("  BTN%i/%i: extra long release\n\r", mIndex, iButton);
  if (mParams.eventExtraLong != 0)
    return;

  writeSwitchOutput(mParams.outputExtraLong, mButtonParams[iButton].outputExtraLong, mParams.statusExtraLong, BTN_KoBTNOutput3);
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

    getKo(iKoOutput)->value((bool)oStatus, getDPT(VAL_DPT_1));
    break;

  case 4:
    // DPT5
    getKo(iKoOutput)->value((uint8_t)iValue, getDPT(VAL_DPT_5));
    break;

  case 5:
    // DPT5001
    getKo(iKoOutput)->value((uint8_t)iValue, getDPT(VAL_DPT_5001));
    break;

  case 6:
    // disabled
    if (iValue == 0)
      return;

    // DPT17
    getKo(iKoOutput)->value((uint8_t)(iValue - 1), getDPT(VAL_DPT_17));
    break;
  }
}

void VirtualButton::dim(bool iButton, bool iRelease)
{
  SERIAL_DEBUG.printf("  BTN%i DIM %i/%i\n\r", mIndex, iButton, iRelease);
  if (!iRelease)
  {
    if (mButtonParams[iButton].outputLong == 1)
      mParams.statusLong = true;

    if (mButtonParams[iButton].outputLong == 2)
      mParams.statusLong = false;

    if (mButtonParams[iButton].outputLong == 3)
      mParams.statusLong = !mParams.statusLong;
  }

  uint8_t lControl = 0x0;
  if (mParams.statusLong)
    lControl |= 0x8;

  if (!iRelease)
    lControl |= 1;

  // 1. Press   9  1001 Up
  // 2. Release 8  1000 Stop
  // 3. Press   1  0001 Down
  // 4. Release 0  0000 Stop
  SERIAL_DEBUG.printf("    BTN%i DIMSTATUS %i/%i/%i\n\r", mIndex, lControl, mParams.statusLong);
  getKo(BTN_KoBTNOutput2)->value(lControl, getDPT(VAL_DPT_5));
}
