#ifndef _SPI_BUS_H_
#define _SPI_BUS_H_

#include "DataTypes.h"

void SPI_init(void);
void SPI_sendRumble(USB_XpadRumble_Data_t *rumble);
void SPI_setLastReport(USB_XpadReport_Data_t *report);

#endif
