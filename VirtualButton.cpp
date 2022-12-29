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
  mParams.multiClickCount = (knx.paramByte(calcParamIndex(BTN_BTNMultiClickCount)) & BTN_BTNMultiClickCountMask) >> BTN_BTNMultiClickCountShift;

  // Input
  mButtonParams[0].inputKo = knx.paramWord(calcParamIndex(BTN_BTNInputA));
  mButtonParams[1].inputKo = knx.paramWord(calcParamIndex(BTN_BTNInputB));

  // Output 1
  //   DPT
  mParams.outputShortDpt = knx.paramByte(calcParamIndex(BTN_BTNOutputShort_DPT));
  mParams.outputLongDpt = knx.paramByte(calcParamIndex(BTN_BTNOutputLong_DPT));
  mParams.outputExtraLongDpt = knx.paramByte(calcParamIndex(BTN_BTNOutputExtraLong_DPT));
  mParams.outputMultiClickDpt = knx.paramByte(calcParamIndex(BTN_BTNOutputMulti_DPT));

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

  // Output 2
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
  mMultiClickParams[0].output = knx.paramWord(calcParamIndex(BTN_BTNOutputMulti_Click1_Dpt1));
  mMultiClickParams[1].output = knx.paramWord(calcParamIndex(BTN_BTNOutputMulti_Click2_Dpt1));
  mMultiClickParams[2].output = knx.paramWord(calcParamIndex(BTN_BTNOutputMulti_Click3_Dpt1));

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

  // Debug
  SERIAL_DEBUG.printf("BTN %i dynamicStatusThreshold: %i\n\r", mIndex, mParams.dynamicStatusThreshold);
  SERIAL_DEBUG.printf("BTN %i dynamicStatusFallback: %i\n\r", mIndex, mParams.dynamicStatusFallback);
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
  processDynamicStatusTimer();
}
void VirtualButton::processDynamicStatusTimer()
{
  // Es ist überhaupt kein dynamischer Status aktiv
  if (mParams.dynamicStatusFallback == 0)
    return;

  if (mDynamicStatusTimer > 0 && delayCheck(mDynamicStatusTimer, mParams.dynamicStatusFallback))
  {
    mDynamicStatusTimer = 0;
    evaluateDynamicStatus();
  }
}

/*
  Nach jedem Tastendruck (3007/3008) wird ein Timer gestartet.
  Läuft dieser ab (processDynamicStatusTimer) wird der interne Status neu evaluiert.
  Ist keine Timer gestartet, so wird immer neu evaluiert (processInputKoStatus)
*/
void VirtualButton::evaluateDynamicStatus() {
  // Läuft gerade ein Timer, so ist keine Änderung erlaubt.
  if(mDynamicStatusTimer > 0)
    return;

  // Es ist überhaupt kein dynamischer Status aktiv
  if (mParams.dynamicStatusFallback == 0)
    return;

  SERIAL_DEBUG.printf("BTN %i: evaluateDynamicStatus\n\r", mIndex);

  // Short
  if (mParams.outputShortDpt == 7 || mParams.outputShortDpt == 8) {
    uint8_t lValue = getKo(BTN_KoBTNOutput1Status)->value(DPT_Scaling);
    mStatusShort = (lValue < mParams.dynamicStatusThreshold) ? false : true;
    SERIAL_DEBUG.printf("    short: %i/%i/%i\n\r", lValue, mParams.dynamicStatusThreshold, mStatusShort);
  }
  // Long
  if (mParams.outputLongDpt == 7 || mParams.outputLongDpt == 8) {
    uint8_t lValue = getKo(BTN_KoBTNOutput2Status)->value(DPT_Scaling);
    mStatusLong = (lValue < mParams.dynamicStatusThreshold) ? false : true;
    SERIAL_DEBUG.printf("    long: %i/%i/%i\n\r", lValue, mParams.dynamicStatusThreshold, mStatusLong);
  }
  // ExtraLong
  if (mParams.outputExtraLongDpt == 7 || mParams.outputExtraLongDpt == 8) {
    uint8_t lValue = getKo(BTN_KoBTNOutput3Status)->value(DPT_Scaling);
    mStatusExtraLong = (lValue < mParams.dynamicStatusThreshold) ? false : true;
    SERIAL_DEBUG.printf("    extralong: %i/%i/%i\n\r", lValue, mParams.dynamicStatusThreshold, mStatusExtraLong);
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
    processInputKoStatus(iKo, 1, mParams.outputShortDpt, mStatusShort);
    break;
  case BTN_KoBTNOutput2Status:
    processInputKoStatus(iKo, 2, mParams.outputLongDpt, mStatusLong);
    break;
  case BTN_KoBTNOutput3Status:
    processInputKoStatus(iKo, 3, mParams.outputExtraLongDpt, mStatusExtraLong);
    break;
  }
}

void VirtualButton::processInputKoStatus(GroupObject &iKo, uint8_t iStatusNumber, uint8_t iDpt, bool &oStatus)
{
  SERIAL_DEBUG.printf("BTN::processInputKoStatus %i: %i/%i/%i\n\r", mIndex, iStatusNumber, iDpt, oStatus);
  if (iDpt == 7 || iDpt == 8)
  {
    uint8_t lValue = iKo.value(DPT_Scaling);
    if (lValue == 0 && oStatus)
      oStatus = false;
    if (lValue == 100 && !oStatus)
      oStatus = true;

    evaluateDynamicStatus();
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
  // if (mParams.outputLong == 0 && mParams.outputLong != 7 && mParams.outputLong != 8)
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
  if (mLock)
    return;

  if (mParams.mode == 3 && iButton == 0)
  {
    mButtonState[0].multiClicks += 1;
    mButtonState[0].multiClickTimer = millis();
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
  if (mParams.multiClickCount)
    getKo(BTN_KoBTNOutput1)->value(iClicks, DPT_DecimalFactor);

  if (iClicks > BTN_MaxMuliClicks)
    return;

  uint8_t lIndex = iClicks - 1;
  uint16_t lOutputKo = BTN_KoBTNOutput1;
  sMultiClickParams lParams = mMultiClickParams[lIndex];

  if (!lParams.active)
    return;

  // Sonderlocke für DTPT1
  if (mParams.outputMultiClickDpt == 1)
    lOutputKo = BTN_KoBTNOutput4 + lIndex;

  SERIAL_DEBUG.printf("  BTN%i/%i: MultiClick %i clicks - type %i ko %i value %i\n\r", mIndex, 0, iClicks, mParams.outputMultiClickDpt, lOutputKo, lParams.output);
  writeOutput(mParams.outputMultiClickDpt, lOutputKo, lParams.output, mStatusShort);
}
void VirtualButton::eventShortPress(bool iButton)
{
  if (!mButtonParams[iButton].outputShortPressActive)
    return;

  SERIAL_DEBUG.printf("  BTN%i/%i: short press\n\r", mIndex, iButton);

  // Output
  writeOutput(mParams.outputShortDpt, BTN_KoBTNOutput1, mButtonParams[iButton].outputShortPress, mStatusShort);
}

void VirtualButton::eventLongPress(bool iButton)
{
  if (!mButtonParams[iButton].outputLongPressActive)
    return;

  SERIAL_DEBUG.printf("  BTN%i/%i: long press\n\r", mIndex, iButton);

  // Output
  writeOutput(mParams.outputLongDpt, BTN_KoBTNOutput2, mButtonParams[iButton].outputLongPress, mStatusLong);
}

void VirtualButton::eventExtraLongPress(bool iButton)
{
  if (!mButtonParams[iButton].outputExtraLongPressActive)
    return;

  SERIAL_DEBUG.printf("  BTN%i/%i: extra long press\n\r", mIndex, iButton);

  // Output
  writeOutput(mParams.outputExtraLongDpt, BTN_KoBTNOutput3, mButtonParams[iButton].outputExtraLongPress, mStatusExtraLong);
}

void VirtualButton::eventShortRelease(bool iButton)
{
  if (!mButtonParams[iButton].outputShortReleaseActive && !mButtonParams[iButton].output2Short)
    return;

  SERIAL_DEBUG.printf("  BTN%i/%i: short release\n\r", mIndex, iButton);

  // Output 1
  if (mButtonParams[iButton].outputShortReleaseActive)
    writeOutput(mParams.outputShortDpt, BTN_KoBTNOutput1, mButtonParams[iButton].outputShortRelease, mStatusShort);

  // Output 2
  if (mButtonParams[iButton].output2Short)
    writeOutput(1, BTN_KoBTNOutput4, mButtonParams[iButton].output2Short, mStatusShort);
}

void VirtualButton::eventLongRelease(bool iButton)
{
  if (!mButtonParams[iButton].outputLongReleaseActive && !mButtonParams[iButton].output2Long)
    return;

  SERIAL_DEBUG.printf("  BTN%i/%i: long release\n\r", mIndex, iButton);

  // Output 1
  if (mButtonParams[iButton].outputLongReleaseActive)
    writeOutput(mParams.outputLongDpt, BTN_KoBTNOutput2, mButtonParams[iButton].outputLongRelease, mStatusLong);

  // Output 2
  if (mButtonParams[iButton].output2Long)
    writeOutput(1, BTN_KoBTNOutput5, mButtonParams[iButton].output2Long, mStatusLong);
}

void VirtualButton::eventExtraLongRelease(bool iButton)
{
  if (!mButtonParams[iButton].outputExtraLongReleaseActive && !mButtonParams[iButton].output2ExtraLong)
    return;

  SERIAL_DEBUG.printf("  BTN%i/%i: extra long release\n\r", mIndex, iButton);

  // Output 1
  if (mButtonParams[iButton].outputExtraLongReleaseActive)
    writeOutput(mParams.outputExtraLongDpt, BTN_KoBTNOutput3, mButtonParams[iButton].outputExtraLongRelease, mStatusExtraLong);

  // Output 2
  if (mButtonParams[iButton].output2ExtraLong)
    writeOutput(1, BTN_KoBTNOutput6, mButtonParams[iButton].output2ExtraLong, mStatusExtraLong);
}

void VirtualButton::writeOutput(uint8_t iOutputDPT, uint16_t iOutputKo, uint16_t iOutputValue, bool &oStatus)
{
  SERIAL_DEBUG.printf("  BTN%i: writeOutput %i/%i/%i/%i\n\r", mIndex, iOutputDPT, iOutputKo, iOutputValue, oStatus);
  switch (iOutputDPT)
  {
  case BTN_DPT1:
    // aus
    if (iOutputValue == 1)
      oStatus = false;

    // an
    if (iOutputValue == 2)
      oStatus = true;

    // toggle
    if (iOutputValue == 3)
    {
      oStatus = !oStatus;
    }

    getKo(iOutputKo)->value((bool)oStatus, DPT_Switch);
    break;
  case BTN_DPT2:
    // toggle
    if (iOutputValue >= 10)
    {
      oStatus = !oStatus;
      iOutputValue = (iOutputValue == 10 ? 0 : 2) + (uint8_t)oStatus;
    }

    getKo(iOutputKo)->value((uint8_t)iOutputValue, DPT_DecimalFactor);
    break;

  case BTN_DPT5:
    getKo(iOutputKo)->value((uint8_t)iOutputValue, DPT_DecimalFactor);
    break;

  case BTN_DPT5001:
    getKo(iOutputKo)->value((uint8_t)iOutputValue, DPT_Scaling);
    break;

  case BTN_DPT18:
    if (iOutputValue >= 100)
    {
      iOutputValue = (uint8_t)((iOutputValue - 101) | 0x81);
    }
    else
    {
      iOutputValue = (uint8_t)(iOutputValue - 1);
    }

    getKo(iOutputKo)->value((uint8_t)iOutputValue, DPT_DecimalFactor);
    break;

  case BTN_DPT3007:
    // Start Timer for Status Fallback
    mDynamicStatusTimer = (iOutputValue == 0 || iOutputValue == 8 || iOutputValue == 16) ? millis() : 0;

    if (iOutputValue == 16) // 1-Taster Stop
      iOutputValue = oStatus ? 8 : 0;

    if (iOutputValue == 17) // 1-Taster 100%
    {
      oStatus = !oStatus;
      iOutputValue = oStatus ? 9 : 1;
    }

    getKo(iOutputKo)->value((uint8_t)iOutputValue, DPT_DecimalFactor);
    break;

    // DPT3008
    if (iOutputValue == 16) // 1-Taster Stop
      iOutputValue = oStatus ? 0 : 8;

    if (iOutputValue == 17) // 1-Taster 100%
    {
      oStatus = !oStatus;
      iOutputValue = oStatus ? 1 : 9;
    }

    getKo(iOutputKo)->value((uint8_t)iOutputValue, DPT_DecimalFactor);
    break;

  case BTN_DPT7:
    // 2Byte Int
    getKo(iOutputKo)->value((uint16_t)iOutputValue, DPT_Value_2_Ucount);
    break;
  }
}

// void VirtualButton::writeSwitchOutput(uint8_t iOutput, uint8_t iValue, bool &oStatus, uint8_t iKoOutput)
// {
//   SERIAL_DEBUG.printf("  BTN%i WRITE %i/%i/%i/%i\n\r", mIndex, iOutput, iValue, oStatus, iKoOutput);

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
// }

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
