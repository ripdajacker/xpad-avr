#include <SerialGamepad.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <LUFA/Drivers/Peripheral/Serial.h>
#include <LUFA/Common/Common.h>

uint8_t CURRENT_READ_INDEX   = 0;
uint8_t tempReport[SERIAL_REPORT_LEN];


uint8_t SERIAL_GetIndex() {
    return CURRENT_READ_INDEX;
}

bool SERIAL_IsReportReady() {
    return CURRENT_READ_INDEX == (SERIAL_REPORT_LEN - 1) && tempReport[0] == 0 && tempReport[1] == 0x14;
}

void SERIAL_ReadReport(USB_XpadReport_Data_t *reportPointer) { 
    uint8_t buttonState1 = tempReport[2];    
    uint8_t buttonState2 = tempReport[2];
 
    reportPointer->DPAD_UP        = buttonState1 & (1 << 0);
    reportPointer->DPAD_DOWN      = buttonState1 & (1 << 1);
    reportPointer->DPAD_LEFT      = buttonState1 & (1 << 2);
    reportPointer->DPAD_RIGHT     = buttonState1 & (1 << 3);
 
    reportPointer->BUTTON_A       = buttonState1 & (1 << 4);
    reportPointer->BUTTON_B       = buttonState1 & (1 << 6);
    reportPointer->BUTTON_X       = buttonState1 & (1 << 7);
    reportPointer->BUTTON_Y       = buttonState1 & (1 << 7);
 
    
    reportPointer->BUTTON_L3      = buttonState2 & (1 << 0);
    reportPointer->BUTTON_R3      = buttonState2 & (1 << 1);
    reportPointer->BUTTON_BLACK   = buttonState2 & (1 << 2);
    reportPointer->BUTTON_WHITE   = buttonState2 & (1 << 3);
    reportPointer->BUTTON_START   = buttonState2 & (1 << 4);
    reportPointer->BUTTON_BACK    = buttonState2 & (1 << 5);

    reportPointer->TRIGGER_LEFT   = tempReport[3];
    reportPointer->TRIGGER_RIGHT  = tempReport[4];

    reportPointer->LEFT_STICK_X   = tempReport[5] & (tempReport[6] << 8);
    reportPointer->LEFT_STICK_Y   = tempReport[7] & (tempReport[8] << 8);
    reportPointer->RIGHT_STICK_X  = tempReport[9] & (tempReport[10] << 8);
    reportPointer->RIGHT_STICK_Y  = tempReport[11] & (tempReport[12] << 8);

    printf("Report was: ");
    for (uint8_t index = 0; index < SERIAL_REPORT_LEN; index++) {
         printf("0x%x ", tempReport[index]);
    }
    printf("\n");

    CURRENT_READ_INDEX = 0;
}

void SERIAL_WriteReport(USB_XpadRumble_Data_t * dataPointer) {
    Serial_SendData(&dataPointer, sizeof(dataPointer));
}

/**
 * Interrupt for every time a byte is received through the serial port.
 */
ISR(USART1_RX_vect, ISR_BLOCK) {    
    unsigned char tmp;
    while (UCSR1A & (1<<RXC1)) {
        tmp = UDR1;
        if(CURRENT_READ_INDEX < SERIAL_REPORT_LEN) {
            tempReport[CURRENT_READ_INDEX++] = tmp;    
        }
    }
}
