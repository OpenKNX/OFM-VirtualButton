#include "VirtualButton.h"

VirtualButton::VirtualButton(uint8_t iIndex)
{
    _channelIndex = iIndex;
    _channelParamBlockSize = BTN_ParamBlockSize;
    _channelParamKoBlockSize = BTN_KoBlockSize;
    _channelParamKoOffset = BTN_KoOffset;
    _channelParamOffset = BTN_ParamBlockOffset;
}

const char *VirtualButton::name()
{
    char *buffer = new char[20];
    sprintf(buffer, "VirtualButton<%i>", _channelIndex + 1);
    return buffer;
}

void VirtualButton::setup()
{
    // Global Params
    // ParamBTN_ChannelMode = (knx.paramByte(calcParamIndex(BTN_ChannelMode)) & BTN_ChannelModeMask) >> BTN_ChannelModeShift;
    // ParamBTN_ChannelLock = (knx.paramByte(calcParamIndex(BTN_ChannelLock)) & BTN_ChannelLockMask) >> BTN_ChannelLockShift;
    // ParamBTN_ChannelMultiClickCount = (knx.paramByte(calcParamIndex(BTN_ChannelMultiClickCount)) & BTN_ChannelMultiClickCountMask) >> BTN_ChannelMultiClickCountShift;

    // Input
    mButtonParams[0].inputKo = ParamBTN_ChannelInputA;
    mButtonParams[1].inputKo = ParamBTN_ChannelInputB;

    // Output 1
    //   DPT
    //ParamBTN_ChannelOutputShort_DPT = ParamBTN_ChannelOutputShort_DPT;
    //ParamBTN_ChannelOutputLong_DPT = ParamBTN_ChannelOutputLong_DPT;
    //ParamBTN_ChannelOutputExtraLong_DPT = ParamBTN_ChannelOutputExtraLong_DPT;
    //ParamBTN_ChannelOutputMulti_DPT = ParamBTN_ChannelOutputMulti_DPT;

    //   Events
    mButtonParams[0].outputShortPressActive = (knx.paramByte(calcParamIndex(BTN_ChannelOutputShort_Taster1_Active_Press)) & BTN_ChannelOutputShort_Taster1_Active_PressMask) >> BTN_ChannelOutputShort_Taster1_Active_PressShift;
    mButtonParams[0].outputShortReleaseActive = (knx.paramByte(calcParamIndex(BTN_ChannelOutputShort_Taster1_Active_Release)) & BTN_ChannelOutputShort_Taster1_Active_ReleaseMask) >> BTN_ChannelOutputShort_Taster1_Active_ReleaseShift;
    debug("outputShortReleaseActive %i", mButtonParams[0].outputShortReleaseActive);
    debug("outputShortReleaseActive %i", (bool)ParamBTN_ChannelOutputShort_Taster1_Active_Release);
    mButtonParams[1].outputShortPressActive = (knx.paramByte(calcParamIndex(BTN_ChannelOutputShort_Taster2_Active_Press)) & BTN_ChannelOutputShort_Taster2_Active_PressMask) >> BTN_ChannelOutputShort_Taster2_Active_PressShift;
    mButtonParams[1].outputShortReleaseActive = (knx.paramByte(calcParamIndex(BTN_ChannelOutputShort_Taster2_Active_Release)) & BTN_ChannelOutputShort_Taster2_Active_ReleaseMask) >> BTN_ChannelOutputShort_Taster2_Active_ReleaseShift;

    mButtonParams[0].outputLongPressActive = (knx.paramByte(calcParamIndex(BTN_ChannelOutputLong_Taster1_Active_Press)) & BTN_ChannelOutputLong_Taster1_Active_PressMask) >> BTN_ChannelOutputLong_Taster1_Active_PressShift;
    mButtonParams[0].outputLongReleaseActive = (knx.paramByte(calcParamIndex(BTN_ChannelOutputLong_Taster1_Active_Release)) & BTN_ChannelOutputLong_Taster1_Active_ReleaseMask) >> BTN_ChannelOutputLong_Taster1_Active_ReleaseShift;
    mButtonParams[1].outputLongPressActive = (knx.paramByte(calcParamIndex(BTN_ChannelOutputLong_Taster2_Active_Press)) & BTN_ChannelOutputLong_Taster2_Active_PressMask) >> BTN_ChannelOutputLong_Taster2_Active_PressShift;
    mButtonParams[1].outputLongReleaseActive = (knx.paramByte(calcParamIndex(BTN_ChannelOutputLong_Taster2_Active_Release)) & BTN_ChannelOutputLong_Taster2_Active_ReleaseMask) >> BTN_ChannelOutputLong_Taster2_Active_ReleaseShift;

    mButtonParams[0].outputExtraLongPressActive = (knx.paramByte(calcParamIndex(BTN_ChannelOutputExtraLong_Taster1_Active_Press)) & BTN_ChannelOutputExtraLong_Taster1_Active_PressMask) >> BTN_ChannelOutputExtraLong_Taster1_Active_PressShift;
    mButtonParams[0].outputExtraLongReleaseActive = (knx.paramByte(calcParamIndex(BTN_ChannelOutputExtraLong_Taster1_Active_Release)) & BTN_ChannelOutputExtraLong_Taster1_Active_ReleaseMask) >> BTN_ChannelOutputExtraLong_Taster1_Active_ReleaseShift;
    mButtonParams[1].outputExtraLongPressActive = (knx.paramByte(calcParamIndex(BTN_ChannelOutputExtraLong_Taster2_Active_Press)) & BTN_ChannelOutputExtraLong_Taster2_Active_PressMask) >> BTN_ChannelOutputExtraLong_Taster2_Active_PressShift;
    mButtonParams[1].outputExtraLongReleaseActive = (knx.paramByte(calcParamIndex(BTN_ChannelOutputExtraLong_Taster2_Active_Release)) & BTN_ChannelOutputExtraLong_Taster2_Active_ReleaseMask) >> BTN_ChannelOutputExtraLong_Taster2_Active_ReleaseShift;

    //   Outputs - DPT1 ist stellvertretend für alle DPTs
    mButtonParams[0].outputShortPress = knx.paramWord(calcParamIndex(BTN_ChannelOutputShort_Taster1_Dpt1_Press));
    mButtonParams[0].outputShortRelease = knx.paramWord(calcParamIndex(BTN_ChannelOutputShort_Taster1_Dpt1_Release));
    mButtonParams[1].outputShortPress = knx.paramWord(calcParamIndex(BTN_ChannelOutputShort_Taster2_Dpt1_Press));
    mButtonParams[1].outputShortRelease = knx.paramWord(calcParamIndex(BTN_ChannelOutputShort_Taster2_Dpt1_Release));

    mButtonParams[0].outputLongPress = knx.paramWord(calcParamIndex(BTN_ChannelOutputLong_Taster1_Dpt1_Press));
    mButtonParams[0].outputLongRelease = knx.paramWord(calcParamIndex(BTN_ChannelOutputLong_Taster1_Dpt1_Release));
    mButtonParams[1].outputLongPress = knx.paramWord(calcParamIndex(BTN_ChannelOutputLong_Taster2_Dpt1_Press));
    mButtonParams[1].outputLongRelease = knx.paramWord(calcParamIndex(BTN_ChannelOutputLong_Taster2_Dpt1_Release));

    mButtonParams[0].outputExtraLongPress = knx.paramWord(calcParamIndex(BTN_ChannelOutputExtraLong_Taster1_Dpt1_Press));
    mButtonParams[0].outputExtraLongRelease = knx.paramWord(calcParamIndex(BTN_ChannelOutputExtraLong_Taster1_Dpt1_Release));
    mButtonParams[1].outputExtraLongPress = knx.paramWord(calcParamIndex(BTN_ChannelOutputExtraLong_Taster2_Dpt1_Press));
    mButtonParams[1].outputExtraLongRelease = knx.paramWord(calcParamIndex(BTN_ChannelOutputExtraLong_Taster2_Dpt1_Release));

    // Output 2
    mButtonParams[0].output2Short = (knx.paramByte(calcParamIndex(BTN_ChannelOutput2Short_Taster1)) & BTN_ChannelOutput2Short_Taster1Mask) >> BTN_ChannelOutput2Short_Taster1Shift;
    mButtonParams[1].output2Short = (knx.paramByte(calcParamIndex(BTN_ChannelOutput2Short_Taster2)) & BTN_ChannelOutput2Short_Taster2Mask) >> BTN_ChannelOutput2Short_Taster2Shift;
    mButtonParams[0].output2Long = (knx.paramByte(calcParamIndex(BTN_ChannelOutput2Long_Taster1)) & BTN_ChannelOutput2Long_Taster1Mask) >> BTN_ChannelOutput2Long_Taster1Shift;
    mButtonParams[1].output2Long = (knx.paramByte(calcParamIndex(BTN_ChannelOutput2Long_Taster2)) & BTN_ChannelOutput2Long_Taster2Mask) >> BTN_ChannelOutput2Long_Taster2Shift;
    mButtonParams[0].output2ExtraLong = (knx.paramByte(calcParamIndex(BTN_ChannelOutput2ExtraLong_Taster1)) & BTN_ChannelOutput2ExtraLong_Taster1Mask) >> BTN_ChannelOutput2ExtraLong_Taster1Shift;
    mButtonParams[1].output2ExtraLong = (knx.paramByte(calcParamIndex(BTN_ChannelOutput2ExtraLong_Taster2)) & BTN_ChannelOutput2ExtraLong_Taster2Mask) >> BTN_ChannelOutput2ExtraLong_Taster2Shift;

    // MultiClick
    mMultiClickParams[0].active = (knx.paramByte(calcParamIndex(BTN_ChannelOutputMulti_Click1_Active)) & BTN_ChannelOutputMulti_Click1_ActiveMask) >> BTN_ChannelOutputMulti_Click1_ActiveShift;
    mMultiClickParams[1].active = (knx.paramByte(calcParamIndex(BTN_ChannelOutputMulti_Click2_Active)) & BTN_ChannelOutputMulti_Click2_ActiveMask) >> BTN_ChannelOutputMulti_Click2_ActiveShift;
    mMultiClickParams[2].active = (knx.paramByte(calcParamIndex(BTN_ChannelOutputMulti_Click3_Active)) & BTN_ChannelOutputMulti_Click3_ActiveMask) >> BTN_ChannelOutputMulti_Click3_ActiveShift;

    // DPT2 ist stellvertretend für alle DPTs (DPT1 nicht ntuzbar da für die einzeldklicks in Verwendung)
    mMultiClickParams[0].output = knx.paramWord(calcParamIndex(BTN_ChannelOutputMulti_Click1_Dpt1));
    mMultiClickParams[1].output = knx.paramWord(calcParamIndex(BTN_ChannelOutputMulti_Click2_Dpt1));
    mMultiClickParams[2].output = knx.paramWord(calcParamIndex(BTN_ChannelOutputMulti_Click3_Dpt1));

    // ReactionTimes
    mParams.reactionTimeMultiClick = knx.paramByte(calcParamIndex(BTN_ChannelReactionTimeMultiClick));
    if (mParams.reactionTimeMultiClick > 0)
        mParams.reactionTimeMultiClick *= 100;
    else
        mParams.reactionTimeMultiClick = knx.paramByte(BTN_ReactionTimeMultiClick) * 100;

    mParams.reactionTimeLong = knx.paramByte(calcParamIndex(BTN_ChannelReactionTimeLong));
    if (mParams.reactionTimeLong > 0)
        mParams.reactionTimeLong *= 100;
    else
        mParams.reactionTimeLong = knx.paramByte(BTN_ReactionTimeLong) * 100;

    mParams.reactionTimeExtraLong = knx.paramByte(calcParamIndex(BTN_ChannelReactionTimeExtraLong));
    if (mParams.reactionTimeExtraLong > 0)
        mParams.reactionTimeExtraLong *= 100;
    else
        mParams.reactionTimeExtraLong = knx.paramByte(BTN_ReactionTimeExtraLong) * 100;

    mParams.dynamicStatusThreshold = knx.paramByte(calcParamIndex(BTN_ChannelStatusThreshold));
    mParams.dynamicStatusFallback = (getDelayPattern(calcParamIndex(BTN_ChannelStatusFallbackBase)));

    // Debug
    // debug("BTN %i dynamicStatusThreshold: %i\n\r", mIndex, mParams.dynamicStatusThreshold);
    // debug("BTN %i dynamicStatusFallback: %i\n\r", mIndex, mParams.dynamicStatusFallback);
    // debug("BTN %i inputKo: %i/%i\n\r", mIndex, 0, mButtonParams[0].inputKo);
    // debug("BTN %i outputShortPressActive: %i/%i\n\r", mIndex, 0, mButtonParams[0].outputShortPressActive);
    // debug("BTN %i outputShortReleaseActive: %i/%i\n\r", mIndex, 0, mButtonParams[0].outputShortReleaseActive);
    // debug("BTN %i outputLongPressActive: %i/%i\n\r", mIndex, 0, mButtonParams[0].outputLongPressActive);
    // debug("BTN %i outputLongReleaseActive: %i/%i\n\r", mIndex, 0, mButtonParams[0].outputLongReleaseActive);
    // debug("BTN %i outputExtraLongPressActive: %i/%i\n\r", mIndex, 0, mButtonParams[0].outputExtraLongPressActive);
    // debug("BTN %i outputExtraLongReleaseActive: %i/%i\n\r", mIndex, 0, mButtonParams[0].outputExtraLongReleaseActive);
    // debug("BTN %i inputKo: %i/%i\n\r", mIndex, 1, mButtonParams[1].inputKo);
    // debug("BTN %i outputShortPressActive: %i/%i\n\r", mIndex, 1, mButtonParams[1].outputShortPressActive);
    // debug("BTN %i outputShortReleaseActive: %i/%i\n\r", mIndex, 1, mButtonParams[1].outputShortReleaseActive);
    // debug("BTN %i outputLongPressActive: %i/%i\n\r", mIndex, 1, mButtonParams[1].outputLongPressActive);
    // debug("BTN %i outputLongReleaseActive: %i/%i\n\r", mIndex, 1, mButtonParams[1].outputLongReleaseActive);
    // debug("BTN %i outputExtraLongPressActive: %i/%i\n\r", mIndex, 1, mButtonParams[1].outputExtraLongPressActive);
    // debug("BTN %i outputExtraLongReleaseActive: %i/%i\n\r", mIndex, 1, mButtonParams[1].outputExtraLongReleaseActive);
    // debug("BTN %i outputShortPress: %i/%i\n\r", mIndex, 0, mButtonParams[0].outputShortPress);
    // debug("BTN %i outputShortRelease: %i/%i\n\r", mIndex, 0, mButtonParams[0].outputShortRelease);
    // debug("BTN %i outputLongPress: %i/%i\n\r", mIndex, 0, mButtonParams[0].outputLongPress);
    // debug("BTN %i outputLongRelease: %i/%i\n\r", mIndex, 0, mButtonParams[0].outputLongRelease);
    // debug("BTN %i outputExtraLongPress: %i/%i\n\r", mIndex, 0, mButtonParams[0].outputExtraLongPress);
    // debug("BTN %i outputExtraLongRelease: %i/%i\n\r", mIndex, 0, mButtonParams[0].outputExtraLongRelease);
    // debug("BTN %i outputShortPress: %i/%i\n\r", mIndex, 1, mButtonParams[1].outputShortPress);
    // debug("BTN %i outputShortRelease: %i/%i\n\r", mIndex, 1, mButtonParams[1].outputShortRelease);
    // debug("BTN %i outputLongPress: %i/%i\n\r", mIndex, 1, mButtonParams[1].outputLongPress);
    // debug("BTN %i outputLongRelease: %i/%i\n\r", mIndex, 1, mButtonParams[1].outputLongRelease);
    // debug("BTN %i outputExtraLongPress: %i/%i\n\r", mIndex, 1, mButtonParams[1].outputExtraLongPress);
    // debug("BTN %i outputExtraLongRelease: %i/%i\n\r", mIndex, 1, mButtonParams[1].outputExtraLongRelease);
    // debug("BTN %i output2Short: %i/%i\n\r", mIndex, 2, mButtonParams[0].output2Short);
    // debug("BTN %i output2Short: %i/%i\n\r", mIndex, 1, mButtonParams[1].output2Short);
    // debug("BTN %i MC Active: %i/%i\n\r", mIndex, 0, mMultiClickParams[0].active);
    // debug("BTN %i MC Output: %i/%i\n\r", mIndex, 0, mMultiClickParams[0].output);
    // debug("BTN %i MC Active: %i/%i\n\r", mIndex, 1, mMultiClickParams[1].active);
    // debug("BTN %i MC Output: %i/%i\n\r", mIndex, 1, mMultiClickParams[1].output);
    // debug("BTN %i MC Active: %i/%i\n\r", mIndex, 2, mMultiClickParams[2].active);
    // debug("BTN %i MC Output: %i/%i\n\r", mIndex, 2, mMultiClickParams[2].output);
    // debug("BTN %i reactionTimeMultiClick: %i\n\r", mIndex, mParams.reactionTimeMultiClick);
    // debug("BTN %i reactionTimeLong: %i\n\r", mIndex, mParams.reactionTimeLong);
    // debug("BTN %i reactionTimeExtraLong: %i\n\r", mIndex, mParams.reactionTimeExtraLong);
    // debug("BTN %i mParamMode: %i\n\r", mIndex, ParamBTN_ChannelMode);
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
void VirtualButton::evaluateDynamicStatus()
{
    // Läuft gerade ein Timer, so ist keine Änderung erlaubt.
    if (mDynamicStatusTimer > 0)
        return;

    // Es ist überhaupt kein dynamischer Status aktiv
    if (mParams.dynamicStatusFallback == 0)
        return;

    debug("evaluateDynamicStatus");

    // Short
    if (ParamBTN_ChannelOutputShort_DPT == 7 || ParamBTN_ChannelOutputShort_DPT == 8)
    {
        uint8_t lValue = getKo(BTN_KoChannelOutput1Status)->value(DPT_Scaling);
        mStatusShort = (lValue < mParams.dynamicStatusThreshold) ? false : true;
        debug("  short: %i/%i/%i", lValue, mParams.dynamicStatusThreshold, mStatusShort);
    }
    // Long
    if (ParamBTN_ChannelOutputLong_DPT == 7 || ParamBTN_ChannelOutputLong_DPT == 8)
    {
        uint8_t lValue = getKo(BTN_KoChannelOutput2Status)->value(DPT_Scaling);
        mStatusLong = (lValue < mParams.dynamicStatusThreshold) ? false : true;
        debug("  long: %i/%i/%i", lValue, mParams.dynamicStatusThreshold, mStatusLong);
    }
    // ExtraLong
    if (ParamBTN_ChannelOutputExtraLong_DPT == 7 || ParamBTN_ChannelOutputExtraLong_DPT == 8)
    {
        uint8_t lValue = getKo(BTN_KoChannelOutput3Status)->value(DPT_Scaling);
        mStatusExtraLong = (lValue < mParams.dynamicStatusThreshold) ? false : true;
        debug("  extralong: %i/%i/%i", lValue, mParams.dynamicStatusThreshold, mStatusExtraLong);
    }
}

void VirtualButton::processInputKo(GroupObject &iKo)
{
    if (ParamBTN_ChannelMode == 0)
        return;

    uint16_t lKoNumber = iKo.asap();
    uint8_t lKoIndex = calcKoIndex(lKoNumber);

    // Internal
    if (mButtonParams[0].inputKo > 0 && mButtonParams[0].inputKo == lKoNumber)
    {
        lKoIndex = BTN_KoChannelInput1;
    }
    else if (mButtonParams[1].inputKo > 0 && mButtonParams[1].inputKo == lKoNumber)
    {
        lKoIndex = BTN_KoChannelInput2;
    }

    switch (lKoIndex)
    {
        case BTN_KoChannelInput1:
            processInputKoInput(iKo, 0);
            break;
        case BTN_KoChannelInput2:
            processInputKoInput(iKo, 1);
            break;
        case BTN_KoChannelLock:
            processInputKoLock(iKo);
            break;
        case BTN_KoChannelOutput1Status:
            processInputKoStatus(iKo, 1, ParamBTN_ChannelOutputShort_DPT, mStatusShort);
            break;
        case BTN_KoChannelOutput2Status:
            processInputKoStatus(iKo, 2, ParamBTN_ChannelOutputLong_DPT, mStatusLong);
            break;
        case BTN_KoChannelOutput3Status:
            processInputKoStatus(iKo, 3, ParamBTN_ChannelOutputExtraLong_DPT, mStatusExtraLong);
            break;
    }
}

void VirtualButton::processInputKoStatus(GroupObject &iKo, uint8_t iStatusNumber, uint8_t iDpt, bool &oStatus)
{
    debug("processInputKoStatus %i/%i/%i", iStatusNumber, iDpt, oStatus);
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
    if (ParamBTN_ChannelLock == 0)
        return;

    bool lValue = iKo.value(DPT_Switch);

    if (ParamBTN_ChannelLock == 1)
        mLock = lValue;
    return;

    if (ParamBTN_ChannelLock == 2)
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

    // debug("processInputKoLock %i", mLock);
}

void VirtualButton::processInputKoInput(GroupObject &iKo, bool iButton)
{
    bool lNewPress = iKo.value(DPT_Switch);
    bool lLastPress = mButtonState[iButton].press;
    debug("processInputKoInput %i", lNewPress);

    // werte "Loslassen" nur aus, wenn der letze Zustand auch "gedrückt" war
    if (!lLastPress && !lNewPress)
        return;

    // wenn der wert sich seit dem letzten mal nicht geändert hat
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

    // no (extra) long press configured
    if (ParamBTN_ChannelOutputLong_DPT == 0 && ParamBTN_ChannelOutputExtraLong_DPT == 0)
        return;

    if (!mButtonState[iButton].pressLong && delayCheck(mButtonState[iButton].pressStart, mParams.reactionTimeLong))
    {
        eventLongPress(iButton);
        mButtonState[iButton].pressLong = true;
    }

    // no extra long press configured
    if (ParamBTN_ChannelOutputExtraLong_DPT == 0)
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

    if (ParamBTN_ChannelMode == 3 && iButton == 0)
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
    else if (ParamBTN_ChannelMode == 3)
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
    if (ParamBTN_ChannelMode != 3)
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
    if (ParamBTN_ChannelMultiClickCount)
        getKo(BTN_KoChannelOutput1)->value(iClicks, DPT_DecimalFactor);

    if (iClicks > BTN_MaxMuliClicks)
        return;

    uint8_t lIndex = iClicks - 1;
    uint16_t lOutputKo = BTN_KoChannelOutput1;
    sMultiClickParams lParams = mMultiClickParams[lIndex];

    if (!lParams.active)
        return;

    // Sonderlocke für DTPT1
    if (ParamBTN_ChannelOutputMulti_DPT == 1)
        lOutputKo = BTN_KoChannelOutput4 + lIndex;

    debug("  Button %i: MultiClick %i clicks - type %i ko %i value %i", 0, iClicks, ParamBTN_ChannelOutputMulti_DPT, lOutputKo, lParams.output);
    writeOutput(ParamBTN_ChannelOutputMulti_DPT, lOutputKo, lParams.output, mStatusShort);
}
void VirtualButton::eventShortPress(bool iButton)
{
    if (!mButtonParams[iButton].outputShortPressActive)
        return;

    debug("  Button %i: short press", iButton);

    // Output
    writeOutput(ParamBTN_ChannelOutputShort_DPT, BTN_KoChannelOutput1, mButtonParams[iButton].outputShortPress, mStatusShort);
}

void VirtualButton::eventLongPress(bool iButton)
{
    if (!mButtonParams[iButton].outputLongPressActive)
        return;

    debug("  Button %i: long press", iButton);

    // Output
    writeOutput(ParamBTN_ChannelOutputLong_DPT, BTN_KoChannelOutput2, mButtonParams[iButton].outputLongPress, mStatusLong);
}

void VirtualButton::eventExtraLongPress(bool iButton)
{
    if (!mButtonParams[iButton].outputExtraLongPressActive)
        return;

    debug("  Button %i: extra long press", iButton);

    // Output
    writeOutput(ParamBTN_ChannelOutputExtraLong_DPT, BTN_KoChannelOutput3, mButtonParams[iButton].outputExtraLongPress, mStatusExtraLong);
}

void VirtualButton::eventShortRelease(bool iButton)
{
    if (!mButtonParams[iButton].outputShortReleaseActive && !mButtonParams[iButton].output2Short)
        return;

    debug("  Button %i: short release", iButton);

    // Output 1
    if (mButtonParams[iButton].outputShortReleaseActive)
        writeOutput(ParamBTN_ChannelOutputShort_DPT, BTN_KoChannelOutput1, mButtonParams[iButton].outputShortRelease, mStatusShort);

    // Output 2
    if (mButtonParams[iButton].output2Short)
        writeOutput(1, BTN_KoChannelOutput4, mButtonParams[iButton].output2Short, mStatusShort);
}

void VirtualButton::eventLongRelease(bool iButton)
{
    if (!mButtonParams[iButton].outputLongReleaseActive && !mButtonParams[iButton].output2Long)
        return;

    debug("  Button %i: long release", iButton);

    // Output 1
    if (mButtonParams[iButton].outputLongReleaseActive)
        writeOutput(ParamBTN_ChannelOutputLong_DPT, BTN_KoChannelOutput2, mButtonParams[iButton].outputLongRelease, mStatusLong);

    // Output 2
    if (mButtonParams[iButton].output2Long)
        writeOutput(1, BTN_KoChannelOutput5, mButtonParams[iButton].output2Long, mStatusLong);
}

void VirtualButton::eventExtraLongRelease(bool iButton)
{
    if (!mButtonParams[iButton].outputExtraLongReleaseActive && !mButtonParams[iButton].output2ExtraLong)
        return;

    debug("  Button %i: extra long release", iButton);

    // Output 1
    if (mButtonParams[iButton].outputExtraLongReleaseActive)
        writeOutput(ParamBTN_ChannelOutputExtraLong_DPT, BTN_KoChannelOutput3, mButtonParams[iButton].outputExtraLongRelease, mStatusExtraLong);

    // Output 2
    if (mButtonParams[iButton].output2ExtraLong)
        writeOutput(1, BTN_KoChannelOutput6, mButtonParams[iButton].output2ExtraLong, mStatusExtraLong);
}

void VirtualButton::writeOutput(uint8_t iOutputDPT, uint16_t iOutputKo, uint16_t iOutputValue, bool &oStatus)
{
    debug("  writeOutput %i/%i/%i/%i", iOutputDPT, iOutputKo, iOutputValue, oStatus);
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