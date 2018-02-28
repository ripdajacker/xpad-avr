#ifndef _XPAD_H_
#define _XPAD_H_

#include <string.h>

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#include <LUFA/Drivers/USB/USB.h>

#include <Descriptors.h>
#include <DataTypes.h>


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

