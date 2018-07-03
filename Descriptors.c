#include "Descriptors.h"

const USB_Descriptor_Device_t PROGMEM DeviceDescriptor = {
    .Header = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device},

    .USBSpecification = VERSION_BCD(1, 1, 0),
    .Class = USB_CSCP_NoDeviceClass,
    .SubClass = USB_CSCP_NoDeviceSubclass,
    .Protocol = USB_CSCP_NoDeviceProtocol,

    .Endpoint0Size = FIXED_CONTROL_ENDPOINT_SIZE,

    .VendorID = 0x045E,
    .ProductID = 0x0202,
    .ReleaseNumber = VERSION_BCD(1, 2, 1),

    .ManufacturerStrIndex = 0,
    .ProductStrIndex = 0,
    .SerialNumStrIndex = NO_DESCRIPTOR,

    .NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS};

const USB_Descriptor_Configuration_t PROGMEM ConfigurationDescriptor = {
    .Config = {.Header = {.Size = sizeof(USB_Descriptor_Configuration_Header_t),
                          .Type = DTYPE_Configuration},

               .TotalConfigurationSize = sizeof(USB_Descriptor_Configuration_t),
               .TotalInterfaces = 1,

               .ConfigurationNumber = 1,
               .ConfigurationStrIndex = 0,
               .ConfigAttributes = USB_CONFIG_ATTR_RESERVED,

               .MaxPowerConsumption = USB_CONFIG_POWER_MA(100)},
    .HID_Interface = {.Header = {.Size = sizeof(USB_Descriptor_Interface_t),
                                 .Type = DTYPE_Interface},
                      .InterfaceNumber = 0,
                      .AlternateSetting = 0x00,
                      .TotalEndpoints = 2,
                      .Class = HID_XPAD_Class,
                      .SubClass = HID_XPAD_Subclass,
                      .Protocol = 0,
                      .InterfaceStrIndex = NO_DESCRIPTOR},
    .HID_ReportINEndpoint = {.Header = {.Size =
                                            sizeof(USB_Descriptor_Endpoint_t),
                                        .Type = DTYPE_Endpoint},
                             .EndpointAddress = 0x81,
                             .Attributes =
                                 (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC |
                                  ENDPOINT_USAGE_DATA),
                             .EndpointSize = 0x20,
                             .PollingIntervalMS = 4},
    .HID_ReportOUTEndpoint = {
        .Header = {.Size = sizeof(USB_Descriptor_Endpoint_t),
                   .Type = DTYPE_Endpoint},
        .EndpointAddress = 0x02,
        .Attributes =
            (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
        .EndpointSize = 0x20,
        .PollingIntervalMS = 4}};

const USB_Descriptor_String_t PROGMEM LanguageString =
    USB_STRING_DESCRIPTOR_ARRAY(LANGUAGE_ID_ENG);

const USB_Descriptor_String_t PROGMEM ManufacturerString =
    USB_STRING_DESCRIPTOR(L"Drunken Bosnian");

const USB_Descriptor_String_t PROGMEM ProductString =
    USB_STRING_DESCRIPTOR(L"Il Duce Universal Adapter");

uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
                                    const uint16_t wIndex,
                                    const void **const DescriptorAddress) {
    const uint8_t DescriptorType = (wValue >> 8);
    const uint8_t DescriptorNumber = (wValue & 0xFF);

    const void *Address = NULL;
    uint16_t Size = NO_DESCRIPTOR;

    switch (DescriptorType) {
        case DTYPE_Device:
            Address = &DeviceDescriptor;
            Size = sizeof(USB_Descriptor_Device_t);
            break;
        case DTYPE_Configuration:
            Address = &ConfigurationDescriptor;
            Size = sizeof(USB_Descriptor_Configuration_t);
            break;
        case DTYPE_String:
            switch (DescriptorNumber) {
                case STRING_ID_Language:
                    Address = &LanguageString;
                    Size = pgm_read_byte(&LanguageString.Header.Size);
                    break;
                case STRING_ID_Manufacturer:
                    Address = &ManufacturerString;
                    Size = pgm_read_byte(&ManufacturerString.Header.Size);
                    break;
                case STRING_ID_Product:
                    Address = &ProductString;
                    Size = pgm_read_byte(&ProductString.Header.Size);
                    break;
            }
            break;
    }

    *DescriptorAddress = Address;
    return Size;
}