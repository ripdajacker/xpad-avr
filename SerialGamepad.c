#include <SerialGamepad.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <LUFA/Drivers/Peripheral/Serial.h>

volatile  static  uint8_t CURRENT_READ_INDEX = 0x0;
uint8_t tempReport[SERIAL_REPORT_LEN];

/**
 * Interrupt for every time a byte is received through the serial port.
 */
ISR(USART1_RX_vect) {
    if(CURRENT_READ_INDEX < SERIAL_REPORT_LEN) {
        tempReport[CURRENT_READ_INDEX] = UDR1;
        CURRENT_READ_INDEX++;
    }
}

uint8_t SERIAL_GetIndex() {
    return CURRENT_READ_INDEX;
}

bool SERIAL_IsReportReady() {
    return CURRENT_READ_INDEX == SERIAL_REPORT_LEN;
}

void SERIAL_ReadReport(USB_XpadReport_Data_t *reportPointer) { 
    uint8_t checksum = 0;
    for (uint8_t index = 0; index < SERIAL_REPORT_LEN - 1; index++) {
        checksum ^= tempReport[index];
    }

    if(checksum != tempReport[SERIAL_REPORT_LEN - 1]) { 
  //      return;
  // TODO
    }

    memcpy(reportPointer, &tempReport, sizeof(&reportPointer));
    
    printf("Report was: ");
    for (uint8_t index = 0; index < SERIAL_REPORT_LEN; index++) {
        printf("0x%x ",tempReport[index]);
        tempReport[index] = 0;
    }

    printf("\n");
    CURRENT_READ_INDEX = 0;
}

void SERIAL_WriteReport(USB_XpadRumble_Data_t * dataPointer) {
    Serial_SendData(&dataPointer, sizeof(dataPointer));
}
