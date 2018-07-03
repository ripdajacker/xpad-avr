#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

#include <LUFA/Drivers/USB/USB.h>
#include <avr/pgmspace.h>

/**
 * The XPAD controller is a HID joystick without the HID joystick descriptor.
 */
typedef struct {
    USB_Descriptor_Configuration_Header_t Config;

    USB_Descriptor_Interface_t HID_Interface;
    USB_Descriptor_Endpoint_t HID_ReportINEndpoint;
    USB_Descriptor_Endpoint_t HID_ReportOUTEndpoint;
} USB_Descriptor_Configuration_t;

enum StringDescriptors_t {
    STRING_ID_Language = 0,
    STRING_ID_Manufacturer = 1,
    STRING_ID_Product = 2,
};

uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
                                    const uint16_t wIndex,
                                    const void **const DescriptorAddress)

    ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);

#endif
