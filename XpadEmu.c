#include "XpadEmu.h"

static uint8_t PrevXpadHIDReportBuffer[sizeof(USB_XpadReport_Data_t)];

USB_ClassInfo_HID_Device_t Xpad_HID_Interface =
{
    .Config =
    {
        .InterfaceNumber              = 0,
        .ReportINEndpoint             = {
            .Address              = (ENDPOINT_DIR_IN | 1),
            .Size                 = 0x14,
            .Banks                = 1,
        },
        .PrevReportINBuffer           = PrevXpadHIDReportBuffer,
        .PrevReportINBufferSize       = sizeof(PrevXpadHIDReportBuffer),
    },
};

int8_t state = 1;
unsigned long tick = 0;

void flash_led(void) {
    tick++;
    
    if (tick > (unsigned long) 50000) {
        state = !state;
        tick = 0;
        PORTC = 0;
        if (state) {
            PORTC |= 1 << 7;
        }
    }
}


/** Main program entry point. This routine contains the overall program flow, including initial
*  setup of all components and the main program loop.
*/
int main(void) {
    setup();
    
    GlobalInterruptEnable();
    for (;;) {
        flash_led();

        HID_Device_USBTask(&Xpad_HID_Interface);
        USB_USBTask();
    }
}

void setup(void) {
    MCUSR &= ~(1 << WDRF);
    wdt_disable();
    clock_prescale_set(clock_div_1);

    DDRC |= (1 << 7);
    sei();
    USB_Init();
}

void EVENT_USB_Device_Connect(void) {}

void EVENT_USB_Device_Disconnect(void) {}

void EVENT_USB_Device_ConfigurationChanged(void) {
    bool ConfigSuccess = true;
    ConfigSuccess &= HID_Device_ConfigureEndpoints(&Xpad_HID_Interface);
    USB_Device_EnableSOFEvents();
}

void EVENT_USB_Device_ControlRequest(void) {
    HID_Device_ProcessControlRequest(&Xpad_HID_Interface);
}

void EVENT_USB_Device_StartOfFrame(void) {
    HID_Device_MillisecondElapsed(&Xpad_HID_Interface);
}


bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t *const HIDInterfaceInfo, uint8_t *const ReportID, const uint8_t ReportType, void *ReportData,uint16_t *const ReportSize) {

    USB_XpadReport_Data_t *XpadReport = (USB_XpadReport_Data_t *) ReportData;
    
    if (state) {
        XpadReport->BUTTON_A = 255;
        /*
        XpadReport->BUTTON_X = 255;
        XpadReport->BUTTON_Y = 255;

        XpadReport->BUTTON_BLACK = 255;
        XpadReport->BUTTON_WHITE = 255;
        
        XpadReport->DPAD_UP = 0x01;
        XpadReport->DPAD_DOWN = 0x01;
        XpadReport->DPAD_LEFT = 0x01;
        XpadReport->DPAD_RIGHT = 0x01;

        XpadReport->BUTTON_START = 0x01;
        XpadReport->BUTTON_BACK = 0x01;

        XpadReport->BUTTON_L3  = 0x01;
        XpadReport->BUTTON_R3 = 0x01;
        
        XpadReport->TRIGGER_LEFT = 0xFF;
        XpadReport->TRIGGER_RIGHT = 0xFF;

        XpadReport->LEFT_STICK_X = 0x7FFF;
        XpadReport->LEFT_STICK_Y = 0x8000;

        XpadReport->RIGHT_STICK_X = 0x8000;
        XpadReport->RIGHT_STICK_Y = 0x7FFF;
        */
    }   else {
        XpadReport->BUTTON_B = 255;        
    }

    XpadReport->LENGTH =  0x14;
    XpadReport->UNUSED_0 = 0;
    
    *ReportSize = 0x14;
    return false;
}
    
    
void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t *const HIDInterfaceInfo, const uint8_t ReportID, const uint8_t ReportType, const void *ReportData,
    const uint16_t ReportSize) {
    if(ReportSize == 6) {

    }                        
}        