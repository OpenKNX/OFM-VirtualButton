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
  mParams.mode = (knx.paramByte(calcParamIndex(BTN_BTNMode)) & BTN_BTNModeMask) >> BTN_BTNModeShift;
  mParams.lock = (knx.paramByte(calcParamIndex(BTN_BTNLock)) & BTN_BTNLockMask) >> BTN_BTNLockShift;

  // Input
  mButtonParams[0].inputKo = knx.paramWord(calcParamIndex(BTN_BTNInputA));
  mButtonParams[1].inputKo = knx.paramWord(calcParamIndex(BTN_BTNInputB));

  // Output 1
  //   DPT
  mParams.outputShortDpt = knx.paramByte(calcParamIndex(BTN_BTNOutputShort_DPT));
  mParams.outputLongDpt = knx.paramByte(calcParamIndex(BTN_BTNOutputLong_DPT));
  mParams.outputExtraLongDpt = knx.paramByte(calcParamIndex(BTN_BTNOutputExtraLong_DPT));

  //   Events
  mButtonParams[0].outputShortPressActive = (knx.paramByte(calcParamIndex(BTN_BTNOutputShort_Taster1_Active_Press)) & BTN_BTNOutputShort_Taster1_Active_PressMask) >> BTN_BTNOutputShort_Taster1_Active_PressShift;
  mButtonParams[0].outputShortReleaseActive = (knx.paramByte(calcParamIndex(BTN_BTNOutputShort_Taster1_Active_Release)) & BTN_BTNOutputShort_Taster1_Active_ReleaseMask) >> BTN_BTNOutputShort_Taster1_Active_ReleaseShift;
  mButtonParams[1].outputShortPressActive = (knx.paramByte(calcParamIndex(BTN_BTNOutputShort_Taster2_Active_Press)) & BTN_BTNOutputShort_Taster2_Active_PressMask) >> BTN_BTNOutputShort_Taster2_Active_PressShift;
  mButtonParams[1].outputShortReleaseActive = (knx.paramByte(calcParamIndex(BTN_BTNOutputShort_Taster2_Active_Release)) & BTN_BTNOutputShort_Taster2_Active_ReleaseMask) >> BTN_BTNOutputShort_Taster2_Active_ReleaseShift;

  mButtonParams[0].outputLongPressActive = (knx.paramByte(calcParamIndex(BTN_BTNOutputLong_Taster1_Active_Press)) & BTN_BTNOutputLong_Taster1_Active_PressMask) >> BTN_BTNOutputLong_Taster1_Active_PressShift;
  mButtonParams[0].outputLongReleaseActive = (knx.paramByte(calcParamIndex(BTN_BTNOutputLong_Taster1_Active_Release)) & BTN_BTNOutputLong_Taster1_Active_ReleaseMask) >> BTN_BTNOutputLong_Taster1_Active_ReleaseShift;
  mButtonParams[1].outputLongPressActive = (knx.paramByte(calcParamIndex(BTN_BTNOutputLong_Taster2_Active_Press)) & BTN_BTNOutputLong_Taster2_Active_PressMask) >> BTN_BTNOutputLong_Taster2_Active_PressShift;
  mButtonParams[1].outputLongReleaseActive = (knx.paramByte(calcParamIndex(BTN_BTNOutputLong_Taster2_Active_Release)) & BTN_BTNOutputLong_Taster2_Active_ReleaseMask) >> BTN_BTNOutputLong_Taster2_Active_ReleaseShift;

  mButtonParams[0].outputExtraLongPressActive = (knx.paramByte(calcParamIndex(BTN_BTNOutputExtraLong_Taster1_Active_Press)) & BTN_BTNOutputExtraLong_Taster1_Active_PressMask) >> BTN_BTNOutputExtraLong_Taster1_Active_PressShift;
  mButtonParams[0].outputExtraLongReleaseActive = (knx.paramByte(calcParamIndex(BTN_BTNOutputExtraLong_Taster1_Active_Release)) & BTN_BTNOutputExtraLong_Taster1_Active_ReleaseMask) >> BTN_BTNOutputExtraLong_Taster1_Active_ReleaseShift;
  mButtonParams[1].outputExtraLongPressActive = (knx.paramByte(calcParamIndex(BTN_BTNOutputExtraLong_Taster2_Active_Press)) & BTN_BTNOutputExtraLong_Taster2_Active_PressMask) >> BTN_BTNOutputExtraLong_Taster2_Active_PressShift;
  mButtonParams[1].outputExtraLongReleaseActive = (knx.paramByte(calcParamIndex(BTN_BTNOutputExtraLong_Taster2_Active_Release)) & BTN_BTNOutputExtraLong_Taster2_Active_ReleaseMask) >> BTN_BTNOutputExtraLong_Taster2_Active_ReleaseShift;

  //   Outputs - DPT1 ist stellvertretend für alle DPTs
  mButtonParams[0].outputShortPress = knx.paramWord(calcParamIndex(BTN_BTNOutputShort_Taster1_Dpt1_Press));
  mButtonParams[0].outputShortRelease = knx.paramWord(calcParamIndex(BTN_BTNOutputShort_Taster1_Dpt1_Release));
  mButtonParams[1].outputShortPress = knx.paramWord(calcParamIndex(BTN_BTNOutputShort_Taster2_Dpt1_Press));
  mButtonParams[1].outputShortRelease = knx.paramWord(calcParamIndex(BTN_BTNOutputShort_Taster2_Dpt1_Release));

  mButtonParams[0].outputLongPress = knx.paramWord(calcParamIndex(BTN_BTNOutputLong_Taster1_Dpt1_Press));
  mButtonParams[0].outputLongRelease = knx.paramWord(calcParamIndex(BTN_BTNOutputLong_Taster1_Dpt1_Release));
  mButtonParams[1].outputLongPress = knx.paramWord(calcParamIndex(BTN_BTNOutputLong_Taster2_Dpt1_Press));
  mButtonParams[1].outputLongRelease = knx.paramWord(calcParamIndex(BTN_BTNOutputLong_Taster2_Dpt1_Release));

  mButtonParams[0].outputExtraLongPress = knx.paramWord(calcParamIndex(BTN_BTNOutputExtraLong_Taster1_Dpt1_Press));
  mButtonParams[0].outputExtraLongRelease = knx.paramWord(calcParamIndex(BTN_BTNOutputExtraLong_Taster1_Dpt1_Release));
  mButtonParams[1].outputExtraLongPress = knx.paramWord(calcParamIndex(BTN_BTNOutputExtraLong_Taster2_Dpt1_Press));
  mButtonParams[1].outputExtraLongRelease = knx.paramWord(calcParamIndex(BTN_BTNOutputExtraLong_Taster2_Dpt1_Release));

  // Output 2 (immer DPT2)
  mButtonParams[0].output2Short = (knx.paramByte(calcParamIndex(BTN_BTNOutput2Short_Taster1)) & BTN_BTNOutput2Short_Taster1Mask) >> BTN_BTNOutput2Short_Taster1Shift;
  mButtonParams[1].output2Short = (knx.paramByte(calcParamIndex(BTN_BTNOutput2Short_Taster2)) & BTN_BTNOutput2Short_Taster2Mask) >> BTN_BTNOutput2Short_Taster2Shift;
  mButtonParams[0].output2Long = (knx.paramByte(calcParamIndex(BTN_BTNOutput2Long_Taster1)) & BTN_BTNOutput2Long_Taster1Mask) >> BTN_BTNOutput2Long_Taster1Shift;
  mButtonParams[1].output2Long = (knx.paramByte(calcParamIndex(BTN_BTNOutput2Long_Taster2)) & BTN_BTNOutput2Long_Taster2Mask) >> BTN_BTNOutput2Long_Taster2Shift;
  mButtonParams[0].output2ExtraLong = (knx.paramByte(calcParamIndex(BTN_BTNOutput2ExtraLong_Taster1)) & BTN_BTNOutput2ExtraLong_Taster1Mask) >> BTN_BTNOutput2ExtraLong_Taster1Shift;
  mButtonParams[1].output2ExtraLong = (knx.paramByte(calcParamIndex(BTN_BTNOutput2ExtraLong_Taster2)) & BTN_BTNOutput2ExtraLong_Taster2Mask) >> BTN_BTNOutput2ExtraLong_Taster2Shift;

  // MultiClick
  mMultiClickParams[0].active = (knx.paramByte(calcParamIndex(BTN_BTNOutputMulti_Click1_Active)) & BTN_BTNOutputMulti_Click1_ActiveMask) >> BTN_BTNOutputMulti_Click1_ActiveShift;
  mMultiClickParams[1].active = (knx.paramByte(calcParamIndex(BTN_BTNOutputMulti_Click2_Active)) & BTN_BTNOutputMulti_Click2_ActiveMask) >> BTN_BTNOutputMulti_Click2_ActiveShift;
  mMultiClickParams[2].active = (knx.paramByte(calcParamIndex(BTN_BTNOutputMulti_Click3_Active)) & BTN_BTNOutputMulti_Click3_ActiveMask) >> BTN_BTNOutputMulti_Click3_ActiveShift;

  // DPT2 ist stellvertretend für alle DPTs (DPT1 nicht ntuzbar da für die einzeldklicks in Verwendung)
  mMultiClickParams[0].output = knx.paramWord(calcParamIndex(BTN_BTNOutputMulti_Click1_Dpt2)); 
  mMultiClickParams[1].output = knx.paramWord(calcParamIndex(BTN_BTNOutputMulti_Click2_Dpt2));
  mMultiClickParams[2].output = knx.paramWord(calcParamIndex(BTN_BTNOutputMulti_Click3_Dpt2));

  // ReactionTimes
  mParams.reactionTimeMultiClick = knx.paramByte(calcParamIndex(BTN_BTNReactionTimeMultiClick));
  if (mParams.reactionTimeMultiClick > 0)
    mParams.reactionTimeMultiClick *= 100;
  else
    mParams.reactionTimeMultiClick = knx.paramByte(BTN_ReactionTimeMultiClick) * 100;

  if (mParams.reactionTimeLong > 0)
    mParams.reactionTimeLong *= 100;
  else
    mParams.reactionTimeLong = knx.paramByte(BTN_ReactionTimeLong) * 100;

  if (mParams.reactionTimeExtraLong > 0)
    mParams.reactionTimeExtraLong *= 100;
  else
    mParams.reactionTimeExtraLong = knx.paramByte(BTN_ReactionTimeExtraLong) * 100;

  mParams.dynamicStatusThreshold = knx.paramByte(calcParamIndex(BTN_BTNStatusThreshold));
  mParams.dynamicStatusFallback = (getDelayPattern(calcParamIndex(BTN_BTNStatusFallbackBase)));

  mDynamicStatusTimer = millis() + mParams.dynamicStatusFallback;

  // Debug
  // SERIAL_DEBUG.printf("BTN %i inputKo: %i/%i\n\r", mIndex, 0, mButtonParams[0].inputKo);
  // SERIAL_DEBUG.printf("BTN %i outputShortPressActive: %i/%i\n\r", mIndex, 0, mButtonParams[0].outputShortPressActive);
  // SERIAL_DEBUG.printf("BTN %i outputShortReleaseActive: %i/%i\n\r", mIndex, 0, mButtonParams[0].outputShortReleaseActive);
  // SERIAL_DEBUG.printf("BTN %i outputLongPressActive: %i/%i\n\r", mIndex, 0, mButtonParams[0].outputLongPressActive);
  // SERIAL_DEBUG.printf("BTN %i outputLongReleaseActive: %i/%i\n\r", mIndex, 0, mButtonParams[0].outputLongReleaseActive);
  // SERIAL_DEBUG.printf("BTN %i outputExtraLongPressActive: %i/%i\n\r", mIndex, 0, mButtonParams[0].outputExtraLongPressActive);
  // SERIAL_DEBUG.printf("BTN %i outputExtraLongReleaseActive: %i/%i\n\r", mIndex, 0, mButtonParams[0].outputExtraLongReleaseActive);

  // SERIAL_DEBUG.printf("BTN %i inputKo: %i/%i\n\r", mIndex, 1, mButtonParams[1].inputKo);
  // SERIAL_DEBUG.printf("BTN %i outputShortPressActive: %i/%i\n\r", mIndex, 1, mButtonParams[1].outputShortPressActive);
  // SERIAL_DEBUG.printf("BTN %i outputShortReleaseActive: %i/%i\n\r", mIndex, 1, mButtonParams[1].outputShortReleaseActive);
  // SERIAL_DEBUG.printf("BTN %i outputLongPressActive: %i/%i\n\r", mIndex, 1, mButtonParams[1].outputLongPressActive);
  // SERIAL_DEBUG.printf("BTN %i outputLongReleaseActive: %i/%i\n\r", mIndex, 1, mButtonParams[1].outputLongReleaseActive);
  // SERIAL_DEBUG.printf("BTN %i outputExtraLongPressActive: %i/%i\n\r", mIndex, 1, mButtonParams[1].outputExtraLongPressActive);
  // SERIAL_DEBUG.printf("BTN %i outputExtraLongReleaseActive: %i/%i\n\r", mIndex, 1, mButtonParams[1].outputExtraLongReleaseActive);

  // SERIAL_DEBUG.printf("BTN %i outputShortPress: %i/%i\n\r", mIndex, 0, mButtonParams[0].outputShortPress);
  // SERIAL_DEBUG.printf("BTN %i outputShortRelease: %i/%i\n\r", mIndex, 0, mButtonParams[0].outputShortRelease);
  // SERIAL_DEBUG.printf("BTN %i outputLongPress: %i/%i\n\r", mIndex, 0, mButtonParams[0].outputLongPress);
  // SERIAL_DEBUG.printf("BTN %i outputLongRelease: %i/%i\n\r", mIndex, 0, mButtonParams[0].outputLongRelease);
  // SERIAL_DEBUG.printf("BTN %i outputExtraLongPress: %i/%i\n\r", mIndex, 0, mButtonParams[0].outputExtraLongPress);
  // SERIAL_DEBUG.printf("BTN %i outputExtraLongRelease: %i/%i\n\r", mIndex, 0, mButtonParams[0].outputExtraLongRelease);

  // SERIAL_DEBUG.printf("BTN %i outputShortPress: %i/%i\n\r", mIndex, 1, mButtonParams[1].outputShortPress);
  // SERIAL_DEBUG.printf("BTN %i outputShortRelease: %i/%i\n\r", mIndex, 1, mButtonParams[1].outputShortRelease);
  // SERIAL_DEBUG.printf("BTN %i outputLongPress: %i/%i\n\r", mIndex, 1, mButtonParams[1].outputLongPress);
  // SERIAL_DEBUG.printf("BTN %i outputLongRelease: %i/%i\n\r", mIndex, 1, mButtonParams[1].outputLongRelease);
  // SERIAL_DEBUG.printf("BTN %i outputExtraLongPress: %i/%i\n\r", mIndex, 1, mButtonParams[1].outputExtraLongPress);
  // SERIAL_DEBUG.printf("BTN %i outputExtraLongRelease: %i/%i\n\r", mIndex, 1, mButtonParams[1].outputExtraLongRelease);
  
  // SERIAL_DEBUG.printf("BTN %i output2Short: %i/%i\n\r", mIndex, 2, mButtonParams[0].output2Short);
  // SERIAL_DEBUG.printf("BTN %i output2Short: %i/%i\n\r", mIndex, 1, mButtonParams[1].output2Short);


  // SERIAL_DEBUG.printf("BTN %i MC Active: %i/%i\n\r", mIndex, 0, mMultiClickParams[0].active);
  // SERIAL_DEBUG.printf("BTN %i MC Output: %i/%i\n\r", mIndex, 0, mMultiClickParams[0].output);
  // SERIAL_DEBUG.printf("BTN %i MC Active: %i/%i\n\r", mIndex, 1, mMultiClickParams[1].active);
  // SERIAL_DEBUG.printf("BTN %i MC Output: %i/%i\n\r", mIndex, 1, mMultiClickParams[1].output);
  // SERIAL_DEBUG.printf("BTN %i MC Active: %i/%i\n\r", mIndex, 2, mMultiClickParams[2].active);
  // SERIAL_DEBUG.printf("BTN %i MC Output: %i/%i\n\r", mIndex, 2, mMultiClickParams[2].output);
  
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
  // if (mDynamicStatusTimer > 0 && delayCheck(mDynamicStatusTimer, mParams.dynamicStatusFallback))
  // {
  //   uint8_t lValue = getKo(BTN_KoBTNOutput2Status)->value(DPT_Scaling);
  //   mStatusLong = (lValue < mParams.dynamicStatusThreshold) ? false : true;
  //   SERIAL_DEBUG.printf("BTN::processDynamicStatus %i/%i/%i: triggered\n\r", mIndex, lValue, mStatusLong);

  //   mDynamicStatusTimer = 0;
  // }
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

  // // Special for Long DPT3007
  // if (iStatusNumber == 2 && mParams.outputLong == 7)
  // {
  //   uint8_t lValue = iKo.value(DPT_Scaling);
  //   if (lValue == 0 && oStatus)
  //     oStatus = false;
  //   if (lValue == 100 && !oStatus)
  //     oStatus = true;
  // }
  // else
  // {
    bool lValue = iKo.value(DPT_Switch);
    oStatus = lValue;
  // }
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

  // // SERIAL_DEBUG.printf("BTN::processInputKoLock %i: %i\n\r", mIndex, mLock);
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
  //if (mParams.outputLong == 0 && mParams.outputLong != 7 && mParams.outputLong != 8)
  //  return;

  if (!mButtonState[iButton].pressLong && delayCheck(mButtonState[iButton].pressStart, mParams.reactionTimeLong))
  {
    eventLongPress(iButton);
    mButtonState[iButton].pressLong = true;
  }

  // no extra long press configured
  if (mParams.outputExtraLongDpt == 0)
    return;

  if (!mButtonState[iButton].pressExtraLong && delayCheck(mButtonState[iButton].pressStart, mParams.reactionTimeExtraLong))
  {
    eventExtraLongPress(iButton);
    mButtonState[iButton].pressExtraLong = true;
  }
}

void VirtualButton::processPress(bool iButton)
{
  SERIAL_DEBUG.printf("  BTN%i/%i: processPress %i/%i\n\r", iButton, mParams.mode);
  if (mLock)
    return;

  if (mParams.mode == 3 && iButton == 0)
  {
    mButtonState[0].multiClicks += 1;
    mButtonState[0].multiClickTimer = millis();
    SERIAL_DEBUG.printf("  BTN%i/%i: mc press %i\n\r", iButton, mButtonState[0].multiClicks);
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
  SERIAL_DEBUG.printf("  BTN%i/%i: MultiClick %i clicks\n\r", mIndex, 0, iClicks);

  uint8_t lIndex = iClicks - 1;
  sMultiClickParams lParams = mMultiClickParams[lIndex];

  if (!lParams.active)
    return;

  SERIAL_DEBUG.printf("  BTN%i/%i: MultiClick %i clicks - output %i value %i\n\r", mIndex, 0, iClicks, mParams.outputShortDpt, lParams.output);

  // // if special - dpt1
  // if (mParams.outputShort == 1)
  // {
  //   // disabled
  //   if (lValue == 0)
  //     return;

  //   bool lValueBool = true;
  //   if (lValue == 1)
  //     lValueBool = false;

  //   getKo(BTN_KoBTNOutput1Multi + lIndex)->value((bool)lValueBool, DPT_Switch);
  // }
  // else if (mParams.outputShort == 2)
  // {
  //   // disabled
  //   if (lValue == 0)
  //     return;

  //   lValue = (uint8_t)(lValue & 3);
  //   getKo(BTN_KoBTNOutput1)->value(lValue, DPT_DecimalFactor);
  // }
  // else if (mParams.outputShort == 4)
  // {
  //   getKo(BTN_KoBTNOutput1)->value(lValue, DPT_DecimalFactor);
  // }
  // else if (mParams.outputShort == 5)
  // {
  //   getKo(BTN_KoBTNOutput1)->value(lValue, DPT_Scaling);
  // }
  // else if (mParams.outputShort == 6)
  // {
  //   if (lValue >= 100)
  //   {
  //     lValue = (uint8_t)((lValue - 101) | 0x80);
  //   }
  //   else
  //   {
  //     lValue = (uint8_t)(lValue - 1);
  //   }
  //   getKo(BTN_KoBTNOutput1)->value(lValue, DPT_DecimalFactor);
  // }
}
void VirtualButton::eventShortPress(bool iButton)
{
  if (!mButtonParams[iButton].outputShortPressActive)
    return;

  SERIAL_DEBUG.printf("  BTN%i/%i: short press\n\r", mIndex, iButton);

  // writeSwitchOutput(mParams.outputShort, mButtonParams[iButton].outputShort, mStatusShort, BTN_KoBTNOutput1);
}

void VirtualButton::eventLongPress(bool iButton)
{
  if (!mButtonParams[iButton].outputLongPressActive)
    return;

  SERIAL_DEBUG.printf("  BTN%i/%i: long press\n\r", mIndex, iButton);
  // if (mParams.outputLong == 7)
  // {
  //   dim(iButton, false);
  // }
  // else if (mParams.outputLong == 8)
  // {
  //   /* JAL */
  // }
  // else
  // {
  //
  // }
  // writeSwitchOutput(mParams.outputLong, mButtonParams[iButton].outputLong, mStatusLong, BTN_KoBTNOutput2);
}

void VirtualButton::eventExtraLongPress(bool iButton)
{
  if (!mButtonParams[iButton].outputExtraLongPressActive)
    return;

  SERIAL_DEBUG.printf("  BTN%i/%i: extra long press\n\r", mIndex, iButton);

  // writeSwitchOutput(mParams.outputExtraLong, mButtonParams[iButton].outputExtraLong, mStatusExtraLong, BTN_KoBTNOutput3);
}

void VirtualButton::eventShortRelease(bool iButton)
{
  if (!mButtonParams[iButton].outputShortReleaseActive && !mButtonParams[iButton].output2Short)
    return;

  SERIAL_DEBUG.printf("  BTN%i/%i: short release\n\r", mIndex, iButton);
  // writeSwitchOutput(mParams.outputShort, mButtonParams[iButton].outputShort, mStatusShort, BTN_KoBTNOutput1);
}

void VirtualButton::eventLongRelease(bool iButton)
{
  if (!mButtonParams[iButton].outputLongReleaseActive && !mButtonParams[iButton].output2Long)
    return;

  SERIAL_DEBUG.printf("  BTN%i/%i: long release\n\r", mIndex, iButton);
  // if (mParams.outputLong == 7)
  // {
  //   dim(iButton, true);
  // }
  // else if (mParams.outputLong == 8)
  // {
  //   /* JAL */
  // }
  // else
  // {
  //   writeSwitchOutput(mParams.outputLong, mButtonParams[iButton].outputLong, mStatusLong, BTN_KoBTNOutput2);
  // }
}

void VirtualButton::eventExtraLongRelease(bool iButton)
{
  if (!mButtonParams[iButton].outputExtraLongReleaseActive && !mButtonParams[iButton].output2ExtraLong)
    return;

  SERIAL_DEBUG.printf("  BTN%i/%i: extra long release\n\r", mIndex, iButton);

  // writeSwitchOutput(mParams.outputExtraLong, mButtonParams[iButton].outputExtraLong, mStatusExtraLong, BTN_KoBTNOutput3);
}

void VirtualButton::writeSwitchOutput(uint8_t iOutput, uint8_t iValue, bool &oStatus, uint8_t iKoOutput)
{
  SERIAL_DEBUG.printf("  BTN%i WRITE %i/%i/%i/%i\n\r", mIndex, iOutput, iValue, oStatus, iKoOutput);

  // switch (iOutput)
  // {
  // case 1:
  //   // DPT1

  //   // disabled
  //   if (iValue == 0)
  //     return;

  //   // aus
  //   if (iValue == 1)
  //     oStatus = false;

  //   // an
  //   if (iValue == 2)
  //     oStatus = true;

  //   // toggle
  //   if (iValue == 3)
  //   {
  //     oStatus = !oStatus;
  //   }

  //   getKo(iKoOutput)->value((bool)oStatus, DPT_Switch);
  //   break;

  // case 2:
  //   // DPT2

  //   // disabled
  //   if (iValue == 0)
  //     return;

  //   getKo(iKoOutput)->value((uint8_t)(iValue & 3), DPT_DecimalFactor);
  //   break;

  // case 4:
  //   // DPT5
  //   getKo(iKoOutput)->value((uint8_t)iValue, DPT_DecimalFactor);
  //   break;

  // case 5:
  //   // DPT5001
  //   getKo(iKoOutput)->value((uint8_t)iValue, DPT_Scaling);
  //   break;

  // case 6:
  //   // disabled
  //   if (iValue == 0)
  //     return;

  //   // DPT18
  //   if (iValue >= 100)
  //   {
  //     iValue = (uint8_t)((iValue - 101) | 0x81);
  //   }
  //   else
  //   {
  //     iValue = (uint8_t)(iValue - 1);
  //   }
  //   getKo(iKoOutput)->value(iValue, DPT_DecimalFactor);
  //   break;
  // }
}

// void VirtualButton::dim(bool iButton, bool iRelease)
// {
//   SERIAL_DEBUG.printf("  BTN%i DIM %i/%i\n\r", mIndex, iButton, iRelease);
//   if (!iRelease)
//   {
//     if (mButtonParams[iButton].outputLong == 1)
//       mStatusLong = true;

//     if (mButtonParams[iButton].outputLong == 2)
//       mStatusLong = false;

//     if (mButtonParams[iButton].outputLong == 3)
//       mStatusLong = !mStatusLong;
//   }

//   uint8_t lControl = 0x0;
//   if (mStatusLong)
//     lControl |= 0x8;

//   if (!iRelease)
//     lControl |= 1;

//   // Start Timer for Status Fallback
//   mDynamicStatusTimer = iRelease ? millis() : 0;

//   // 1. Press   9  1001 Up
//   // 2. Release 8  1000 Stop
//   // 3. Press   1  0001 Down
//   // 4. Release 0  0000 Stop
//   SERIAL_DEBUG.printf("    BTN%i DIMSTATUS %i/%i/%i/%i\n\r", mIndex, lControl, mStatusLong, mDynamicStatusTimer);
//   getKo(BTN_KoBTNOutput2)->value(lControl, DPT_DecimalFactor);
// }
