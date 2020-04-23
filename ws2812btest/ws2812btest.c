#include "elecanisms.h"
#include "usb.h"
#include "ws2812b.h"

#define TOGGLE_LED1         0
#define TOGGLE_LED2         1
#define TOGGLE_LED3         2
#define READ_SW1            3
#define READ_SW2            4
#define READ_SW3            5
#define WRITE_WS2812BS      6

#define BUFFER_LENGTH       1024

uint8_t data_buffer[BUFFER_LENGTH];

void ws2812b_send_data_buffer(void) {
    uint16_t i;

    disable_interrupts();
    for (i = 0; i < USB_request.setup.wLength.w; i += 3) 
        ws2812b_write(data_buffer[i], data_buffer[i + 1], data_buffer[i + 2]);
    enable_interrupts();
}

void vendor_requests(void) {
    WORD temp;
    uint16_t i;

    switch (USB_setup.bRequest) {
        case TOGGLE_LED1:
            LED1 = !LED1;
            BD[EP0IN].bytecount = 0;
            BD[EP0IN].status = UOWN | DTS | DTSEN;
            break;
        case TOGGLE_LED2:
            LED2 = !LED2;
            BD[EP0IN].bytecount = 0;
            BD[EP0IN].status = UOWN | DTS | DTSEN;
            break;
        case TOGGLE_LED3:
            LED3 = !LED3;
            BD[EP0IN].bytecount = 0;
            BD[EP0IN].status = UOWN | DTS | DTSEN;
            break;
        case READ_SW1:
            BD[EP0IN].address[0] = SW1 ? 1 : 0;
            BD[EP0IN].bytecount = 1;
            BD[EP0IN].status = UOWN | DTS | DTSEN;
            break;
        case READ_SW2:
            BD[EP0IN].address[0] = SW2 ? 1 : 0;
            BD[EP0IN].bytecount = 1;
            BD[EP0IN].status = UOWN | DTS | DTSEN;
            break;
        case READ_SW3:
            BD[EP0IN].address[0] = SW3 ? 1 : 0;
            BD[EP0IN].bytecount = 1;
            BD[EP0IN].status = UOWN | DTS | DTSEN;
            break;
        case WRITE_WS2812BS:
            if (USB_setup.wLength.w > BUFFER_LENGTH)
                USB_error_flags |= REQUEST_ERROR;
            else {
                USB_request.setup.bmRequestType = USB_setup.bmRequestType;
                USB_request.setup.bRequest = USB_setup.bRequest;
                USB_request.setup.wValue.w = USB_setup.wValue.w;
                USB_request.setup.wIndex.w = USB_setup.wIndex.w;
                USB_request.setup.wLength.w = USB_setup.wLength.w;
                USB_request.bytes_left.w = USB_setup.wLength.w;
                USB_request.data_ptr = data_buffer;
                USB_request.done_callback = ws2812b_send_data_buffer;
                USB_out_callbacks[0] = usb_receive_data_packet;
            }
            break;
        default:
            USB_error_flags |= REQUEST_ERROR;
    }
}

int16_t main(void) {
    init_elecanisms();
    init_ws2812b();

    USB_setup_vendor_callback = vendor_requests;
    init_usb();

    while (USB_USWSTAT != CONFIG_STATE) {
#ifndef USB_INTERRUPT
        usb_service();
#endif
    }
    while (1) {
#ifndef USB_INTERRUPT
        usb_service();
#endif
    }
}

