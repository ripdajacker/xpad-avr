#ifndef _SERIAL_GAMEPAD_H_
#define _SERIAL_GAMEPAD_H_

#include <DataTypes.h>
#include <stdbool.h>
#include <stdint.h>

#define SERIAL_REPORT_LEN  0x15

uint8_t SERIAL_GetIndex(void);

bool SERIAL_IsReportReady(void);

void SERIAL_ReadReport(USB_XpadReport_Data_t *reportPointer);

void SERIAL_WriteReport(USB_XpadRumble_Data_t * dataPointer);

#endif

