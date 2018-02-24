#ifndef _XPAD_H_
#define _XPAD_H_

#include <string.h>

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#include <LUFA/Drivers/USB/USB.h>

#include "Descriptors.h"


/** 
 * Type define for the Xpad HID report structure.
 */
typedef struct {
    uint8_t UNUSED_0;
    uint8_t LENGTH; // always 0x14

    uint8_t DPAD_UP      :1; 
    uint8_t DPAD_DOWN    :1; 
    uint8_t DPAD_LEFT    :1; 
    uint8_t DPAD_RIGHT   :1; 
    
    uint8_t BUTTON_START :1; 
    uint8_t BUTTON_BACK  :1; 

    uint8_t BUTTON_L3    :1; 
    uint8_t BUTTON_R3    :1; 

    uint8_t UNUSED_1;
    
    uint8_t BUTTON_A;
    uint8_t BUTTON_B;
    uint8_t BUTTON_X;
    uint8_t BUTTON_Y;
    
    uint8_t BUTTON_BLACK;
    uint8_t BUTTON_WHITE;
    
    uint8_t TRIGGER_LEFT;
    uint8_t TRIGGER_RIGHT;
    
    uint16_t LEFT_STICK_X;
    uint16_t LEFT_STICK_Y;
    uint16_t RIGHT_STICK_X;
    uint16_t RIGHT_STICK_Y;

} USB_XpadReport_Data_t;

void setup(void);

void EVENT_USB_Device_Connect(void);

void EVENT_USB_Device_Disconnect(void);

void EVENT_USB_Device_ConfigurationChanged(void);

void EVENT_USB_Device_ControlRequest(void);

void EVENT_USB_Device_StartOfFrame(void);

bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t *const HIDInterfaceInfo,
                                         uint8_t *const ReportID,
                                         const uint8_t ReportType,
                                         void *ReportData,
                                         uint16_t *const ReportSize);

void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t *const IDInterfaceInfo, 
                                         const uint8_t ReportID, 
                                         const uint8_t ReportType, 
                                         const void *ReportData, 
                                         const uint16_t ReportSize);

#endif

