#ifndef _DATA_TYPES_H_
#define _DATA_TYPES_H_
#include <stdint.h>
/**
 * Type define for the Xpad HID report structure.
 */
typedef struct {
    uint8_t UNUSED_0;
    uint8_t LENGTH;  // always 0x14

    uint8_t DPAD_UP : 1;
    uint8_t DPAD_DOWN : 1;
    uint8_t DPAD_LEFT : 1;
    uint8_t DPAD_RIGHT : 1;

    uint8_t BUTTON_START : 1;
    uint8_t BUTTON_BACK : 1;
    uint8_t BUTTON_L3 : 1;
    uint8_t BUTTON_R3 : 1;

    uint8_t UNUSED_1;

    uint8_t BUTTON_A;
    uint8_t BUTTON_B;
    uint8_t BUTTON_X;
    uint8_t BUTTON_Y;

    uint8_t BUTTON_BLACK;
    uint8_t BUTTON_WHITE;

    uint8_t TRIGGER_LEFT;
    uint8_t TRIGGER_RIGHT;

    int16_t LEFT_STICK_X;
    int16_t LEFT_STICK_Y;
    int16_t RIGHT_STICK_X;
    int16_t RIGHT_STICK_Y;

} __attribute__((packed)) USB_XpadReport_Data_t;

typedef struct {
    uint8_t UNUSED_0;
    uint8_t LENGTH;  // always 0x06

    uint8_t LEFT_RUMBLE;
    uint8_t RIGHT_RUMBLE;

    uint8_t PADDING[16];
} __attribute__((packed)) USB_XpadRumble_Data_t;

typedef struct {
    USB_XpadReport_Data_t XPAD_REPORT;
    uint16_t ACTIVE_NEXT_MILLISECONDS;
} __attribute__((packed)) SERIAL_ExtendedReport_Data_t;

#endif