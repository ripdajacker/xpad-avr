#include "XpadEmu.h"
#include "LUFA/Drivers/Peripheral/Serial.h"

static const uint8_t desc1[] = {0x12, 0x01, 0x10, 0x01, 0x00, 0x00, 0x00, 0x40};
static const uint8_t desc2[] = {0x09, 0x02, 0x29, 0x00, 0x01, 0x01, 0x00, 0x80, 0x32, 0x09, 0x04, 0x00, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00, 0x09, 0x21, 0x10, 0x01, 0x00, 0x01, 0x22, 0x65, 0x00, 0x07, 0x05, 0x81, 0x03, 0x40, 0x00, 0x04, 0x07, 0x05, 0x02, 0x03, 0x40, 0x00, 0x04};
static const uint8_t desc3[] = {0x10, 0x42, 0x00, 0x01, 0x01, 0x01, 0x14, 0x06, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};


void printControlRequest(char *string) {
    printf(string);
    printf(" 0x%x 0x%x 0x%x 0x%x 0x%x\n",USB_ControlRequest.bmRequestType, USB_ControlRequest.bRequest, USB_ControlRequest.wValue, USB_ControlRequest.wIndex, USB_ControlRequest.wLength);
}

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
    if (tick > (unsigned long) 3000) {
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

    Serial_Init(115200, false);
    Serial_CreateStream(NULL);

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
    printControlRequest("Processing req");
    if (USB_ControlRequest.bRequest == 0x6 ) {
        
        if(USB_ControlRequest.bmRequestType == 0x80 && USB_ControlRequest.wValue == 0x100) {
            Endpoint_ClearSETUP();
            Endpoint_Write_Control_Stream_LE(&desc1, USB_ControlRequest.wLength);
            Endpoint_ClearOUT();
            return;
        }
        if (USB_ControlRequest.bmRequestType == 0xC1 && USB_ControlRequest.wValue == 0x4200) {
            Endpoint_ClearSETUP();
            switch(USB_ControlRequest.wLength) {
                case 0x50:
                    Endpoint_Write_Control_Stream_LE(&desc2, USB_ControlRequest.wLength);
                    break;
                case 0x10:
                    Endpoint_Write_Control_Stream_LE(&desc3, USB_ControlRequest.wLength);
                    break;
                case 0x06:
                    Endpoint_Write_Control_Stream_LE(&desc1, USB_ControlRequest.wLength);
                    break;
            }
            Endpoint_ClearOUT();
            return;
        }        
    } 
    HID_Device_ProcessControlRequest(&Xpad_HID_Interface);    
}

void EVENT_USB_Device_StartOfFrame(void) {
    HID_Device_MillisecondElapsed(&Xpad_HID_Interface);
    tick++;
}


bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t *const HIDInterfaceInfo, uint8_t *const ReportID, const uint8_t ReportType, void *ReportData,uint16_t *const ReportSize) {
    USB_XpadReport_Data_t *XpadReport = (USB_XpadReport_Data_t *) ReportData;
    if (state) {
        XpadReport->BUTTON_A = 255;
    }   else {
        XpadReport->BUTTON_B = 255;        
    }

    XpadReport->LENGTH =  0x14;
    XpadReport->UNUSED_0 = 0;
    
    *ReportSize = 0x14;
    printControlRequest("Get report");
    return false;
}
    
    
void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t *const HIDInterfaceInfo, const uint8_t ReportID, const uint8_t ReportType, const void *ReportData, const uint16_t ReportSize) {
    printControlRequest("Set report");
    if(ReportSize == 6) {
        printf("Lets get ready to rumble\n");
    }                        
}



