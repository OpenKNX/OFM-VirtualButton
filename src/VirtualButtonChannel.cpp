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

bool VirtualButtonChannel::isActive()
{
    return ParamBTN_bMode > 0;
}

void VirtualButtonChannel::setup()
{
    // Input
    _buttonParams[0].inputKo = ParamBTN_bInA;
    _buttonParams[1].inputKo = ParamBTN_bInB;

    //   Events
    _buttonParams[0].outputShortPressActive = ParamBTN_bOutShort_T1_Active_Press;
    _buttonParams[0].outputShortReleaseActive = ParamBTN_bOutShort_T1_Active_Release;
    _buttonParams[1].outputShortPressActive = ParamBTN_bOutShort_T2_Active_Press;
    _buttonParams[1].outputShortReleaseActive = ParamBTN_bOutShort_T2_Active_Release;

    _buttonParams[0].outputLongPressActive = ParamBTN_bOutLong_T1_Active_Press;
    _buttonParams[0].outputLongReleaseActive = ParamBTN_bOutLong_T1_Active_Release;
    _buttonParams[1].outputLongPressActive = ParamBTN_bOutLong_T2_Active_Press;
    _buttonParams[1].outputLongReleaseActive = ParamBTN_bOutLong_T2_Active_Release;

    _buttonParams[0].outputExtraLongPressActive = ParamBTN_bOutExtraLong_T1_Active_Press;
    _buttonParams[0].outputExtraLongReleaseActive = ParamBTN_bOutExtraLong_T1_Active_Release;
    _buttonParams[1].outputExtraLongPressActive = ParamBTN_bOutExtraLong_T2_Active_Press;
    _buttonParams[1].outputExtraLongReleaseActive = ParamBTN_bOutExtraLong_T2_Active_Release;

    //   Outputs - DPT1 ist stellvertretend für alle DPTs
    _buttonParams[0].outputShortPress = ParamBTN_bOutShort_T1_Dpt1_Press;
    _buttonParams[0].outputShortRelease = ParamBTN_bOutShort_T1_Dpt1_Release;
    _buttonParams[1].outputShortPress = ParamBTN_bOutShort_T2_Dpt1_Press;
    _buttonParams[1].outputShortRelease = ParamBTN_bOutShort_T2_Dpt1_Release;

    _buttonParams[0].outputLongPress = ParamBTN_bOutLong_T1_Dpt1_Press;
    _buttonParams[0].outputLongRelease = ParamBTN_bOutLong_T1_Dpt1_Release;
    _buttonParams[1].outputLongPress = ParamBTN_bOutLong_T2_Dpt1_Press;
    _buttonParams[1].outputLongRelease = ParamBTN_bOutLong_T2_Dpt1_Release;

    _buttonParams[0].outputExtraLongPress = ParamBTN_bOutExtraLong_T1_Dpt1_Press;
    _buttonParams[0].outputExtraLongRelease = ParamBTN_bOutExtraLong_T1_Dpt1_Release;
    _buttonParams[1].outputExtraLongPress = ParamBTN_bOutExtraLong_T2_Dpt1_Press;
    _buttonParams[1].outputExtraLongRelease = ParamBTN_bOutExtraLong_T2_Dpt1_Release;

    // Output 2
    _buttonParams[0].output2Short = ParamBTN_bOut2Short_T1;
    _buttonParams[1].output2Short = ParamBTN_bOut2Short_T2;
    _buttonParams[0].output2Long = ParamBTN_bOut2Long_T1;
    _buttonParams[1].output2Long = ParamBTN_bOut2Long_T2;
    _buttonParams[0].output2ExtraLong = ParamBTN_bOut2ExtraLong_T1;
    _buttonParams[1].output2ExtraLong = ParamBTN_bOut2ExtraLong_T2;

    // MultiClick
    _multiClickParams[0].active = ParamBTN_bOutMulti_Click1_Active;
    _multiClickParams[1].active = ParamBTN_bOutMulti_Click2_Active;
    _multiClickParams[2].active = ParamBTN_bOutMulti_Click3_Active;

    // DPT2 ist stellvertretend für alle DPTs (DPT1 nicht ntuzbar da für die einzeldklicks in Verwendung)
    _multiClickParams[0].output = ParamBTN_bOutMulti_Click1_Dpt1;
    _multiClickParams[1].output = ParamBTN_bOutMulti_Click2_Dpt1;
    _multiClickParams[2].output = ParamBTN_bOutMulti_Click3_Dpt1;

    // ReactionTimes
    _params.reactionTimeMultiClick = ParamBTN_bReactionTimeMultiClick;
    if (_params.reactionTimeMultiClick > 0)
        _params.reactionTimeMultiClick *= 100;
    else
        _params.reactionTimeMultiClick = ParamBTN_ReactionTimeMultiClick * 100;

    _params.reactionTimeLong = ParamBTN_bReactionTimeLong;
    if (_params.reactionTimeLong > 0)
        _params.reactionTimeLong *= 100;
    else
        _params.reactionTimeLong = ParamBTN_ReactionTimeLong * 100;

    _params.reactionTimeExtraLong = ParamBTN_bReactionTimeExtraLong;
    if (_params.reactionTimeExtraLong > 0)
        _params.reactionTimeExtraLong *= 100;
    else
        _params.reactionTimeExtraLong = ParamBTN_ReactionTimeExtraLong * 100;

    // Debug
    // logInfoP("_buttonParams[0].inputKo: %i", _buttonParams[0].inputKo);
    // logInfoP("_buttonParams[1].inputKo: %i", _buttonParams[1].inputKo);
    // logInfoP("_buttonParams[0].outputShortPressActive: %i", _buttonParams[0].outputShortPressActive);
    // logInfoP("_buttonParams[0].outputShortReleaseActive: %i", _buttonParams[0].outputShortReleaseActive);
    // logInfoP("_buttonParams[0].outputLongPressActive: %i", _buttonParams[0].outputLongPressActive);
    // logInfoP("_buttonParams[0].outputLongReleaseActive: %i", _buttonParams[0].outputLongReleaseActive);
    // logInfoP("_buttonParams[0].outputShortRelease: %i", _buttonParams[0].outputShortRelease);
    // logInfoP("_buttonParams[0].outputLongPress: %i", _buttonParams[0].outputLongPress);
    // logInfoP("_buttonParams[0].outputLongRelease: %i", _buttonParams[0].outputLongRelease);
    // logInfoP("ParamBTN_bStatusFallbackTime: %i", ParamBTN_bStatusFallbackTime * 1000);
    // logInfoP("ParamBTN_bStatusThresholdHigh: %i", ParamBTN_bStatusThresholdHigh);
    // logInfoP("ParamBTN_bStatusThresholdLow: %i", ParamBTN_bStatusThresholdLow);
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
    if (!ParamBTN_bDynamicStatus)
        return;

    // Timer muss aktiv sein
    if (_dynamicStatusTimer && delayCheck(_dynamicStatusTimer, ParamBTN_bStatusFallbackTime * 1000))
    {
        logTraceP("processDynamicStatusTimer");
        logIndentUp();
        _dynamicStatusTimer = 0;
        evaluateDynamicStatus();
        logIndentDown();
    }
}

void VirtualButtonChannel::evaluateDynamicStatus()
{
    // und nur wenn nicht gerade gedimmt wird
    if (_buttonState[0].pressLong)
        return;

    uint8_t value = KoBTN_Out2Status.value(DPT_Scaling);
    logTraceP("evaluateDynamicStatus: %i", value);
    logIndentUp();

    if (value >= ParamBTN_bStatusThresholdHigh && !_statusLong)
    {
        logTraceP("evaluateDynamicStatus %i >= %i (true)", value, ParamBTN_bStatusThresholdHigh);
        _statusLong = true;
    }

    else if (value <= ParamBTN_bStatusThresholdLow && _statusLong)
    {
        logTraceP("evaluateDynamicStatus %i <= %i (false)", value, ParamBTN_bStatusThresholdLow);
        _statusLong = false;
    }

    logIndentDown();
}

void VirtualButtonChannel::processInputKo(GroupObject &ko)
{
    if (ParamBTN_bMode == 0)
        return;

    uint16_t koNumber = ko.asap();
    int8_t koIndex = BTN_KoCalcIndex(koNumber);
    // logInfoP("koNumber %i -> koIndex %i", koNumber, koIndex);

    // External KO -> to Channel Mapping
    if (_buttonParams[0].inputKo > 0 && _buttonParams[0].inputKo == koNumber)
    {
        koIndex = BTN_KoIn1;
    }
    else if (_buttonParams[1].inputKo > 0 && _buttonParams[1].inputKo == koNumber)
    {
        koIndex = BTN_KoIn2;
    }

    switch (koIndex)
    {
        case BTN_KoIn1:
            processInputKoInput(ko, 0);
            break;
        case BTN_KoIn2:
            processInputKoInput(ko, 1);
            break;
        case BTN_KoLock:
            processInputKoLock(ko);
            break;
        case BTN_KoOut1Status:
            processInputKoStatus(ko, 1, ParamBTN_bOutShort_DPT, _statusShort);
            break;
        case BTN_KoOut2Status:
            processInputKoStatus(ko, 2, ParamBTN_bOutLong_DPT, _statusLong);
            break;
        case BTN_KoOut3Status:
            processInputKoStatus(ko, 3, ParamBTN_bOutExtraLong_DPT, _statusExtraLong);
            break;
    }
}

void VirtualButtonChannel::processInputKoStatus(GroupObject &ko, uint8_t statusNumber, uint8_t dpt, bool &status)
{
    // logTraceP("processInputKoStatus %i/%i/%i", statusNumber, dpt, status);

    if (dpt == 7 || dpt == 8) // 3.000x (long only)
    {
        // nur wenn nicht gerade gedimmt wird
        if (_buttonState[0].pressLong)
            return;

        uint8_t value = ko.value(DPT_Scaling);

        /*
         * Sonderfall
         * Es wird noch ein weiterer Status gespeichert, damit bei Änderung des Status AN/AUS zusätzlich eine Umkehr gemacht werden kann.
         * Bedeutet: Schalte ich aus (nächster fahrt geht hoch) und jemand schaltet woanders auf 80%, dann soll dieser Taster auch runter fahren.
         */

        if (!_statusLongLast && value > 0)
        {
            _statusLong = true;
            _statusLongLast = true;
        }
        if (_statusLongLast && value == 0)
        {
            _statusLong = false;
            _statusLongLast = false;
        }

        if (value == 100 && !_statusLong)
        {
            _statusLong = true;
            return;
        }

        else if (value == 0 && _statusLong)
        {
            _statusLong = false;
            return;
        }

        // Sonderlocke 1-Tasten Dimmen
        if (_buttonParams[0].outputLongPress == 17)
        {
            // und wenn kein timer läuft
            if (_dynamicStatusTimer)
                return;

            // wird der dynamic status genutzt dann muss
            if (ParamBTN_bDynamicStatus)
                evaluateDynamicStatus();
        }
    }
    else
    {
        bool value = ko.value(DPT_Switch);
        status = value;
    }
}

void VirtualButtonChannel::processInputKoLock(GroupObject &ko)
{
    if (ParamBTN_bLock == 0)
        return;

    bool value = ko.value(DPT_Switch);

    if (ParamBTN_bLock == 1)
        _lock = value;

    if (ParamBTN_bLock == 2)
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

    logTraceP("processInputKoLock %i", _lock);
}

void VirtualButtonChannel::processInputKoInput(GroupObject &ko, bool button)
{
    bool newPress = ko.value(DPT_Switch);
    bool lastPress = _buttonState[button].press;
    logTraceP("processInputKoInput %i", newPress);

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
    if (ParamBTN_bOutLong_DPT == 0 && ParamBTN_bOutExtraLong_DPT == 0)
        return;

    if (!_buttonState[button].pressLong && delayCheck(_buttonState[button].pressStart, _params.reactionTimeLong))
    {
        eventLongPress(button);
        _buttonState[button].pressLong = true;
    }

    // no extra long press configured
    if (ParamBTN_bOutExtraLong_DPT == 0)
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

    if (ParamBTN_bMode == 3 && button == 0)
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
    else if (ParamBTN_bMode == 3)
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
    if (ParamBTN_bMode != 3)
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
    if (ParamBTN_bMultiClickCount)
        KoBTN_Out1.value(clicks, DPT_DecimalFactor);

    if (clicks > BTN_MaxMuliClicks)
        return;

    uint8_t index = clicks - 1;
    uint16_t outputKo = BTN_KoOut1;
    MultiClickParams params = _multiClickParams[index];

    if (!params.active)
        return;

    // Sonderlocke für DTPT1
    if (ParamBTN_bOutMulti_DPT == 1)
        outputKo = BTN_KoOut4 + index;

    logDebugP("  Button %i: MultiClick %i clicks - type %i ko %i value %i", 0, clicks, ParamBTN_bOutMulti_DPT, outputKo, params.output);
    writeOutput(ParamBTN_bOutMulti_DPT, outputKo, params.output, _statusShort);
}

void VirtualButtonChannel::eventShortPress(bool button)
{
    if (!_buttonParams[button].outputShortPressActive)
        return;

    logDebugP("  Button %i: short press", button);

    // Output
    writeOutput(ParamBTN_bOutShort_DPT, BTN_KoOut1, _buttonParams[button].outputShortPress, _statusShort);
}

void VirtualButtonChannel::eventLongPress(bool button)
{
    if (!_buttonParams[button].outputLongPressActive)
        return;

    logDebugP("  Button %i: long press", button);

    // Output
    writeOutput(ParamBTN_bOutLong_DPT, BTN_KoOut2, _buttonParams[button].outputLongPress, _statusLong);
}

void VirtualButtonChannel::eventExtraLongPress(bool button)
{
    if (!_buttonParams[button].outputExtraLongPressActive)
        return;

    logDebugP("  Button %i: extra long press", button);

    // Output
    writeOutput(ParamBTN_bOutExtraLong_DPT, BTN_KoOut3, _buttonParams[button].outputExtraLongPress, _statusExtraLong);
}

void VirtualButtonChannel::eventShortRelease(bool button)
{
    if (!_buttonParams[button].outputShortReleaseActive && !_buttonParams[button].output2Short)
        return;

    logDebugP("  Button %i: short release", button);

    // Output 1
    if (_buttonParams[button].outputShortReleaseActive)
        writeOutput(ParamBTN_bOutShort_DPT, BTN_KoOut1, _buttonParams[button].outputShortRelease, _statusShort);

    // Output 2
    if (_buttonParams[button].output2Short)
        KoBTN_Out4.value(_buttonParams[button].output2Short, DPT_Switch);
}

void VirtualButtonChannel::eventLongRelease(bool button)
{
    if (!_buttonParams[button].outputLongReleaseActive && !_buttonParams[button].output2Long)
        return;

    logDebugP("  Button %i: long release", button);

    // Output 1
    if (_buttonParams[button].outputLongReleaseActive)
        writeOutput(ParamBTN_bOutLong_DPT, BTN_KoOut2, _buttonParams[button].outputLongRelease, _statusLong);

    // Output 2
    if (_buttonParams[button].output2Long)
        KoBTN_Out5.value(_buttonParams[button].output2Long, DPT_Switch);
}

void VirtualButtonChannel::eventExtraLongRelease(bool button)
{
    if (!_buttonParams[button].outputExtraLongReleaseActive && !_buttonParams[button].output2ExtraLong)
        return;

    logDebugP("  Button %i: extra long release", button);

    // Output 1
    if (_buttonParams[button].outputExtraLongReleaseActive)
        writeOutput(ParamBTN_bOutExtraLong_DPT, BTN_KoOut3, _buttonParams[button].outputExtraLongRelease, _statusExtraLong);

    // Output 2
    if (_buttonParams[button].output2ExtraLong)
        KoBTN_Out6.value(_buttonParams[button].output2ExtraLong, DPT_Switch);
}

void VirtualButtonChannel::writeOutput(uint8_t outputDpt, uint16_t outputKo, uint16_t outputValue, bool &status)
{
    logTraceP("  writeOutput %i/%i/%i/%i // %i", outputDpt, outputKo, outputValue, status, BTN_KoCalcNumber(outputKo));

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
        case BTN_DPT3008:
            _dynamicStatusTimer = 0;
            if (outputValue == 16) // 1-Taster Stop
            {
                outputValue = status ? 8 : 0;
                if (ParamBTN_bDynamicStatus)
                {
                    logTraceP("Start dynamic status timer");
                    _dynamicStatusTimer = millis();
                }
            }
            else if (outputValue == 17) // 1-Taster 100%
            {
                status = !status;
                outputValue = status ? 9 : 1;
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

    if (valids.count(ParamBTN_bOutShort_DPT) > 0)
        if (KoBTN_Out1Status.commFlag() == ComFlag::Uninitialized)
            KoBTN_Out1Status.requestObjectRead();

    if (valids.count(ParamBTN_bOutLong_DPT) > 0)
        if (KoBTN_Out2Status.commFlag() == ComFlag::Uninitialized)
            KoBTN_Out2Status.requestObjectRead();

    if (valids.count(ParamBTN_bOutExtraLong_DPT) > 0)
        if (KoBTN_Out3Status.commFlag() == ComFlag::Uninitialized)
            KoBTN_Out3Status.requestObjectRead();
}