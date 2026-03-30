#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "Ifx_Cfg_Ssw.h"
#include "IfxPort.h"
#include "IfxPort_PinMap.h"
#include "IfxScuEru.h"
#include "IfxSrc.h"

#include "stop_bell.h"

IFX_ALIGN(4) IfxCpu_syncEvent cpuSyncEvent = 0;

void initERU();

IFX_INTERRUPT(onStopBtnISR, 0, STOP_BTN_ON_ISR_PRIORITY);

void onStopBtnISR(void);

void core0_main(void)
{
    IfxCpu_enableInterrupts();

    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

    IfxCpu_emitEvent(&cpuSyncEvent);
    IfxCpu_waitEvent(&cpuSyncEvent, 1);

    initGPIO();
    initERU();

    IfxPort_setPinLow(STOP_LED_1.port, STOP_LED_1.pinIndex); // 기본 LED 끔

    while (1)
    {
        // 하차벨 초기화 버튼
        if (IfxPort_getPinState(STOP_BTN_OFF.port, STOP_BTN_OFF.pinIndex) == 0) // 안 누르면
        {
            offStopButtonLED();
        }
    }
}


void onStopBtnISR()
{
    onStopButtonLED();
}
