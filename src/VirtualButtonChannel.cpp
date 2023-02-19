#include "VirtualButtonChannel.h"
#include <set>

VirtualButtonChannel::VirtualButtonChannel(uint8_t index)
{
    _channelIndex = index;
}

const std::string VirtualButtonChannel::name()
{
    return "VirtualButton";
}

void VirtualButtonChannel::setup()
{
    // Input
    _buttonParams[0].inputKo = ParamBTN_ChannelInputA;
    _buttonParams[1].inputKo = ParamBTN_ChannelInputB;

    //   Events
    _buttonParams[0].outputShortPressActive = ParamBTN_ChannelOutputShort_Taster1_Active_Press;
    _buttonParams[0].outputShortReleaseActive = ParamBTN_ChannelOutputShort_Taster1_Active_Release;
    _buttonParams[1].outputShortPressActive = ParamBTN_ChannelOutputShort_Taster2_Active_Press;
    _buttonParams[1].outputShortReleaseActive = ParamBTN_ChannelOutputShort_Taster2_Active_Release;

    _buttonParams[0].outputLongPressActive = ParamBTN_ChannelOutputLong_Taster1_Active_Press;
    _buttonParams[0].outputLongReleaseActive = ParamBTN_ChannelOutputLong_Taster1_Active_Release;
    _buttonParams[1].outputLongPressActive = ParamBTN_ChannelOutputLong_Taster2_Active_Press;
    _buttonParams[1].outputLongReleaseActive = ParamBTN_ChannelOutputLong_Taster2_Active_Release;

    _buttonParams[0].outputExtraLongPressActive = ParamBTN_ChannelOutputExtraLong_Taster1_Active_Press;
    _buttonParams[0].outputExtraLongReleaseActive = ParamBTN_ChannelOutputExtraLong_Taster1_Active_Release;
    _buttonParams[1].outputExtraLongPressActive = ParamBTN_ChannelOutputExtraLong_Taster2_Active_Press;
    _buttonParams[1].outputExtraLongReleaseActive = ParamBTN_ChannelOutputExtraLong_Taster2_Active_Release;

    //   Outputs - DPT1 ist stellvertretend für alle DPTs
    _buttonParams[0].outputShortPress = ParamBTN_ChannelOutputShort_Taster1_Dpt1_Press;
    _buttonParams[0].outputShortRelease = ParamBTN_ChannelOutputShort_Taster1_Dpt1_Release;
    _buttonParams[1].outputShortPress = ParamBTN_ChannelOutputShort_Taster2_Dpt1_Press;
    _buttonParams[1].outputShortRelease = ParamBTN_ChannelOutputShort_Taster2_Dpt1_Release;

    _buttonParams[0].outputLongPress = ParamBTN_ChannelOutputLong_Taster1_Dpt1_Press;
    _buttonParams[0].outputLongRelease = ParamBTN_ChannelOutputLong_Taster1_Dpt1_Release;
    _buttonParams[1].outputLongPress = ParamBTN_ChannelOutputLong_Taster2_Dpt1_Press;
    _buttonParams[1].outputLongRelease = ParamBTN_ChannelOutputLong_Taster2_Dpt1_Release;

    _buttonParams[0].outputExtraLongPress = ParamBTN_ChannelOutputExtraLong_Taster1_Dpt1_Press;
    _buttonParams[0].outputExtraLongRelease = ParamBTN_ChannelOutputExtraLong_Taster1_Dpt1_Release;
    _buttonParams[1].outputExtraLongPress = ParamBTN_ChannelOutputExtraLong_Taster2_Dpt1_Press;
    _buttonParams[1].outputExtraLongRelease = ParamBTN_ChannelOutputExtraLong_Taster2_Dpt1_Release;

    // Output 2
    _buttonParams[0].output2Short = ParamBTN_ChannelOutput2Short_Taster1;
    _buttonParams[1].output2Short = ParamBTN_ChannelOutput2Short_Taster2;
    _buttonParams[0].output2Long = ParamBTN_ChannelOutput2Long_Taster1;
    _buttonParams[1].output2Long = ParamBTN_ChannelOutput2Long_Taster2;
    _buttonParams[0].output2ExtraLong = ParamBTN_ChannelOutput2ExtraLong_Taster1;
    _buttonParams[1].output2ExtraLong = ParamBTN_ChannelOutput2ExtraLong_Taster2;

    // MultiClick
    _multiClickParams[0].active = ParamBTN_ChannelOutputMulti_Click1_Active;
    _multiClickParams[1].active = ParamBTN_ChannelOutputMulti_Click2_Active;
    _multiClickParams[2].active = ParamBTN_ChannelOutputMulti_Click3_Active;

    // DPT2 ist stellvertretend für alle DPTs (DPT1 nicht ntuzbar da für die einzeldklicks in Verwendung)
    _multiClickParams[0].output = ParamBTN_ChannelOutputMulti_Click1_Dpt1;
    _multiClickParams[1].output = ParamBTN_ChannelOutputMulti_Click2_Dpt1;
    _multiClickParams[2].output = ParamBTN_ChannelOutputMulti_Click3_Dpt1;

    // ReactionTimes
    _params.reactionTimeMultiClick = ParamBTN_ChannelReactionTimeMultiClick;
    if (_params.reactionTimeMultiClick > 0)
        _params.reactionTimeMultiClick *= 100;
    else
        _params.reactionTimeMultiClick = ParamBTN_ReactionTimeMultiClick * 100;

    _params.reactionTimeLong = ParamBTN_ChannelReactionTimeLong;
    if (_params.reactionTimeLong > 0)
        _params.reactionTimeLong *= 100;
    else
        _params.reactionTimeLong = ParamBTN_ReactionTimeLong * 100;

    _params.reactionTimeExtraLong = ParamBTN_ChannelReactionTimeExtraLong;
    if (_params.reactionTimeExtraLong > 0)
        _params.reactionTimeExtraLong *= 100;
    else
        _params.reactionTimeExtraLong = ParamBTN_ReactionTimeExtraLong * 100;

    // Debug
    // log("reactionTimeMultiClick: %i", _params.reactionTimeMultiClick);
}

void VirtualButtonChannel::loop()
{
    processPressAndHold(0);
    processPressAndHold(1);
    processMultiClick();
    processDynamicStatusTimer();
}
void VirtualButtonChannel::processDynamicStatusTimer()
{
    // Es ist überhaupt kein dynamischer Status aktiv
    if (ParamBTN_ChannelStatusFallbackTimeMS == 0)
        return;

    if (_dynamicStatusTimer > 0 && delayCheck(_dynamicStatusTimer, ParamBTN_ChannelStatusFallbackTimeMS))
    {
        _dynamicStatusTimer = 0;
        evaluateDynamicStatus();
    }
}

/*
  Nach jedem Tastendruck (3007/3008) wird ein Timer gestartet.
  Läuft dieser ab (processDynamicStatusTimer) wird der interne Status neu evaluiert.
  Ist keine Timer gestartet, so wird immer neu evaluiert (processInputKoStatus)
*/
void VirtualButtonChannel::evaluateDynamicStatus()
{
    // Läuft gerade ein Timer, so ist keine Änderung erlaubt.
    if (_dynamicStatusTimer > 0)
        return;

    // Es ist überhaupt kein dynamischer Status aktiv
    if (ParamBTN_ChannelStatusFallbackTimeMS == 0)
        return;

    log("evaluateDynamicStatus");

    // Short
    if (ParamBTN_ChannelOutputShort_DPT == 7 || ParamBTN_ChannelOutputShort_DPT == 8)
    {
        uint8_t value = KoBTN_ChannelOutput1Status.value(DPT_Scaling);
        _statusShort = (value < ParamBTN_ChannelStatusThreshold) ? false : true;
        log("  short: %i/%i/%i", value, ParamBTN_ChannelStatusThreshold, _statusShort);
    }
    // Long
    if (ParamBTN_ChannelOutputLong_DPT == 7 || ParamBTN_ChannelOutputLong_DPT == 8)
    {
        uint8_t value = KoBTN_ChannelOutput2Status.value(DPT_Scaling);
        _statusLong = (value < ParamBTN_ChannelStatusThreshold) ? false : true;
        log("  long: %i/%i/%i", value, ParamBTN_ChannelStatusThreshold, _statusLong);
    }
    // ExtraLong
    if (ParamBTN_ChannelOutputExtraLong_DPT == 7 || ParamBTN_ChannelOutputExtraLong_DPT == 8)
    {
        uint8_t value = KoBTN_ChannelOutput3Status.value(DPT_Scaling);
        _statusExtraLong = (value < ParamBTN_ChannelStatusThreshold) ? false : true;
        log("  extralong: %i/%i/%i", value, ParamBTN_ChannelStatusThreshold, _statusExtraLong);
    }
}

void VirtualButtonChannel::processInputKo(GroupObject &ko)
{
    if (ParamBTN_ChannelMode == 0)
        return;

    uint16_t koNumber = ko.asap();
    int8_t koIndex = BTN_KoCalcIndex(koNumber);
    // log("koNumber %i -> koIndex %i", koNumber, koIndex);

    // External KO -> to Channel Mapping
    if (_buttonParams[0].inputKo > 0 && _buttonParams[0].inputKo == koNumber)
    {
        koIndex = BTN_KoChannelInput1;
    }
    else if (_buttonParams[1].inputKo > 0 && _buttonParams[1].inputKo == koNumber)
    {
        koIndex = BTN_KoChannelInput2;
    }

    switch (koIndex)
    {
        case BTN_KoChannelInput1:
            processInputKoInput(ko, 0);
            break;
        case BTN_KoChannelInput2:
            processInputKoInput(ko, 1);
            break;
        case BTN_KoChannelLock:
            processInputKoLock(ko);
            break;
        case BTN_KoChannelOutput1Status:
            processInputKoStatus(ko, 1, ParamBTN_ChannelOutputShort_DPT, _statusShort);
            break;
        case BTN_KoChannelOutput2Status:
            processInputKoStatus(ko, 2, ParamBTN_ChannelOutputLong_DPT, _statusLong);
            break;
        case BTN_KoChannelOutput3Status:
            processInputKoStatus(ko, 3, ParamBTN_ChannelOutputExtraLong_DPT, _statusExtraLong);
            break;
    }
}

void VirtualButtonChannel::processInputKoStatus(GroupObject &ko, uint8_t statusNumber, uint8_t dpt, bool &status)
{
    log("processInputKoStatus %i/%i/%i", statusNumber, dpt, status);

    if (dpt == 7 || dpt == 8)
    {
        uint8_t value = ko.value(DPT_Scaling);
        if (value == 0 && status)
            status = false;
        if (value == 100 && !status)
            status = true;

        evaluateDynamicStatus();
    }
    else
    {
        bool value = ko.value(DPT_Switch);
        status = value;
    }
}

void VirtualButtonChannel::processInputKoLock(GroupObject &ko)
{
    if (ParamBTN_ChannelLock == 0)
        return;

    bool value = ko.value(DPT_Switch);

    if (ParamBTN_ChannelLock == 1)
        _lock = value;

    if (ParamBTN_ChannelLock == 2)
        _lock = !value;

    // Reset
    _buttonState[0].multiClicks = 0;
    _buttonState[0].multiClickTimer = 0;
    _buttonState[0].press = false;
    _buttonState[0].pressLong = false;
    _buttonState[0].pressExtraLong = false;
    _buttonState[0].pressStart = 0;
    _buttonState[1].multiClicks = 0;
    _buttonState[1].multiClickTimer = 0;
    _buttonState[1].press = false;
    _buttonState[1].pressLong = false;
    _buttonState[1].pressExtraLong = false;
    _buttonState[1].pressStart = 0;

    log("processInputKoLock %i", _lock);
}

void VirtualButtonChannel::processInputKoInput(GroupObject &ko, bool button)
{
    bool newPress = ko.value(DPT_Switch);
    bool lastPress = _buttonState[button].press;
    log("processInputKoInput %i", newPress);

    // werte "Loslassen" nur aus, wenn der letze Zustand auch "gedrückt" war
    if (!lastPress && !newPress)
        return;

    // wenn der wert sich seit dem letzten mal nicht geändert hat
    if (lastPress == newPress)
        return;

    if (newPress)
    {
        processPress(button);
    }
    else
    {
        processRelease(button);
    }
}

void VirtualButtonChannel::processPressAndHold(bool button)
{
    // not pressed
    if (!_buttonState[button].press)
        return;

    // no (extra) long press configured
    if (ParamBTN_ChannelOutputLong_DPT == 0 && ParamBTN_ChannelOutputExtraLong_DPT == 0)
        return;

    if (!_buttonState[button].pressLong && delayCheck(_buttonState[button].pressStart, _params.reactionTimeLong))
    {
        eventLongPress(button);
        _buttonState[button].pressLong = true;
    }

    // no extra long press configured
    if (ParamBTN_ChannelOutputExtraLong_DPT == 0)
        return;

    if (!_buttonState[button].pressExtraLong && delayCheck(_buttonState[button].pressStart, _params.reactionTimeExtraLong))
    {
        eventExtraLongPress(button);
        _buttonState[button].pressExtraLong = true;
    }
}

void VirtualButtonChannel::processPress(bool button)
{
    if (_lock)
        return;

    if (ParamBTN_ChannelMode == 3 && button == 0)
    {
        _buttonState[0].multiClicks += 1;
        _buttonState[0].multiClickTimer = millis();
    }
    else
    {
        eventShortPress(button);
    }

    _buttonState[button].press = true;
    _buttonState[button].pressLong = false;
    _buttonState[button].pressExtraLong = false;
    _buttonState[button].pressStart = millis();
}

void VirtualButtonChannel::processRelease(bool button)
{
    if (_buttonState[button].pressExtraLong)
    {
        eventExtraLongRelease(button);
    }
    else if (_buttonState[button].pressLong)
    {
        eventLongRelease(button);
    }
    else if (ParamBTN_ChannelMode == 3)
    {
        _buttonState[button].multiClickTimer = millis();
    }
    else
    {
        eventShortRelease(button);
    }

    // Reset MultiClick on Long or ExtraLong
    if (_buttonState[button].pressExtraLong || _buttonState[button].pressLong)
    {
        _buttonState[button].multiClicks = 0;
        _buttonState[button].multiClickTimer = 0;
    }

    // Reset all others
    _buttonState[button].press = false;
    _buttonState[button].pressLong = false;
    _buttonState[button].pressExtraLong = false;
    _buttonState[button].pressStart = 0;
}

void VirtualButtonChannel::processMultiClick()
{
    // no multiclick mode
    if (ParamBTN_ChannelMode != 3)
        return;

    // skip during a press
    if (_buttonState[0].press)
        return;

    // skip when no timer started
    if (_buttonState[0].multiClickTimer == 0)
        return;

    if (delayCheck(_buttonState[0].multiClickTimer, _params.reactionTimeMultiClick))
    {
        eventMultiClick(_buttonState[0].multiClicks);
        _buttonState[0].multiClickTimer = 0;
        _buttonState[0].multiClicks = 0;
    }
}

void VirtualButtonChannel::eventMultiClick(uint8_t clicks)
{
    if (ParamBTN_ChannelMultiClickCount)
        KoBTN_ChannelOutput1.value(clicks, DPT_DecimalFactor);

    if (clicks > BTN_MaxMuliClicks)
        return;

    uint8_t index = clicks - 1;
    uint16_t outputKo = BTN_KoChannelOutput1;
    MultiClickParams params = _multiClickParams[index];

    if (!params.active)
        return;

    // Sonderlocke für DTPT1
    if (ParamBTN_ChannelOutputMulti_DPT == 1)
        outputKo = BTN_KoChannelOutput4 + index;

    log("  Button %i: MultiClick %i clicks - type %i ko %i value %i", 0, clicks, ParamBTN_ChannelOutputMulti_DPT, outputKo, params.output);
    writeOutput(ParamBTN_ChannelOutputMulti_DPT, outputKo, params.output, _statusShort);
}
void VirtualButtonChannel::eventShortPress(bool button)
{
    if (!_buttonParams[button].outputShortPressActive)
        return;

    log("  Button %i: short press", button);

    // Output
    writeOutput(ParamBTN_ChannelOutputShort_DPT, BTN_KoChannelOutput1, _buttonParams[button].outputShortPress, _statusShort);
}

void VirtualButtonChannel::eventLongPress(bool button)
{
    if (!_buttonParams[button].outputLongPressActive)
        return;

    log("  Button %i: long press", button);

    // Output
    writeOutput(ParamBTN_ChannelOutputLong_DPT, BTN_KoChannelOutput2, _buttonParams[button].outputLongPress, _statusLong);
}

void VirtualButtonChannel::eventExtraLongPress(bool button)
{
    if (!_buttonParams[button].outputExtraLongPressActive)
        return;

    log("  Button %i: extra long press", button);

    // Output
    writeOutput(ParamBTN_ChannelOutputExtraLong_DPT, BTN_KoChannelOutput3, _buttonParams[button].outputExtraLongPress, _statusExtraLong);
}

void VirtualButtonChannel::eventShortRelease(bool button)
{
    if (!_buttonParams[button].outputShortReleaseActive && !_buttonParams[button].output2Short)
        return;

    log("  Button %i: short release", button);

    // Output 1
    if (_buttonParams[button].outputShortReleaseActive)
        writeOutput(ParamBTN_ChannelOutputShort_DPT, BTN_KoChannelOutput1, _buttonParams[button].outputShortRelease, _statusShort);

    // Output 2
    if (_buttonParams[button].output2Short)
        KoBTN_ChannelOutput4.value(_buttonParams[button].output2Short, DPT_Switch);
}

void VirtualButtonChannel::eventLongRelease(bool button)
{
    if (!_buttonParams[button].outputLongReleaseActive && !_buttonParams[button].output2Long)
        return;

    log("  Button %i: long release", button);

    // Output 1
    if (_buttonParams[button].outputLongReleaseActive)
        writeOutput(ParamBTN_ChannelOutputLong_DPT, BTN_KoChannelOutput2, _buttonParams[button].outputLongRelease, _statusLong);

    // Output 2
    if (_buttonParams[button].output2Long)
        KoBTN_ChannelOutput5.value(_buttonParams[button].output2Long, DPT_Switch);
}

void VirtualButtonChannel::eventExtraLongRelease(bool button)
{
    if (!_buttonParams[button].outputExtraLongReleaseActive && !_buttonParams[button].output2ExtraLong)
        return;

    log("  Button %i: extra long release", button);

    // Output 1
    if (_buttonParams[button].outputExtraLongReleaseActive)
        writeOutput(ParamBTN_ChannelOutputExtraLong_DPT, BTN_KoChannelOutput3, _buttonParams[button].outputExtraLongRelease, _statusExtraLong);

    // Output 2
    if (_buttonParams[button].output2ExtraLong)
        KoBTN_ChannelOutput6.value(_buttonParams[button].output2ExtraLong, DPT_Switch);
}

void VirtualButtonChannel::writeOutput(uint8_t outputDpt, uint16_t outputKo, uint16_t outputValue, bool &status)
{
    log("  writeOutput %i/%i/%i/%i // %i", outputDpt, outputKo, outputValue, status, BTN_KoCalcNumber(outputKo));

    switch (outputDpt)
    {
        case BTN_DPT1:
            // aus
            if (outputValue == 1)
                status = false;

            // an
            if (outputValue == 2)
                status = true;

            // toggle
            if (outputValue == 3)
            {
                status = !status;
            }

            knx.getGroupObject(BTN_KoCalcNumber(outputKo)).value((bool)status, DPT_Switch);
            break;
        case BTN_DPT2:
            // toggle
            if (outputValue >= 10)
            {
                status = !status;
                outputValue = (outputValue == 10 ? 0 : 2) + (uint8_t)status;
            }

            knx.getGroupObject(BTN_KoCalcNumber(outputKo)).value((uint8_t)outputValue, DPT_DecimalFactor);
            break;

        case BTN_DPT5:
            knx.getGroupObject(BTN_KoCalcNumber(outputKo)).value((uint8_t)outputValue, DPT_DecimalFactor);
            break;

        case BTN_DPT5001:
            knx.getGroupObject(BTN_KoCalcNumber(outputKo)).value((uint8_t)outputValue, DPT_Scaling);
            break;

        case BTN_DPT18:
            if (outputValue >= 100)
            {
                outputValue = (uint8_t)((outputValue - 101) | 0x81);
            }
            else
            {
                outputValue = (uint8_t)(outputValue - 1);
            }

            knx.getGroupObject(BTN_KoCalcNumber(outputKo)).value((uint8_t)outputValue, DPT_DecimalFactor);
            break;

        case BTN_DPT3007:
            // Start Timer for Status Fallback
            _dynamicStatusTimer = (outputValue == 0 || outputValue == 8 || outputValue == 16) ? millis() : 0;

            if (outputValue == 16) // 1-Taster Stop
                outputValue = status ? 8 : 0;

            if (outputValue == 17) // 1-Taster 100%
            {
                status = !status;
                outputValue = status ? 9 : 1;
            }

            knx.getGroupObject(BTN_KoCalcNumber(outputKo)).value((uint8_t)outputValue, DPT_DecimalFactor);
            break;

            // DPT3008
            if (outputValue == 16) // 1-Taster Stop
                outputValue = status ? 0 : 8;

            if (outputValue == 17) // 1-Taster 100%
            {
                status = !status;
                outputValue = status ? 1 : 9;
            }

            knx.getGroupObject(BTN_KoCalcNumber(outputKo)).value((uint8_t)outputValue, DPT_DecimalFactor);
            break;

        case BTN_DPT7:
            // 2Byte Int
            knx.getGroupObject(BTN_KoCalcNumber(outputKo)).value((uint16_t)outputValue, DPT_Value_2_Ucount);
            break;
    }
}

void VirtualButtonChannel::readStatus()
{
    std::set<int> valids{1, 2, 7, 8};

    if (valids.count(ParamBTN_ChannelOutputShort_DPT) > 0)
        if (KoBTN_ChannelOutput1Status.commFlag() == ComFlag::Uninitialized)
            KoBTN_ChannelOutput1Status.requestObjectRead();

    if (valids.count(ParamBTN_ChannelOutputLong_DPT) > 0)
        if (KoBTN_ChannelOutput2Status.commFlag() == ComFlag::Uninitialized)
            KoBTN_ChannelOutput2Status.requestObjectRead();

    if (valids.count(ParamBTN_ChannelOutputExtraLong_DPT) > 0)
        if (KoBTN_ChannelOutput3Status.commFlag() == ComFlag::Uninitialized)
            KoBTN_ChannelOutput3Status.requestObjectRead();
}