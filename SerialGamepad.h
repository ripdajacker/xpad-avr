#include <XpadEmu.h>

bool SERIAL_IsReportReady();

void SERIAL_ReadReport(USB_XpadReport_Data_t *reportPointer);

void SERIAL_WriteReport(USB_XpadRumble_Data_t * dataPointer);